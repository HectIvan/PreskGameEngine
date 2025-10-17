/*****************************************************************************/
/**
 * @file    pkScriptManager.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    11/02/2025
 * @brief   Class for reading lua scripts
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
#include "pkPrerequisitesCore.h"
#include "pkModule.h"

namespace pkEngineSDK
{

// using ScriptState = sol::state;
// using sol::lib;

class PK_CORE_EXPORT ScriptManager : public Module<ScriptManager>
{
 public:
  ScriptManager();
  virtual ~ScriptManager() = default;

 public:
  // ScriptState m_state;
};

PK_CORE_EXPORT ScriptManager&
g_ScriptManager();
}