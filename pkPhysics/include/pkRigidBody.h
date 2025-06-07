/*************************************************************************/
/**
 * @file    pkRigidBody.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    05/06/2025
 * @brief   Rigid body for the physics library.
 *
 * @bug    no knon bugs.
 */
 /*****************************************************************************/
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

class RigidBody
{
 public:
  RigidBody() = default;
  virtual ~RigidBody() = default;

 public:
  float m_drag;
  float m_gravity;
  Vector3 m_prevPos;
};
}
