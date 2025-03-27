/************************************************************************/
/**
* @pkLight pkLight.h
* @Hector Ivan Muñoz Ceballos
* @date 22/11/2024
* @Light file for the Presk Game Engine.
*
* Light struct for the engine
*
* @bug No bug known.
*
* @HectIvan 22/11/2024
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
#include "pkVector3.h"

namespace pkEngineSDK
{

namespace LIGHT_TYPE
{
  enum E : uint32
  {
    kDirectional = 0,
    kPoint,
    kSpot
  };
}

struct Light
{
  uint32 Type;
  float SpotCutoff;
  float SpotExponent;
  Vector3 LightDir = Vector3::FORWARD;
  Vector3 LightPos;
  Vector3 LightColor = Vector3(1.0f);
  Vector2 unused1 = Vector2(0.0f);
  Vector2 unused2 = Vector2(0.0f);
};
}