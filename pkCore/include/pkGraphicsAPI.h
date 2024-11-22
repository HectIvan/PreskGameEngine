/************************************************************************/
/**
* @pkGraphicsAPI pkGraphicsAPI.h
* @Hector Ivan Muñoz Ceballos
* @date 19/11/2024
* @Graphics api file for the Presk Game Engine.
*
* Graphics API class for the engine
*
* @bug No bug known.
*
* @HectIvan 19/11/2024
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
#include "pkWindow.h"

namespace pkEngineSDK
{

class GraphicsAPI
{
 public:
  GraphicsAPI() = default;
  virtual ~GraphicsAPI() = default;

  virtual void
  init(WindowHandle& _wHnd) = 0;
};
}