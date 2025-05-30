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
  Capsule(Vector3 _center, Vector3 _direction, float _height, float _radius)
  : m_center(_center),
    m_direction(_direction),
    m_radius(_radius),
    m_height(_height) {};
  ~Capsule() = default;

  float
  getHeight() { return (getA() - getB()).magnitude(); }
  
  Vector3
  getA() { return m_direction.normalized() * (0.5f * m_height) + m_center; }

  Vector3
  getB() { return m_direction.normalized() * -1.0f * (0.5f * m_height) + m_center; }

 public:
  Vector3 m_center;
  Vector3 m_direction;
  float m_height;
  float m_radius;
};
}