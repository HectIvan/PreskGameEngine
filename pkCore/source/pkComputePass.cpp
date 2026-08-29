/*****************************************************************************/
/**
 * @file    pkComputePass.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    29/08/2026
 * @brief   Source file for a compute pass.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#include "pkComputePass.h"
#include "pkGraphicsAPI.h"
#include "pkLogger.h"

namespace pkEngineSDK
{
ComputePass::ComputePass(const ComputePassDesc& _desc)
{
  m_name = _desc.name;

  m_pCShader = _desc.pCShader;
  
  const uint32 cBufferCount = static_cast<uint32>(_desc.cBSizes.size());
  for (uint32 i = 0; i < cBufferCount; ++i) {
    m_cCBuffers.push_back(pk_shared_ptr_new<ConstantBuffer>(_desc.cBSizes[i]));
  }

  m_uavTex = _desc.uavs;
  m_cTex = _desc.cTex;
}

void
ComputePass::clear()
{
  LOG_REGISTER(">---Clearing Compute Pass " + m_name + "---<", __FILE__, __LINE__);
  m_pCShader.reset();
  m_cCBuffers.clear();
  m_uavTex.clear();
  m_cTex.clear();
  LOG_REGISTER("<---Finished clearing Compute Pass " + m_name + "--->", __FILE__, __LINE__);
}

void
ComputePass::updateCBuffers(const Vector<const void*>& _data, const Vector<SIZE_T>& _sizes) const
{
  // assert that all data counts are the same.
  const SIZE_T blobCount = _data.size();
  PK_ASSERT(blobCount == _sizes.size() && "CBuffer blob count & sizes count do not match.");
  const uint32 CBufferCount = toUint32(m_cCBuffers.size());
  PK_ASSERT(CBufferCount == blobCount && "CBuffer update count does not match pass CBuffer count.");

  GraphicsAPI& api = g_GraphicAPI();

  for (uint32 i = 0; i < CBufferCount; ++i) {
    api.updateConstantBuffer(m_cCBuffers[i], _data[i], _sizes[i]);
  }
}

void
ComputePass::updateCBuffer(const uint32 _index, const void* _data, const SIZE_T _size) const
{
  PK_ASSERT(_index < m_cCBuffers.size() && "Compute CBuffer update index out of range.");
  g_GraphicAPI().updateConstantBuffer(m_cCBuffers[_index], _data, _size);
}

void
ComputePass::beginPass(const FColor& _color)
{
  GraphicsAPI& api = g_GraphicAPI();

  api.clearUnorderedAccessViews(m_uavTex, _color);

  SPtr<Shader> cShader = m_pCShader.lock();
  if (!api.setCShader(cShader)) { // Compute shader.
    LOG_ERROR("Failed to set the compute shader for pass: " + m_name, __FILE__, __LINE__);
  }
  api.cSSetShaderResourceViews(m_cTex);
  api.cSSetUnorderedAccessViews(m_uavTex);
  api.cSSetConstantBuffers(m_cCBuffers);

  api.setViewport(m_viewPortSize);
  api.setSampler(m_pSamplerState);
  api.setRasterizerState(m_pRasterizerState);
}

void
ComputePass::endPass()
{
  GraphicsAPI& api = g_GraphicAPI();

  const SIZE_T uavCount = m_uavTex.size();
  const SIZE_T resourceCount = m_cTex.size();
  const SIZE_T cBufferCount = m_cCBuffers.size();

  api.cSUnbindShaderResourceViews(resourceCount);
  api.cSUnbindUnorderedAccessViews(uavCount);
  api.cSUnbindConstantBuffers(cBufferCount);
  api.setCShader(nullptr);
}
}