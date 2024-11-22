/************************************************************************/
/**
* @pkPlane pkPlane.h
* @Hector Ivan Muñoz Ceballos
* @date 23/09/2024
* @Plane file for the Presk Game Engine.
*
* This file contains the Plane class of the engine
*
* @bug No bug known.
*
* @HectIvan 23/09/2024
* Added comments and class
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesUtilities.h"
#include "pkVector3.h"

namespace pkEngineSDK {

class PK_UTILITY_EXPORT Plane
{
 public:
  Plane() = default;
  ~Plane() = default;

  Vector3 m_normal;
  float m_offset;
};
}