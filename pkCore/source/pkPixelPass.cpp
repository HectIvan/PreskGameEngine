/*****************************************************************************/
/**
 * @file    pkPixelPass.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    29/08/2026
 * @brief   Source file for a pixel pass.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#include "pkGraphicsAPI.h"
#include "pkPixelPass.h"
#include "pkLogger.h"

namespace pkEngineSDK
{
PixelPass::PixelPass(const PixelPassDesc& _desc)
{
  m_name = _desc.name;

  m_pVShader = _desc.pVShader;
  m_pPShader = _desc.pPShader;

  /**
   * Create pass buffers for the vertex and pixel shaders.
   */
  const uint32 vBufferCount = static_cast<uint32>(_desc.vSBuffers.size());
  for (uint32 i = 0; i < vBufferCount; ++i) {
    m_vCBuffers.push_back(pk_shared_ptr_new<ConstantBuffer>(_desc.vSBuffers[i]));
  }

  const uint32 pBufferCount = static_cast<uint32>(_desc.pSBuffers.size());
  for (uint32 i = 0; i < pBufferCount; ++i) {
    m_pCBuffers.push_back(pk_shared_ptr_new<ConstantBuffer>(_desc.pSBuffers[i]));
  }

  /**
   * Assign textures for the vertex and pixel shaders.
   */
  m_VTex = _desc.vTex;
  m_PTex = _desc.pTex;

  m_outputTex = _desc.outputs;
  m_depthTex = _desc.pDepth;
}

void
PixelPass::clear()
{
  LOG_REGISTER(">---Clearing Pixel Pass " + m_name + "---<", __FILE__, __LINE__);
  m_pVShader.reset();
  m_pPShader.reset();
  m_vCBuffers.clear();
  m_pCBuffers.clear();
  m_VTex.clear();
  m_PTex.clear();
  m_outputTex.clear();
  m_depthTex.reset();
  LOG_REGISTER("<---Finished clearing Pixel Pass " + m_name + "--->", __FILE__, __LINE__);
}

void
PixelPass::updateVCBuffers(const Vector<const void*>& _data, const Vector<SIZE_T>& _sizes) const
{
  // assert that all data counts are the same.
  const SIZE_T blobCount = _data.size();
  PK_ASSERT(blobCount == _sizes.size() && "CBuffer blob count & sizes count do not match.");
  const uint32 CBufferCount = toUint32(m_vCBuffers.size());
  PK_ASSERT(CBufferCount == blobCount && "CBuffer update count does not match pass CBuffer count.");

  GraphicsAPI& api = g_GraphicAPI();

  for (uint32 i = 0; i < CBufferCount; ++i) {
    api.updateConstantBuffer(m_vCBuffers[i], _data[i], _sizes[i]);
  }
}

void
PixelPass::updateVCBuffer(const uint32 _index, const void* _data, const SIZE_T _size) const
{
  PK_ASSERT(_index < m_vCBuffers.size() && "Vertex CBuffer update index out of range.");
  g_GraphicAPI().updateConstantBuffer(m_vCBuffers[_index], _data, _size);
}

void
PixelPass::updatePCBuffers(const Vector<const void*>& _data, const Vector<SIZE_T>& _sizes) const
{
  // assert that all data counts are the same.
  const SIZE_T blobCount = _data.size();
  PK_ASSERT(blobCount == _sizes.size() && "CBuffer blob count & sizes count do not match.");
  const uint32 CBufferCount = toUint32(m_pCBuffers.size());
  PK_ASSERT(CBufferCount == blobCount && "CBuffer update count does not match pass CBuffer count.");

  GraphicsAPI& api = g_GraphicAPI();

  for (uint32 i = 0; i < CBufferCount; ++i) {
    api.updateConstantBuffer(m_pCBuffers[i], _data[i], _sizes[i]);
  }
}

void
PixelPass::updatePCBuffer(const uint32 _index, const void* _data, const SIZE_T _size) const
{
  PK_ASSERT(_index < m_pCBuffers.size() && "Pixel CBuffer update index out of range.");
  g_GraphicAPI().updateConstantBuffer(m_pCBuffers[_index], _data, _size);
}

void
PixelPass::beginPass(const FColor& _color)
{
  GraphicsAPI& api = g_GraphicAPI();

  api.setRenderTargets(m_outputTex, m_depthTex);
  api.clearRenderTargetViews(_color, m_outputTex);
  api.clearDepthBuffer(1.0f, m_depthTex);
  api.setInputLayout(getInputLayout());

  /**
   * Set the shaders and their resources.
   */
  SPtr<Shader> vShader = m_pVShader.lock();
  if (!api.setVShader(vShader)) { // Vertex shader.
    LOG_ERROR("Failed to set the vertex shader for pass: " + m_name, __FILE__, __LINE__);
    return;
  }
  api.vSSetShaderResourceViews(m_VTex);
  api.vSSetConstantBuffers(m_vCBuffers);

  SPtr<Shader> pShader = getPShader().lock();
  if (!api.setPShader(pShader)) { // Pixel shader.
    LOG_ERROR("Failed to set the pixel shader for pass: " + m_name, __FILE__, __LINE__);
    return;
  }
  api.pSSetShaderResourceViews(m_PTex);
  api.pSSetConstantBuffers(m_pCBuffers);

  api.setViewport(m_viewPortSize);
  api.setSampler(m_pSamplerState);
  api.setRasterizerState(m_pRasterizerState);
}

void
PixelPass::endPass()
{
  GraphicsAPI& api = g_GraphicAPI();
  api.setSampler(nullptr);
  api.setRasterizerState(nullptr);
  api.setInputLayout(nullptr);

  const SIZE_T renderTargetCount = m_outputTex.size();
  const SIZE_T vTexCount = m_VTex.size();
  const SIZE_T pTexCount = m_PTex.size();
  const SIZE_T pCBuferCount = m_pCBuffers.size();
  const SIZE_T vCBuferCount = m_vCBuffers.size();

  // unbind render targets.
  api.unbindRenderTargets(renderTargetCount);

  // vertex buffers & resources.
  api.vSUnbindShaderResourceViews(vTexCount);
  api.vSUnbindConstantBuffers(vCBuferCount);

  // pixel buffers & resources.
  api.pSUnbindShaderResourceViews(pTexCount);
  api.pSUnbindConstantBuffers(pCBuferCount);

  // unbind shaders.
  api.setVShader(nullptr);
  api.setPShader(nullptr);
}
}