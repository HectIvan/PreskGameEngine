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
void
DLLManager::runDll(const String _name)
{
  DllLoader dll;
  dll.init(_name);
  auto dllSymbol = reinterpret_cast<void(*)()>(dll.getMethod("loadPlugin"));
  if (dllSymbol) {
    dllSymbol();
  }
}

PK_CORE_EXPORT DLLManager&
g_DLLManager()
{
  return DLLManager::instance();
}
}
