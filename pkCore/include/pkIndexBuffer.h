/************************************************************************/
/**
* @pkIndexbuffer pkIndexbuffer.h
* @Hector Ivan Muñoz Ceballos
* @date 13/11/2024
* @Index Buffer file for the Presk Game Engine.
*
* This file will contain the Index Buffer used for the engine
*
* @bug No bug known.
*
* @HectIvan 13/11/2024
*/
/************************************************************************/
#pragma once

#define USING_API "DX"

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"

namespace pkEngineSDK
{

class Device;

class PK_CORE_EXPORT IndexBuffer
{
 public:
  IndexBuffer() = default;
  virtual ~IndexBuffer() = default;
};
}