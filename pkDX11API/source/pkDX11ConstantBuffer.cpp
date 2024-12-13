/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11ConstantBuffer.h"
#include "pkDX11Device.h"

namespace pkEngineSDK
{

HRESULT
DX11ConstantBuffer::create(DX11Device* _pDevice, uint32 _size, const void* _pData, uint32 _usage)
{
  HRESULT hr;
  D3D11_BUFFER_DESC bDesc;
  bDesc.Usage = static_cast<D3D11_USAGE>(_usage);
  bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  bDesc.MiscFlags = 0;
  bDesc.ByteWidth = _size;
  bDesc.CPUAccessFlags = _usage == D3D10_USAGE_DYNAMIC ?
    D3D11_CPU_ACCESS_WRITE : 0;

  D3D11_SUBRESOURCE_DATA subData;
  if (_pData)
  {
    subData.pSysMem = _pData;
    subData.SysMemPitch = _size;
    subData.SysMemSlicePitch = 0;
  }

  hr = _pDevice->pd3dDevice->CreateBuffer(&bDesc, _pData ? &subData : nullptr, &pCBuffer);
  if (FAILED(hr))
  {
    return hr;
  }
  return hr;
}

void
DX11ConstantBuffer::updateSubResource(DX11Device* _pDevice, const void* _pNewData, uint32 _size)
{
  _pDevice->pImmediateContext->UpdateSubresource(pCBuffer, 0, nullptr, _pNewData, _size, 0);
}
}