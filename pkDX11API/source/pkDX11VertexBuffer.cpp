/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11VertexBuffer.h"
#include "pkDX11Device.h"
#include "pkSimpleVertex.h"

namespace pkEngineSDK
{

void
DX11VertexBuffer::set(Device* _pDevice,
                      uint32 _start,
                      uint32 _bufferCount,
                      uint32 _offset)
{
  // convert from parent to child
  if (DX11Device* deviceX = dynamic_cast<DX11Device*>(_pDevice->getDevice().get()))
  {
    
  }
}

void
DX11VertexBuffer::clean()
{
  safeRelease(m_pBuffer);
}
}