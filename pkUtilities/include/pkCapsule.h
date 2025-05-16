/************************************************************************/
/**
* @pkCapsule pkCapsule.h
* @Hector Ivan Muñoz Ceballos
* @date 23/09/2024
* @Capsule file for the Presk Game Engine.
*
* This file contains the Capsule class of the engine
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

class PK_UTILITY_EXPORT Capsule
{
 public:
  Capsule() = default;
  Capsule(Vector3 _topOrigin, Vector3 _bottomOrigin, float _radius, float _height)
  : m_topOrigin(_topOrigin),
    m_bottomOrigin(_bottomOrigin),
    m_radius(_radius),
    m_height(_height) {};
  ~Capsule() = default;

  Vector3 m_topOrigin;
  Vector3 m_bottomOrigin;
  float m_radius;
  float m_height;
};
}