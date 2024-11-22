/************************************************************************/
/**
* @pkSphere pkSphere.h
* @Hector Ivan Muñoz Ceballos
* @date 23/09/2024
* @Sphere file for the Presk Game Engine.
*
* This file contains the Sphere class of the engine
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

class PK_UTILITY_EXPORT Sphere
{
 public:
  Sphere() = default;
  FORCEINLINE Sphere(Vector3& _origin, float _radius) : m_origin(_origin), m_radius(_radius) {};
  ~Sphere() = default;

  Vector3 m_origin;
  float m_radius;
};
}