/************************************************************************/
/**
* @simpleVertex pkSimpleVertex.h
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
#include "pkPrerequisitesCore.h"
#include "pkVector2.h"
#include "pkVector3.h"

namespace pkEngineSDK
{

struct SimpleVertex
{
  SimpleVertex() = default;
  SimpleVertex(Vector3 _pos) :
    pos(_pos)
  {};
  Vector3 pos;
  Vector3 normal;
  Vector2 Tex;
  Vector3 tangent;
  Vector3 bitangent;

  int32 boneIDs[MAX_BONE_INFLUENCE];
  float weights[MAX_BONE_INFLUENCE];
};
}