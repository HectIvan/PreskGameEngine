/*************************************************************************/
/**
 * @file    pkRigidBody.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    05/06/2025
 * @brief   Rigid body for the physics library.
 *
 * @bug    no known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPhysicsMaterial.h"
#include "pkMatrix3.h"
#include "pkQuaternion.h"
#include "pkComponent.h"

namespace pkEngineSDK
{

class RigidBody : public Component
{
 public:
  RigidBody() {
    m_gravity = 1.0f;
    m_drag = 0.0f;
    // physics material
    m_physMat = make_shared<PhysicsMaterial>();
  }
  virtual ~RigidBody() = default;

  COMPONENT_TYPE::E
  getType() override { return COMPONENT_TYPE::kRigidBody; }

  static COMPONENT_TYPE::E
  getObjType() { return COMPONENT_TYPE::kRigidBody; }

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

  /**
   * @brief Get the rigid body elasticity.
   * @return The body elasticity.
   */
  float
  getElasticity() const;

  /**
   * @brief Set the rigid body elasticity.
   * @param _elasticity The new body elasticity.
   */
  void
  setElasticity(const float _elasticity);

  /**
   * @brief Get the friction coefficient.
   * @return The body friction.
   */
  float
  getFrictionCoefficient() const;

  /**
   * @brief Set the friction coefficient.
   * @param _friction The new friction of the object.
   */
  void
  setFrictionCoef(const float _friction);

 public:
  float m_gravity;
  float m_drag;
  float m_inverseMass;
  Vector3 m_linearVelocity;
  Vector3 m_angularVelocity;
  Matrix3 m_invAngularInertia;
  Vector3 m_prevPos;
  Vector3 m_position;
  Quaternion m_orientation;
  Matrix4 m_transform;

  SPtr<PhysicsMaterial> m_physMat;
};
}
