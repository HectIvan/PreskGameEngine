/************************************************************************/
/**
* @pkEvent pkEvent.h
* @Hector Ivan Muñoz Ceballos
* @date 21/10/2024
* @Event file for the Presk Game Engine.
*
* This file contains the Event class of the engine
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

namespace TYPE
{
  enum E
  {
    kKey = 0,
    kMouse
  };
}

class Event
{
 public:
  Event() = default;
  virtual ~Event() = default;
};
}