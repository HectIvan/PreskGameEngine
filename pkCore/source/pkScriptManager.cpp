/*****************************************************************************/
/**
 * @file    pkScriptManager.cpp
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    11/02/2025
 * @brief   cpp file for reading lua scripts
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
#include "pkScriptManager.h"

namespace pkEngineSDK
{

ScriptManager::ScriptManager()
{
  m_state.open_libraries(
    lib::base
  );
}

PK_CORE_EXPORT ScriptManager&
g_ScriptManager()
{
  return ScriptManager::instance();
}
}