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
  m_pVShader = g_GraphicAPI().internalCreateShader();
  m_pPShader = g_GraphicAPI().internalCreateShader();
  m_pInputLayout = make_shared<InputLayout>();
  m_pSamplerState = make_shared<SamplerState>();
}

Pass::Pass(PassDesc& _desc)
{
  // call the api manager
  GraphicsAPI& api = g_GraphicAPI().instance();
  // create all pointers
  m_pVShader = api.internalCreateShader();
  m_pPShader = api.internalCreateShader();
  m_pInputLayout = make_shared<InputLayout>();
  m_pSamplerState = make_shared<SamplerState>();
  // create the shaders
  createVShader(_desc.vSDirectory, _desc.vSEntry, _desc.vSModel);
  createPShader(_desc.pSDirectory, _desc.pSEntry, _desc.pSModel);
  // create input and sampler state
  m_pInputLayout = api.createInputLayoutFromVShader(m_pVShader);
  m_pSamplerState = api.createSamplerState(_desc.samAdress, _desc.samFilters);
  // create a buffer for each size in the vector
  for (uint32 i = 0; i < _desc.cBSizes.size(); ++i) {
    m_cBuffers.push_back(api.createConstantBuffer(static_cast<uint32>(_desc.cBSizes[i])));
  }
  // set input, output and depth
  m_inputTex = _desc.inputs;
  m_outputTex = _desc.outputs;
  m_depthTex = _desc.pDepth;
}

void
Pass::createVShader(const WString _directory, const char* _entry, const char* _sModel)
{
  m_pVShader->setData(_directory, _entry, _sModel);
  m_pVShader->compile();
  g_GraphicAPI().createVShader(m_pVShader);
}

void
Pass::createPShader(const WString _directory, const char* _entry, const char* _sModel)
{
  m_pPShader->setData(_directory, _entry, _sModel);
  m_pPShader->compile();
  g_GraphicAPI().createPShader(m_pPShader);
}

void
Pass::compileShaders()
{
  m_pVShader->compile();
  m_pPShader->compile();
}

// to do: properly link passes with the textures
void
Pass::beginPass(Color _color)
{
  // get managers
  GraphicsAPI& api = g_GraphicAPI().instance();
  RendererManager& renderManager = g_RenderManager().instance();
  // clear RTVs and Depth stencil
  api.clearRenderTargetViews(_color, m_outputTex);
  api.clearDepthBuffer(1.0f, m_depthTex);
  // set render targets and depth texture
  api.setRenderTargets(m_outputTex, m_depthTex);
  // set input layout of shader
  api.setInputLayout(getInputLayout());
  // set the shaders
  api.setVSShader(getVShader());
  api.setPSShader(getPShader());
  // set resources
  for (uint32 i = 0; i < m_inputTex.size(); ++i) {
    api.pSSetShaderResourceView(m_inputTex[i], i);
  }
  // set the sampler state
  api.setSampler(getSamplerState());
  // set constant buffers
  renderManager.pSSetConstantBuffers(getCBuffers());
  renderManager.vSSetConstantBuffers(getCBuffers());
}

void
Pass::endPass()
{
  // get managers
  GraphicsAPI& api = g_GraphicAPI().instance();
  // set all to nullptr
  api.setRenderTarget(nullptr);
  api.setInputLayout(nullptr);
  api.setVSShader(nullptr);
  api.setPSShader(nullptr);
  for (uint32 i = 0; i < m_inputTex.size(); ++i) {
    api.pSSetShaderResourceView(nullptr, i);
  }
  api.setSampler(nullptr);
  for (uint32 i = 0; i < m_cBuffers.size(); ++i) {
    api.pSSetConstantBuffer(nullptr, i);
    api.vSSetConstantBuffer(nullptr, i);
  }
}
}
