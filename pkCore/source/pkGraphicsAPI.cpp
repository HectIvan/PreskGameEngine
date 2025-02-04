#include "pkGraphicsAPI.h"

/**
* Windows version
**/

#if PK_PLATFORM == PK_PLATFORM_WIN32

namespace pkEngineSDK
{

PK_CORE_EXPORT GraphicsAPI&
g_GraphicAPI()
{
  return GraphicsAPI::instance();
}
}

#endif