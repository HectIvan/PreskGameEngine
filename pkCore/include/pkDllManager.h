/*****************************************************************************/
/**
 * @file    pkDllManager.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    23/10/2025
 * @brief   Dll manager class for the engine.
 *
 * @bug    no known bugs.
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
class PK_CORE_EXPORT DLLManager : public Module<DLLManager>
{
 public:
  DLLManager() = default;
  virtual ~DLLManager() = default;

  /**
   * @brief run a dll by name.
   */
  bool
  runDll(const String _name);
};

PK_CORE_EXPORT DLLManager&
g_DLLManager();
}
