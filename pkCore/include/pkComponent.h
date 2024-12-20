/************************************************************************/
/**
* @pkComponent pkComponent.h
* @Hector Ivan Muñoz Ceballos
* @date 00/12/2024
* @Component for the Presk Game Engine.
*
* This file contains the Component for the engine
*
* @bug No bug known.
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

class PK_CORE_EXPORT Component
{
public:
  Component() = default;
  virtual ~Component() = default;
};
}
