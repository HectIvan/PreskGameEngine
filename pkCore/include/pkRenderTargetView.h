/************************************************************************/
/**
* @pkRenderTargetView pkRenderTargetView.h
* @Hector Ivan Muñoz Ceballos
* @date 17/12/2024
* @Render Target file for the Presk Game Engine.
*
* This file contains the Render Target class of the engine
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

class RenderTargetView
{
 public:
  RenderTargetView() = default;
  virtual ~RenderTargetView() = default;
};
}