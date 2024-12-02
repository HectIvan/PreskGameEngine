/************************************************************************/
/**
* @simpleVertex simpleVertex.h
* @Hector Ivan Muñoz Ceballos
* @date 21/10/2024
* @Simple vertex file for the Presk Game Engine.
*
* This file contains the Simple Vertex struct of the engine
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
#include "pkVector3.h"
#include "pkVector2.h"

namespace pkEngineSDK {

struct SimpleVertex
{
  Vector3 pos;
  Vector3 normal;
  Vector2 Tex;
};
}