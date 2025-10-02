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

high_resolution_clock::time_point
TimeManager::startTimer()
{
  m_start = high_resolution_clock::now();
  return m_start;
}

high_resolution_clock::time_point
TimeManager::endTimer()
{
  m_end = high_resolution_clock::now();
  return m_end;
}

float
TimeManager::getTimerElapsed()
{
  duration<float> time = m_end - m_start;
  float elapsed = time.count();
  return elapsed;
}

PK_CORE_EXPORT
TimeManager& g_TimeManager()
{
  return TimeManager::instance();
}
}