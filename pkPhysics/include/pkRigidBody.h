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
#include "pkMatrix3.h"
#include "pkQuaternion.h"
#include "pkComponent.h"

namespace pkEngineSDK
{

class RigidBody : public Component
{
 public:
  RigidBody() = default;
  virtual ~RigidBody() = default;

  COMPONENT_TYPE::E
  getType() override { return COMPONENT_TYPE::kRigidBody; }

  const char*
  getName() override { return "RigidBody"; }

  /**
   * @brief Get the rigid body world position.
   * @return The world position.
   */
  FORCEINLINE Vector3
  getWorldPosition() { return m_position; }

  /**
   * @brief Get the inverse inertia world.
   */
  Matrix3
  getInvInertiaWorld();

  /**
   * @brief Apply impulse to the rigid body
   * @param _impulse Ammount of impulse to apply.
   * @param _point Point where the impulse will be applied.
   */
  void
  applyImpulse(const Vector3& _impulse, const Vector3& _point);

  /**
   * @brief Apply impulse to the rigid body
   * @param _impulse Ammount of impulse to apply.
   * @param _point Point where the impulse will be applied.
   */
  void
  applyPositionalImpulse(const Vector3& _impulse, const Vector3& _point);

 public:
  float m_gravity;
  float m_drag;
  float m_inverseMass;
  Vector3 m_linearVelocity;
  Vector3 m_angularVelocity;
  Vector3 m_invAngularInertia;
  Vector3 m_prevPos;
  Vector3 m_position;
  Quaternion m_orientation;
  Matrix4 m_transform;
};
}
