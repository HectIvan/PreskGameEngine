/************************************************************************/
/**
* @pkWindowDesc pkWindowDesc.h
* @Hector Ivan Muñoz Ceballos
* @date 30/09/2024
* @Window descriptor file for the Presk Game Engine.
*
* This file will contain the window descriptor used for the engine
*
* @bug No bug known.
*
* @HectIvan 30/09/2024
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"

namespace pkEngineSDK {

struct PK_CORE_EXPORT PKWindowDesc
{
  uint32 width;
  uint32 height;

  uint32 posX;
  uint32 posY;
};
}