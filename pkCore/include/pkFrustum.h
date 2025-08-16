/************************************************************************/
/**
* @pkFrustum pkFrustum.h
* @Hector Ivan Muñoz Ceballos
* @date 22/01/2025
* @Frustum file for the Presk Game Engine.
*
* This file will contain the Frustum class used for the engine
*
* @bug No current function.
*
* @HectIvan 22/01/20245
* File Creation.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"
#include "pkPlane.h"

namespace pkEngineSDK
{

namespace FRUSTUM_SIDES
{
  enum E
  {
    kNear = 0,
    kFar,
    kLeft,
    kRight,
    kUp,
    kDown,
  };
}

class PK_CORE_EXPORT Frustum
{
 public:
  Frustum() = default;
  virtual ~Frustum() = default,

};
}