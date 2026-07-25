/*****************************************************************************/
/**
 * @file    pkDllManager.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    23/10/2025
 * @brief   Dll manager class for the engine.
 *
 * @bug    no known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDllLoader.h"
#include "pkDllManager.h"

namespace pkEngineSDK
{
bool
DLLManager::runDll(const String _name)
{
  DllLoader dll;
  dll.init(_name);
  auto dllSymbol = reinterpret_cast<void(*)()>(dll.getMethod("loadPlugin"));
  if (dllSymbol) {
    dllSymbol();
    return true;
  }
  return false;
}

PK_CORE_EXPORT DLLManager&
g_DLLManager()
{
  return DLLManager::instance();
}
}
