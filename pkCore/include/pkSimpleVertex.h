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
  SimpleVertex(Vector3 _pos = Vector3(0.0f),
               Vector3 _nor = Vector3(0.0f),
               Vector2 _tex = Vector2(0.0f),
               Vector3 _tan = Vector3(0.0f),
               Vector3 _bitan = Vector3(0.0f)) :
               pos(_pos),
               normal(_nor),
               tex(_tex),
               tangent(_tan),
               bitangent(_bitan) {};
  Vector3 pos;
  Vector3 normal;
  Vector2 tex;
  Vector3 tangent;
  Vector3 bitangent;

  int32 boneIDs[MAX_BONE_INFLUENCE];
  float weights[MAX_BONE_INFLUENCE];
};
}