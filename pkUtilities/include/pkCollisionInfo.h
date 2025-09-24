/*****************************************************************************/
/**
 * @file    pkCollisionInfo.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    23/05/2025
 * @brief   Collision info for physics simulations.
 *
 * @bug     no known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesUtilities.h"
#include "pkVector3.h"

namespace pkEngineSDK
{

class PK_UTILITY_EXPORT CollisionInfo
{
 public:
  CollisionInfo() = default;
  FORCEINLINE CollisionInfo(Vector3 _normalHit, float _penDist) :
    m_normalHit(_normalHit),
    m_penDistance(_penDist) {};
  virtual ~CollisionInfo() = default;

 public:
  Vector3 m_normalHit = Vector3::ZERO;
  float m_penDistance = 0.0f;
};
}