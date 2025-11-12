/************************************************************************/
/**
* @pkTimeManager pkTimeManager.cpp
* @Hector Ivan Muñoz Ceballos
* @date 04/02/2025
* @Time Manager file for the Presk Game Engine.
*
* This file will contain the Time Manager used for the engine
*
* @bug No current function.
*
* @HectIvan 04/02/20245
* File Creation.
*/
/************************************************************************/
#include "pkTimeManager.h"

namespace pkEngineSDK
{

using std::localtime;
using std::put_time;
using std::stringstream;
using std::time_t;
using std::tm;

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

String
TimeManager::getCurrentTime()
{
  auto now = system_clock::now();

  time_t tt = system_clock::to_time_t(now);
  tm tm = *localtime(&tt);
  stringstream ss;
  ss << put_time(&tm, "%Y-%m-%d %H:%M:%S");

  return ss.str();
}

PK_CORE_EXPORT
TimeManager& g_TimeManager()
{
  return TimeManager::instance();
}
}