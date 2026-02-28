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
  m_cBuffers.clear();
  m_inputTex.clear();
  m_outputTex.clear();
  m_uavTex.clear();
}

Pass::Pass(const PassDesc& _desc)
{
  // call the api manager
  GraphicsAPI& api = g_GraphicAPI();
  AssetResourceManager& assetMan = g_AssetResourceManager();
  m_pSamplerState = make_shared<SamplerState>();
  // Try to create the vertex shader if there's a path.
  if (!_desc.vSDirectory.empty()) {
    SPtr<BaseResource> res = assetMan.getResourceBydirectory(_desc.vSDirectory);
    m_pVShader = api.internalCreateShader();
    m_pVShader->compileFromResource(res);
    api.createVShader(m_pVShader);
    // create the input layout for the shader
    m_pInputLayout = api.createInputLayoutFromVShader(m_pVShader);
  };
  // Try to create the pixel shader if there's a path.
  if (!_desc.pSDirectory.empty()) {
    SPtr<BaseResource> res = assetMan.getResourceBydirectory(_desc.pSDirectory);
    m_pPShader = api.internalCreateShader();
    m_pPShader->compileFromResource(res);
    api.createPShader(m_pPShader);
  }
  // Try to create the compute shader if there's a path.
  if (!_desc.cSDirectory.empty()) {
    SPtr<BaseResource> res = assetMan.getResourceBydirectory(_desc.cSDirectory);
    m_pCShader = api.internalCreateShader();
    m_pCShader->compileFromResource(res);
    api.createCShader(m_pCShader);
  }
  // create the sampler state
  m_pSamplerState = api.createSamplerState(_desc.samAdress, _desc.samFilters);
  // create a buffer for each size in the vector
  for (uint32 i = 0; i < _desc.cBSizes.size(); ++i) {
    m_cBuffers.push_back(api.createConstantBuffer(static_cast<uint32>(_desc.cBSizes[i])));
  }
  if (_desc.rSExists) {
    // rasterizer state
    RASTERIZER_DESC rDesc = {};
    rDesc.fillMode = _desc.rSFillMode;
    rDesc.cullMode = _desc.rSCullMode;
    rDesc.frontCounterClockwise = _desc.rSFrontCounterClockwise;
    rDesc.depthClipEnable = _desc.rSDepthClipEnable;
    // create the rasterizer state
    m_pRasterizerState = api.createRasterizerState(rDesc);
  }
  // set input, output and depth
  m_inputTex = _desc.inputs;
  m_outputTex = _desc.outputs;
  m_uavTex = _desc.uavs;
  m_depthTex = _desc.pDepth;

  // get the viewport size from the needed render target or unordered access view.
  m_viewPortSize = Vector2(0.0f);
  if (!m_outputTex.empty()) {
    m_viewPortSize = m_outputTex[0]->getSize();
  }
  else if (m_uavTex.empty()) {
    m_viewPortSize = m_uavTex[0]->getSize();
  }
}

void
Pass::createVShader(const Path _directory, const ANSICHAR* _entry, const ANSICHAR* _sModel)
{
  ShaderManager& shaderMan = g_ShaderManager();

  // check if the shader already exists
  const ShaderKey key(_directory.toString(), _entry, _sModel);
  SPtr<Shader> checkShader = shaderMan.getShader(key);

  // if the shader exists, get the shader and return.
  if (checkShader) {
    m_pVShader = checkShader;
    const String msg = "Found previously compiled Vertex shader: " + key.shaderPath;
    LOG_REGISTER(msg, __FILE__, __LINE__);
    return;
  }
  // if it doesnt exist, continue with the compilation and store the shader.
  m_pVShader->setData(_directory, _entry, _sModel);
  m_pVShader->compileFromFile();
  g_GraphicAPI().createVShader(m_pVShader);
  g_ShaderCodec().createResourceFromShader(m_pVShader);

  shaderMan.insertShader(key, m_pVShader);
}

void
Pass::createPShader(const Path _directory, const ANSICHAR* _entry, const ANSICHAR* _sModel)
{
  ShaderManager& shaderMan = g_ShaderManager();
  const ShaderKey key(_directory.toString(), _entry, _sModel);
  SPtr<Shader> checkShader = shaderMan.getShader(key);

  // if the shader exists, get the shader and return.
  if (checkShader) {
    m_pPShader = checkShader;
    const String msg = "Found previously compiled Pixel shader: " + key.shaderPath;
    LOG_REGISTER(msg, __FILE__, __LINE__);
    return;
  }

  m_pPShader->setData(_directory, _entry, _sModel);
  m_pPShader->compileFromFile();
  g_GraphicAPI().createPShader(m_pPShader);
  g_ShaderCodec().createResourceFromShader(m_pPShader);

  shaderMan.insertShader(key, m_pPShader);
}

