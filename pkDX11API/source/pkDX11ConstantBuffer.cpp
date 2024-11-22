/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11ConstantBuffer.h"
#include "pkDX11Device.h"

namespace pkEngineSDK {

DX11ConstantBuffer::DX11ConstantBuffer()
{
}

DX11ConstantBuffer::~DX11ConstantBuffer()
{
}

HRESULT
DX11ConstantBuffer::Create(DX11Device* device, uint32 size, const void* pData, uint32 usage)
{
  HRESULT hr;
  D3D11_BUFFER_DESC bDesc;
  bDesc.Usage = static_cast<D3D11_USAGE>(usage);
  bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  bDesc.MiscFlags = 0;
  bDesc.ByteWidth = size;
  bDesc.CPUAccessFlags = usage == D3D10_USAGE_DYNAMIC ?
    D3D11_CPU_ACCESS_WRITE : 0;

  D3D11_SUBRESOURCE_DATA subData;
  if (pData)
  {
    subData.pSysMem = pData;
    subData.SysMemPitch = size;
    subData.SysMemSlicePitch = 0;
  }

  hr = device->m_pd3dDevice->CreateBuffer(&bDesc, pData ? &subData : nullptr, &m_pCBuffer);
  if (FAILED(hr))
  {
    return hr;
  }
  return hr;
}

void
DX11ConstantBuffer::UpdateSubResource(DX11Device* device, const void* newData, uint32 size)
{
  device->m_pImmediateContext->UpdateSubresource(m_pCBuffer, 0, nullptr, newData, size, 0);
}

void
DX11ConstantBuffer::Clean()
{
  if (m_pCBuffer) m_pCBuffer->Release();
}
}