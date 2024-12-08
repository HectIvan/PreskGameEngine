/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11IndexBuffer.h"
#include "pkDX11Device.h"

namespace pkEngineSDK
{

void
DX11IndexBuffer::clean()
{
  safeRelease(m_pBuffer);
}
}