#include "pkDllLoader.h"

#if PK_PLATFORM == PK_PLATFORM_WIN32
#include <Windows.h>

namespace pkEngineSDK
{

void
DllLoader::init(const String& _name, const bool& _load)
{
  handler = nullptr;
  name = _name + ".dll";
  if (_load) { load(); }
}

void
DllLoader::load()
{
  handler = static_cast<InstanceHandle>(LoadLibraryA(name.c_str()));

  if (!handler)
  {
    uint32 var;
    var = 0;
    return;
    // could not load the dll
  }
}

void*
DllLoader::getMethod(const String& _methodName)
{
  return static_cast<void*>(GetProcAddress(handler, _methodName.c_str()));
}
}

#endif