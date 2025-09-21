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
#include "pkGraphicsAPI.h"
#include "pkPass.h"
#include "pkRendererManager.h"
#include "pkSceneManager.h"

#include <iostream>

namespace pkEngineSDK
{

Pass::Pass() {
  m_pInputLayout = make_shared<InputLayout>();
  m_pSamplerState = make_shared<SamplerState>();
}

Pass::Pass(PassDesc& _desc)
{
  // call the api manager
  GraphicsAPI& api = g_GraphicAPI();
  m_pSamplerState = make_shared<SamplerState>();
  // Try to create the vertex shader if there's a path.
  if (!_desc.vSDirectory.getPath().empty()) {
    m_pVShader = api.internalCreateShader();
    createVShader(_desc.vSDirectory, _desc.vSEntry, _desc.vSModel);
    // create all pointers
    m_pInputLayout = make_shared<InputLayout>();
    // create input and sampler state
    m_pInputLayout = api.createInputLayoutFromVShader(m_pVShader);
  };
  // Try to create the pixel shader if there's a path.
  if (!_desc.pSDirectory.getPath().empty()) {
    m_pPShader = api.internalCreateShader();
    createPShader(_desc.pSDirectory, _desc.pSEntry, _desc.pSModel);
  }
  // Try to create the compute shader if there's a path.
  if (!_desc.cSDirectory.getPath().empty()) {
    m_pCShader = api.internalCreateShader();
    createCShader(_desc.cSDirectory, _desc.cSEntry, _desc.cSModel);
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

Pass::~Pass()
{
  for (uint32 i = 0; i < m_cBuffers.size(); ++i) {
    m_cBuffers[i].reset();
  }
  m_cBuffers.clear();

  for (uint32 i = 0; i < m_inputTex.size(); ++i) {
    m_inputTex[i].reset();
  }
  m_inputTex.clear();

  for (uint32 i = 0; i < m_outputTex.size(); ++i) {
    m_outputTex[i].reset();
  }
  m_outputTex.clear();

  for (uint32 i = 0; i < m_uavTex.size(); ++i) {
    m_uavTex[i].reset();
  }
  m_uavTex.clear();

  m_depthTex = nullptr;
}

void
Pass::createVShader(const Path _directory, const char* _entry, const char* _sModel)
{
  m_pVShader->setData(_directory, _entry, _sModel);
  m_pVShader->compile();
  g_GraphicAPI().createVShader(m_pVShader);
}

void
Pass::createPShader(const Path _directory, const char* _entry, const char* _sModel)
{
  m_pPShader->setData(_directory, _entry, _sModel);
  m_pPShader->compile();
  g_GraphicAPI().createPShader(m_pPShader);
}

void
Pass::createCShader(const Path _directory, const char* _entry, const char* _sModel)
{
  m_pCShader->setData(_directory, _entry, _sModel);
  m_pCShader->compile();
  g_GraphicAPI().createCShader(m_pCShader);
}

void
Pass::compileShaders()
{
  if (m_pVShader) {
    m_pVShader->compile();
    g_GraphicAPI().createVShader(m_pVShader);
  }
  if (m_pPShader) {
    m_pPShader->compile();
    g_GraphicAPI().createPShader(m_pPShader);
  }
  if (m_pCShader) {
    m_pCShader->compile();
    g_GraphicAPI().createCShader(m_pCShader);
  }
}

// to do: properly link passes with the textures
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
  // set the shaders
  api.setVShader(getVShader());
  api.setPShader(getPShader());
  api.setCShader(getCShader());
  // set resources
  if (!m_inputTex.empty()) {
    api.vSSetShaderResourceViews(m_inputTex);
    api.pSSetShaderResourceViews(m_inputTex);
    api.cSSetShaderResourceViews(m_inputTex);
  }
  if (!m_uavTex.empty()) {
    api.cSSetUnorderedAccessViews(m_uavTex);
  }
  // set the sampler state
  api.setSampler(getSamplerState());
  // set constant buffers
  api.pSSetConstantBuffers(getCBuffers());
  api.vSSetConstantBuffers(getCBuffers());
  api.cSSetConstantBuffers(getCBuffers());
  // set the rasterizer state
  api.setRasterizerState(m_pRasterizerState);
}

void
Pass::endPass()
{
  // get managers
  GraphicsAPI& api = g_GraphicAPI();
  // set all to nullptr
  SIZE_T resourceCount = m_inputTex.size();
  SIZE_T uavCount = m_uavTex.size();
  SIZE_T renderTargetCount = m_outputTex.size();

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
