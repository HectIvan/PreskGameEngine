/*****************************************************************************/
/**
 * @file    pkPass.cpp
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    07/02/2025
 * @brief   Source file of the pass
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkAssetResourceManager.h"
#include "pkGraphicsAPI.h"
#include "pkPass.h"
#include "pkRendererManager.h"
#include "pkSceneManager.h"
#include "pkShaderCodec.h"
#include "pkShaderManager.h"
#include "pkLogger.h"

namespace pkEngineSDK
{

Pass::Pass() {
  m_pInputLayout = make_shared<InputLayout>();
  m_pSamplerState = make_shared<SamplerState>();
}

void
Pass::clear()
{
  if (m_cBuffers.size() > 0) {
    m_cBuffers.clear();
  }
  if (m_inputTex.size() > 0) {
    for (uint32 i = 0; i < m_inputTex.size(); ++i) {
      int32 count = m_inputTex[i].use_count();
      const String msg = "input Tex of pass " + m_name + " index " + to_string(i) +" has " + to_string(count) + " references.";
      LOG_REGISTER(msg, __FILE__, __LINE__);
    }
    m_inputTex.clear();
  }
  if (m_outputTex.size() > 0) {
    m_outputTex.clear();
  }
  if (m_uavTex.size() > 0) {
    m_uavTex.clear();
  }
}

Pass::Pass(const PixelDesc& _desc)
{
  PK_ASSERT(!_desc.outputs.empty() && "Pixel Pass has no output textures.");

  GraphicsAPI& api = g_GraphicAPI();
  ShaderManager& shaderMan = g_ShaderManager();

  bool shaderDirty = false;
  /**
   * ----------------------- VERTEX SHADER -----------------------
   */
  if (!_desc.vSKey.isEmpty()) {
    m_pVShader = shaderMan.getShader(_desc.vSKey);
    // create the input layout for the shader.
    m_pInputLayout = api.createInputLayoutFromVShader(m_pVShader.lock());
    shaderDirty = true;
  };
  // verify if the shader was created correctly.
  if (shaderDirty && m_pVShader.expired()) {
    const String msg = "Failed to internally create vertex shader for pass. Path: " +
                       _desc.vSKey.shaderPath +
                       ". Shader is expired.";
    LOG_FATAL(msg, __FILE__, __LINE__);
    THROW_ERROR(msg);
    return;
  }
  /**
   * ----------------------- PIXEL SHADER -----------------------
   */
  shaderDirty = false;
  if (!_desc.pSKey.isEmpty()) {
    m_pPShader = shaderMan.getShader(_desc.pSKey);
    shaderDirty = true;
  }
  // verify if the shader was created correctly.
  if (shaderDirty && m_pPShader.expired()) {
    const String msg = "Failed to internally create pixel shader for pass. Path: " +
                       _desc.pSKey.shaderPath +
                       ". Shader is expired.";
    LOG_FATAL(msg, __FILE__, __LINE__);
    THROW_ERROR(msg);
    return;
  }

  createBasics(_desc);

  // set input, output and depth.
  m_inputTex = _desc.inputs;
  m_outputTex = _desc.outputs;
  m_depthTex = _desc.pDepth;
  m_viewPortSize = m_outputTex[0].lock()->getSize();

  m_passModel = PASS_MODEL::kPass_Pixel;
}

Pass::Pass(const ComputeDesc& _desc)
{
  PK_ASSERT(!m_uavTex.empty() && "Compute Pass has no UAVs set.");

  ShaderManager& shaderMan = g_ShaderManager();

  /**
   * ----------------------- COMPUTE SHADER -----------------------
   */
  bool shaderDirty = false;
  if (!_desc.cSKey.isEmpty()) {
    m_pCShader = shaderMan.getShader(_desc.cSKey);
    shaderDirty = true;
  }
  // verify if the shader was created correctly.
  if (shaderDirty && m_pCShader.expired()) {
    const String msg = "Failed to internally create compute shader for pass. Path: " +
      _desc.cSKey.shaderPath +
      ". Shader is expired.";
    LOG_FATAL(msg, __FILE__, __LINE__);
    THROW_ERROR(msg);
    return;
  }

  createBasics(_desc);

  m_viewPortSize = m_uavTex[0]->getSize();

  m_inputTex = _desc.inputs;
  m_outputTex = _desc.outputs;
  m_uavTex = _desc.uavs;

  m_passModel = PASS_MODEL::kPass_Compute;
}

