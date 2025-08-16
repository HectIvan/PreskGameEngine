/*************************************************************************/
/**
 * @file    pkPhysicsMaterial.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    25/07/2025
 * @brief   Physics Material for physics simulation.
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
#include "pkPrerequisitesCore.h"
#include "pkPhysicsDefines.h"

namespace pkEngineSDK
{

class PK_PHYSICS_EXPORT PhysicsMaterial
{
 public:
  PhysicsMaterial(float _elasticity = 0.5f, float _friction = 0.5f) :
    m_elasticity(_elasticity), m_friction(_friction)
  {};
  virtual ~PhysicsMaterial() = default;

  /**
   * @brief Set the material elasticity.
   * @param _elasticity New elasticity.
   */
  void
  setElasticity(const float _elasticity) { m_elasticity = _elasticity; }

  /**
   * @brief Get the material elasticity.
   * @return The material elasticity.
   */
  float
  getElasticity() const { return m_elasticity; }

  /**
   * @brief Set the material friction.
   * @param _friction New friction.
   */
  void
  setFriction(const float _friction) { m_friction = _friction; }

  /**
   * @brief Get the material friction.
   * @return The material friction.
   */
  float
  getFriction() const { return m_friction; }

 private:
  float m_elasticity;
  float m_friction;
};
}