void
Pass::createCShader(const Path _directory, const ANSICHAR* _entry, const ANSICHAR* _sModel)
{
  ShaderManager& shaderMan = g_ShaderManager();
  const ShaderKey key(_directory.toString(), _entry, _sModel);
  SPtr<Shader> checkShader = shaderMan.getShader(key);

  // if the shader exists, get the shader and return.
  if (checkShader) {
    m_pCShader = checkShader;
    const String msg = "Found previously compiled Compute shader: " + key.shaderPath;
    LOG_REGISTER(msg, __FILE__, __LINE__);
    return;
  }

  m_pCShader->setData(_directory, _entry, _sModel);
  m_pCShader->compileFromFile();
  g_GraphicAPI().createCShader(m_pCShader);
  g_ShaderCodec().createResourceFromShader(m_pCShader);

  shaderMan.insertShader(key, m_pCShader);
}

void
Pass::compileShaders()
{
  GraphicsAPI& api = g_GraphicAPI();
  if (m_pVShader) {
    m_pVShader->compileFromFile();
    api.createVShader(m_pVShader);
  }
  if (m_pPShader) {
    m_pPShader->compileFromFile();
    api.createPShader(m_pPShader);
  }
  if (m_pCShader) {
    m_pCShader->compileFromFile();
    api.createCShader(m_pCShader);
  }
}

void
Pass::updateCBuffers(const Vector<const void*>& _data,
                     const Vector<SIZE_T>& _sizes)
{
  const SIZE_T blobCount = _data.size();
  // assert that all data counts are the same.
  PK_ASSERT(blobCount == _sizes.size());
  PK_ASSERT(m_cBuffers.size() == blobCount);

  GraphicsAPI& api = g_GraphicAPI();
  
  const uint32 CBufferCount = static_cast<uint32>(m_cBuffers.size());
  for (uint32 i = 0; i < CBufferCount; ++i) {
    api.updateConstantBuffer(m_cBuffers[i], _data[i], _sizes[i]);
  }
}

void
Pass::updateCBuffer(const uint32 _index, const void* _data, const SIZE_T _size)
{
  PK_ASSERT(_index < m_cBuffers.size());
  g_GraphicAPI().updateConstantBuffer(m_cBuffers[_index], _data, _size);
}

void
Pass::beginPass(Color _color)
{
  // get managers
  GraphicsAPI& api = g_GraphicAPI();
  // clear RTVs and Depth stencil
  api.clearRenderTargetViews(_color, m_outputTex);
  api.clearDepthBuffer(1.0f, m_depthTex);
  // set render targets and depth texture
  api.setRenderTargets(m_outputTex, m_depthTex);
  // set the viewport size.
  api.setViewport(m_viewPortSize);
  // api.resizeSwapChain(m_viewPortSize);
  // set input layout of shader
  api.setInputLayout(getInputLayout());

  // if there are shaders to set, set them and their resources.
  if (api.setVShader(getVShader())) { // Vertex shader.
    api.vSSetShaderResourceViews(m_inputTex);
    api.vSSetConstantBuffers(getCBuffers());
  }
  if (api.setPShader(getPShader())) { // Pixel shader.
    api.pSSetShaderResourceViews(m_inputTex);
    api.pSSetConstantBuffers(getCBuffers());
  }
  if (api.setCShader(getCShader())) { // Compute shader.
    api.cSSetShaderResourceViews(m_inputTex);
    api.cSSetUnorderedAccessViews(m_uavTex);
    api.cSSetConstantBuffers(getCBuffers());
  }
  // set the sampler state
  api.setSampler(getSamplerState());

  // set the rasterizer state
  api.setRasterizerState(m_pRasterizerState);
}

void
Pass::endPass()
{
  // get managers
  GraphicsAPI& api = g_GraphicAPI();
  // set all to nullptr
  const SIZE_T resourceCount = m_inputTex.size();
  const SIZE_T uavCount = m_uavTex.size();
  const SIZE_T renderTargetCount = m_outputTex.size();

  if (renderTargetCount > 0) {
    api.unbindRenderTargets(renderTargetCount);
  }
  api.setInputLayout(nullptr);
  api.setVShader(nullptr);
  api.setPShader(nullptr);
  api.setCShader(nullptr);
  if (resourceCount > 0) {
    api.vSUnbindShaderResourceViews(resourceCount);
    api.pSUnbindShaderResourceViews(resourceCount);
    api.cSUnbindShaderResourceViews(resourceCount);
  }
  if (uavCount > 0) {
    api.cSUnbindUnorderedAccessViews(uavCount);
  }
  api.setSampler(nullptr);
  api.vSUnbindConstantBuffers();
  api.pSUnbindConstantBuffers();
  api.cSUnbindConstantBuffers();
  api.setRasterizerState(nullptr);
}
}
