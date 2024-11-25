/************************************************************************/
/**
* @pkDevice pkDevice.h
* @Hector Ivan Muñoz Ceballos
* @date 22/11/2024
* @Device file for the Presk Game Engine.
*
* Device base class for the engine
*
* @bug No bug known.
*
* @HectIvan 25/11/2024
* Created the file
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT Device
{
 public:
  Device() = default;
  virtual ~Device() = default;
};
}