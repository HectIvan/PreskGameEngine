/************************************************************************/
/**
* @pkSprite pkSprite.h
* @Hector Ivan Muñoz Ceballos
* @date 07/01/2025
* @Sprite file for the Presk Game Engine.
*
* This file contains the Sprite class of the engine, it will be made to work for 2D
* spaces.
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
#include "pkSimpleVertex.h"
#include "pkVector3.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT Sprite
{
public:
  Sprite();
  virtual ~Sprite() = default;

  Vector<SimpleVertex> vertexVector;
  Vector<uint32> indexVector;
};
}