void
Pass::compileShaders()
{
  GraphicsAPI& api = g_GraphicAPI();
  SPtr<Shader> vShader = m_pVShader.lock();
  if (vShader) {
    vShader->compileFromFile();
    m_pVShader = api.createVShader(vShader);
  }
  SPtr<Shader> pShader = m_pPShader.lock();
  if (pShader) {
    pShader->compileFromFile();
    m_pPShader = api.createPShader(pShader);
  }
  SPtr<Shader> cShader = m_pCShader.lock();
  if (cShader) {
    cShader->compileFromFile();
    m_pCShader = api.createCShader(cShader);
  }
}

void
Pass::updateCBuffers(const Vector<const void*>& _data,
                     const Vector<SIZE_T>& _sizes)
{
  // assert that all data counts are the same.
  const SIZE_T blobCount = _data.size();
  PK_ASSERT(blobCount == _sizes.size() && "CBuffer blob count & sizes count do not match.");
  const uint32 CBufferCount = static_cast<uint32>(m_cBuffers.size());
  PK_ASSERT(CBufferCount == blobCount && "CBuffer update count does not match pass CBuffer count.");

  GraphicsAPI& api = g_GraphicAPI();
  
  for (uint32 i = 0; i < CBufferCount; ++i) {
    api.updateConstantBuffer(m_cBuffers[i], _data[i], _sizes[i]);
  }
}

void
Pass::updateCBuffer(const uint32 _index, const void* _data, const SIZE_T _size)
{
  PK_ASSERT(_index < m_cBuffers.size() && "CBuffer update index out of range.");
  g_GraphicAPI().updateConstantBuffer(m_cBuffers[_index], _data, _size);
}

void
Pass::beginPass(const FColor& _color, const bool& _clearRT)
{
  GraphicsAPI& api = g_GraphicAPI();

  // check what type of pass it is and call the corresponding begin function.
  if (PASS_MODEL::kPass_Pixel == m_passModel) {
    beginPixel(_color, _clearRT);
  }
  else if (PASS_MODEL::kPass_Compute == m_passModel) {
    beginCompute(_color, _clearRT);
  }
  else if (PASS_MODEL::kPass_Geometry == m_passModel) {
    beginGeometry(_color);
  }
  else {
    const String msg = "Pass model not set correctly.";
    LOG_FATAL(msg, __FILE__, __LINE__);
    THROW_ERROR(msg);
    return;
  }

  // set the viewport size.
  api.setViewport(m_viewPortSize);

  // set the sampler state
  api.setSampler(m_pSamplerState);

  // set the rasterizer state
  api.setRasterizerState(m_pRasterizerState);
}

void
Pass::endPass()
{
  // get managers
  GraphicsAPI& api = g_GraphicAPI();

  // check what type of pass it is and call the corresponding end function.
  if (PASS_MODEL::kPass_Pixel == m_passModel) {
    endPixel();
  }
  else if (PASS_MODEL::kPass_Compute == m_passModel) {
    endCompute();
  }
  else if (PASS_MODEL::kPass_Geometry == m_passModel) {
    endGeometry();
  }

  else {
    const String msg = "Pass model not set correctly.";
    LOG_FATAL(msg, __FILE__, __LINE__);
    THROW_ERROR(msg);
    return;
  }
  
  api.setSampler(nullptr);
  api.setRasterizerState(nullptr);
}

