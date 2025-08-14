#include "pkBaseManager.h"

namespace pkEngineSDK
{

BaseManager&
g_BaseManager()
{
  return BaseManager::instance();
}
}
