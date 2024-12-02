/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11VertexBuffer.h"
#include "pkDX11Device.h"

namespace pkEngineSDK
{

SPtr<VertexBuffer>
DX11VertexBuffer::create(Device* _pDevice,
                         const Vector<SimpleVertex>& _vertex,
                         uint32 _usage)
{
  auto spVB = std::make_shared<DX11VertexBuffer>();

  // --------------------------------------------------------------//
  //          Define and create the buffer
  // --------------------------------------------------------------//
  D3D11_BUFFER_DESC bd;
  memset(&bd, 0, sizeof(bd));
  bd.ByteWidth = static_cast<uint32>(sizeof(SimpleVertex) * _vertex.size()); // size of the buffer
  bd.Usage = static_cast<D3D11_USAGE>(_usage); // how it is expected to be read
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // how it will be binded to the pipeline
  bd.CPUAccessFlags = 0; // default -> CPU ha no accesss to this
  bd.MiscFlags = 0;
  // bd.StructureByteStride = sizeof(SimpleVertex); // size of each element

  D3D11_SUBRESOURCE_DATA InitData; // info descriptor
  memset(&InitData, 0, sizeof(InitData));
  InitData.pSysMem = _vertex.data(); // pointer to the initialization data
  InitData.SysMemPitch = static_cast<uint32>(_vertex.size() * sizeof(SimpleVertex)); // distance between values

  // convert from parent to child
  if (DX11Device* deviceX = dynamic_cast<DX11Device*>(_pDevice->getDevice().get()))
  {
    deviceX->m_pd3dDevice->CreateBuffer(&bd, &InitData, &spVB->m_pBuffer);
  }
  return spVB;
}

void
DX11VertexBuffer::set(Device* _pDevice,
                      uint32 _start,
                      uint32 _bufferCount,
                      uint32 _offset)
{
  // convert from parent to child
  if (DX11Device* deviceX = dynamic_cast<DX11Device*>(_pDevice->getDevice().get()))
  {
    uint32 stride = sizeof(SimpleVertex);
    deviceX->m_pImmediateContext->IASetVertexBuffers(_start,
                                                     _bufferCount,
                                                     &m_pBuffer,
                                                     &stride,
                                                     &_offset);
  }
}

void
DX11VertexBuffer::clean()
{
  safeRelease(m_pBuffer);
}
}