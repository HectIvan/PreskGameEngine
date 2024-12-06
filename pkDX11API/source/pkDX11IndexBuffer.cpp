/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11IndexBuffer.h"
#include "pkDX11Device.h"

namespace pkEngineSDK {

SPtr<IndexBuffer>
DX11IndexBuffer::create(Device* _pDevice,
                        const Vector<uint32>& _index,
                        uint32 _usage)
{
  auto spIB = std::make_shared<DX11IndexBuffer>();

  /***************************************************************/
  /**
  * Define and create the buffer
  **/
  /***************************************************************/
  D3D11_BUFFER_DESC bd;
  memset(&bd, 0, sizeof(bd));
  bd.ByteWidth = sizeof(uint32) * (uint32)_index.size(); // size of the buffer
  bd.Usage = static_cast<D3D11_USAGE>(_usage); // how it is expected to be read
  bd.BindFlags = D3D11_BIND_INDEX_BUFFER; // how it will be binded to the pipeline
  bd.CPUAccessFlags = 0; // default -> CPU ha no accesss to this
  bd.MiscFlags = 0;
  // bd.StructureByteStride = sizeof(uint32); // size of each element

  D3D11_SUBRESOURCE_DATA InitData; // info descriptor
  memset(&InitData, 0, sizeof(InitData));
  InitData.pSysMem = _index.data(); // pointer to the initialization data
  // InitData.SysMemPitch = (uint32)index.size() * sizeof(uint32); // distance between values
  // convert from parent to child
  if (DX11Device* deviceX = dynamic_cast<DX11Device*>(_pDevice->getDevice().get()))
  {
    deviceX->m_pd3dDevice->CreateBuffer(&bd, &InitData, &spIB->m_pBuffer);
  }
  return spIB;
}

void
DX11IndexBuffer::set(Device* _pDevice,
                     uint32 _format,
                     uint32 _offset)
{
  if (DX11Device* deviceX = dynamic_cast<DX11Device*>(_pDevice->getDevice().get()))
  {
    deviceX->m_pImmediateContext->IASetIndexBuffer(m_pBuffer,
                                                   static_cast<DXGI_FORMAT>(_format),
                                                   _offset);
  }
}

void
DX11IndexBuffer::clean()
{
  safeRelease(m_pBuffer);
}
}