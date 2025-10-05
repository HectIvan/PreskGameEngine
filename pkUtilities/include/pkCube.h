/************************************************************************/
/**
* @pkCube pkCube.h
* @Hector Ivan Muñoz Ceballos
* @date 23/09/2024
* @Cube file for the Presk Game Engine.
*
* This file contains the Cube class of the engine
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

class PK_UTILITY_EXPORT Cube
{
 public:
  Cube() = default;
  PKFORCEINLINE Cube(Vector3 _origin, float _size) : m_origin(_origin), m_size(_size) {};
  ~Cube() = default;

  Vector3 m_origin;
  float m_size;
};
}
