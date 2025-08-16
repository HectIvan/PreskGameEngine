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
  FORCEINLINE CollisionInfo(Vector3 _normalHit,
                            Vector3 _contact1,
                            Vector3 _contact2,
                            float _penDist) :
    m_normalHit(_normalHit),
    m_contactPoint1(_contact1),
    m_contactPoint2(_contact2),
    m_penDistance(_penDist) {};
  virtual ~CollisionInfo() = default;

  Vector3 m_normalHit;
  Vector3 m_contactPoint1;
  Vector3 m_contactPoint2;
  float m_penDistance;
};
}