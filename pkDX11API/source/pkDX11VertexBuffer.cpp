/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11VertexBuffer.h"
#include "pkDX11Device.h"

namespace pkEngineSDK
{

SPtr<DX11VertexBuffer>
DX11VertexBuffer::Create(DX11Device* pDevice,
                         const Vector<SimpleVertex>& vertex,
                         uint32 usage)
{
  auto spVB = std::make_shared<DX11VertexBuffer>();

  // --------------------------------------------------------------//
  //          Define and create the buffer
  // --------------------------------------------------------------//
  D3D11_BUFFER_DESC bd;
  memset(&bd, 0, sizeof(bd));
  bd.ByteWidth = static_cast<uint32>(sizeof(SimpleVertex) * vertex.size()); // size of the buffer
  bd.Usage = static_cast<D3D11_USAGE>(usage); // how it is expected to be read
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // how it will be binded to the pipeline
  bd.CPUAccessFlags = 0; // default -> CPU ha no accesss to this
  bd.MiscFlags = 0;
  // bd.StructureByteStride = sizeof(SimpleVertex); // size of each element

  D3D11_SUBRESOURCE_DATA InitData; // info descriptor
  memset(&InitData, 0, sizeof(InitData));
  InitData.pSysMem = vertex.data(); // pointer to the initialization data
  InitData.SysMemPitch = static_cast<uint32>(vertex.size() * sizeof(SimpleVertex)); // distance between values

  pDevice->m_pd3dDevice->CreateBuffer(&bd, &InitData, &spVB->m_pBuffer);

  return spVB;
}

void
DX11VertexBuffer::Set(DX11Device* pDevice,
                      uint32 start,
                      uint32 bufferCount,
                      uint32 offset)
{
  // set the buffer
  uint32 stride = sizeof(SimpleVertex);
  pDevice->m_pImmediateContext->IASetVertexBuffers(start, bufferCount, &m_pBuffer, &stride, &offset);
}

void
DX11VertexBuffer::Clean()
{
  safeRelease(m_pBuffer);
}
}