/*****************************************************************************/
/**
 * @file    pkScriptManager.h
 * @author   ()
 * @date    2025/01/29
 * @brief   
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include <sol/sol.hpp>
#include "pkPrerequisitesUtilities.h"
#include "pkModule.h"

using ScriptState = sol::state;

namespace pkEngineSDK
{

class PK_UTILITY_EXPORT ScriptManager : public Module<ScriptManager>
{
public:
  ScriptManager() = default;
  virtual ~ScriptManager() = default;

  /**
   * @brief
   */
  FORCEINLINE ScriptState&
  getState() { return m_state; }

  /**
   * @brief Execute the designated script.
   * @param _script The desired script.
   */
  void
  executeScript(const String& _script);

 private:
  ScriptState m_state;
};

PK_UTILITY_EXPORT ScriptManager&
g_SManager()
{
  return ScriptManager::instance();
}
}