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

namespace pkEngineSDK
{

class RigidBody
{
 public:
  RigidBody() = default;
  virtual ~RigidBody() = default;

  /**
   * @brief Get the rigid body world position.
   * @return The world position.
   */
  FORCEINLINE Vector3
  getWorldPosition() { return m_position; }

  /**
   * @brief Set the rigid body world position.
   * @param _position New position.
   */
  FORCEINLINE void
  setWorldPosition(Vector3 _position) { m_position = _position; }

  /**
   * @brief Get the inverse inertia world.
   */
  Matrix3
  getInvInertiaWorld();

  /**
   * @brief Get the friction of the body on a direction and force.
   * @param _dir direction of the force.
   * @param _magnitude Magnitude of the force
   * @return Final friction;
   */
  float
  getFriction(Vector3 _dir, float _magnitude);

  /**
   * @brief Get the friction of the body with a vector.
   * @param _vector Vector to use.
   * @return Final friction.
   */
  float
  getFriction(Vector3 _vector);

  /**
   * @brief Calculate the friction between 2 RigidBodies.
   * @param _rb Other RigidBody.
   * @param _dir Direction of the object.
   * @param _magnitude Magnitude of the force.
   * @return Final friction
   */
  float 
  getFriction(RigidBody& _rb, Vector3 _dir, float _magnitude);

  /**
   * @brief Get the body elasticity.
   * @return Elasticity.
   */
  FORCEINLINE float
  getElasticity() { return m_elasticity; }

  /**
   * @brief Get the elasticity between 2 RigidBodies.
   * @param _rb Other RigidBody
   * @return Final elasticity.
   */
  float
  getElasticity(RigidBody& _rb);

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
   * @brief Apply physics based rotation to the rigid body
   * @brief _orientation Body rotation
   */
  void
  integrateRotation(const Vector3& angularAcceleration,
                    float deltaTime);

 public:
  float m_drag;
  float m_gravity;
  float m_mass;
  float m_inverseMass;
  float m_frictionCoeff;
  float m_elasticity;
  Vector3 m_linearVelocity;
  Vector3 m_angularVelocity;
  Vector3 m_invAngularInertia;
  Vector3 m_prevPos;
  Vector3 m_position;
  Quaternion m_orientation;
  Matrix4 m_transform;
};
}
