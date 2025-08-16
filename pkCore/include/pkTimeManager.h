/************************************************************************/
/**
* @pkTimeManager pkTimeManager.h
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
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include <chrono>

#include "pkPrerequisitesCore.h"
#include "pkModule.h"

namespace pkEngineSDK
{

using std::chrono::high_resolution_clock;
using std::chrono::duration;

class PK_CORE_EXPORT TimeManager : public Module<TimeManager>
{
 public:
  TimeManager() = default;
  virtual ~TimeManager() = default;

  /**
   * @brief Get the delta time from the time provided.
   * @param _delta Current Time.
   */
  float
  getDeltaTime(high_resolution_clock::time_point& _delta);

  float m_deltaTime;
  float m_fixedDeltaTime = 0.016f;
};

PK_CORE_EXPORT TimeManager&
g_TimeManager();
}