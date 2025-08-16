#include "pkTimeManager.h"

namespace pkEngineSDK
{

float
TimeManager::getDeltaTime(high_resolution_clock::time_point& _delta)
{
  // get the current time
  high_resolution_clock::time_point end = high_resolution_clock::now();
  // subtract the previous time to the current time to get the difference (delta time)
  float deltaTime = duration<float>(end - _delta).count();
  // previous time is now the current time
  _delta = high_resolution_clock::now();
  // return the difference
  return deltaTime;
}

PK_CORE_EXPORT
TimeManager& g_TimeManager()
{
  return TimeManager::instance();
}
}