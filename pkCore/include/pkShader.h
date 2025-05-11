/************************************************************************/
/**
* @pkShader pkShader.h
* @Hector Ivan Muñoz Ceballos
* @date 17/12/2024
* @Shader file for the Presk Game Engine.
*
* This file contains the base Shader class of the engine
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

class PK_CORE_EXPORT Shader
{
 public:
  Shader() {};
  virtual ~Shader() = default;
};
}