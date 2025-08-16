/*****************************************************************************/
/**
 * @file    pkInputLayout.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    17/12/2024
 * @brief   Input Layout class of the engine
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

namespace pkEngineSDK
{

class Device;

class InputLayout
{
 public:
  InputLayout() = default;
  virtual ~InputLayout() = default;
};
}
