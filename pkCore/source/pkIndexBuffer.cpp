#if PK_PLATFORM == PK_PLATFORM_WIN32

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkIndexBuffer.h"

namespace pkEngineSDK
{

template <class Device>
SPtr<Device>
IndexBuffer::create(Device* _pDevice,
                    const Vector<uint32>& _index,
                    uint32 _usage)
{
  
}
}
#endif