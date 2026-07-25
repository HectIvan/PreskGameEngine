/*****************************************************************************/
/**
 * @file    pkDllLoader.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    06/12/2024
 * @brief   Dll loader class for the engine.
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

/*********************************************/
/**
* #if's
**/
/*********************************************/

namespace pkEngineSDK
{

class PK_CORE_EXPORT DllLoader
{
 public:
  DllLoader() = default;
  virtual ~DllLoader() = default;

  /**
   * @brief Initialize the library loader.
   * @param _name Name of the file to find.
   * @param _load Wether the file will be immediately loaded.
   */
  void
  init(const String& _name, const bool& _load = true);

  /**
   * load the library
   */
  void
  load();

  /**
   * @brief Get the desired method to look for in the library.
   * @param _methodName Name of the method.
   * @return Direction of the process.
   */
  void*
  getMethod(const String& _methodName);

 public:
  InstanceHandle handler;
  String name;
};
}