void
Pass::beginPixel(const FColor& _color, const bool& _clearRT)
{
  // get managers
  GraphicsAPI& api = g_GraphicAPI();
  // clear RTVs and Depth stencil
  if (_clearRT) { // to do: temporary. maybe it should have a flag for depth and another for render targets.
    api.clearRenderTargetViews(_color, m_outputTex);
    api.clearDepthBuffer(1.0f, m_depthTex);
  }
  // set render targets and depth texture
  api.setRenderTargets(m_outputTex, m_depthTex);
  // api.resizeSwapChain(m_viewPortSize);
  // set input layout of shader
  api.setInputLayout(getInputLayout());

  // if there are shaders to set, set them and their resources.
  SPtr<Shader> vShader = m_pVShader.lock();
  if (api.setVShader(vShader)) { // Vertex shader.
    api.vSSetShaderResourceViews(m_inputTex);
    api.vSSetConstantBuffers(getCBuffers());
  }
  SPtr<Shader> pShader = getPShader().lock();
  if (api.setPShader(pShader)) { // Pixel shader.
    api.pSSetShaderResourceViews(m_inputTex);
    api.pSSetConstantBuffers(getCBuffers());
  }
}

void
Pass::endPixel()
{
  GraphicsAPI& api = g_GraphicAPI();

  const SIZE_T renderTargetCount = m_outputTex.size();
  const SIZE_T resourceCount = m_inputTex.size();

  api.unbindRenderTargets(renderTargetCount);
  api.vSUnbindShaderResourceViews(resourceCount);
  api.pSUnbindShaderResourceViews(resourceCount);

  api.vSUnbindConstantBuffers();
  api.pSUnbindConstantBuffers();

  api.setInputLayout(nullptr);
  api.setVShader(nullptr);
  api.setPShader(nullptr);
}

void
Pass::beginCompute(const FColor& _color, const bool& _clearUAV)
{
  GraphicsAPI& api = g_GraphicAPI();

  if (_clearUAV) {
    api.clearUnorderedAccessViews(m_uavTex, _color);
  }

  SPtr<Shader> cShader = getCShader().lock();
  if (api.setCShader(cShader)) { // Compute shader.
    api.cSSetShaderResourceViews(m_inputTex);
    api.cSSetUnorderedAccessViews(m_uavTex);
    api.cSSetConstantBuffers(getCBuffers());
  }
}

void
Pass::endCompute()
{
  GraphicsAPI& api = g_GraphicAPI();

  const SIZE_T uavCount = m_uavTex.size();
  const SIZE_T resourceCount = m_inputTex.size();
  const SIZE_T cBufferCount = m_cBuffers.size();

  api.cSUnbindShaderResourceViews(resourceCount);
  api.cSUnbindUnorderedAccessViews(uavCount);

  api.cSUnbindConstantBuffers(cBufferCount);

  api.setCShader(nullptr);
}

void
Pass::beginGeometry(const FColor&)
{
}

void
Pass::endGeometry()
{
}

void
Pass::createBasics(const PassDesc& _desc)
{
  GraphicsAPI& api = g_GraphicAPI();

  // create the sampler state
  m_pSamplerState = api.createSamplerState(_desc.samAdress, _desc.samFilters);
  // create a buffer for each size in the vector
  const uint32 cBufferCount = static_cast<uint32>(_desc.cBSizes.size());
  for (uint32 i = 0; i < cBufferCount; ++i) {
    m_cBuffers.push_back(api.createConstantBuffer(static_cast<uint32>(_desc.cBSizes[i])));
  }

  if (_desc.rSExists) {
    // rasterizer state.
    RASTERIZER_DESC rDesc = {};
    rDesc.fillMode = _desc.rSFillMode;
    rDesc.cullMode = _desc.rSCullMode;
    rDesc.frontCounterClockwise = _desc.rSFrontCounterClockwise;
    rDesc.depthClipEnable = _desc.rSDepthClipEnable;
    // create the rasterizer state.
    m_pRasterizerState = api.createRasterizerState(rDesc);
  }
}
}
