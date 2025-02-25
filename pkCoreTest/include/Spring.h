/*****************************************************************************/
/**
 * @file    Spring.h
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    17/02/2025
 * @brief
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkActor.h"
#include "pkPrerequisitesCore.h"
#include "pkVector3.h"
#include "pkVector2.h"

using pkEngineSDK::Actor;
using pkEngineSDK::SPtr;
using pkEngineSDK::Vector2;
using pkEngineSDK::Vector3;

class Spring
{
 public:
  Spring() = default;
  virtual ~Spring() = default;

  /**
   * @brief Apply a force in a direction.
   * @param _force Force to apply.
   * @param _direction Direction to apply the force to.
   */
  void
  applyForce(float _force, Vector3 _direction);

  /**
   * Move the weight of the string
   */
  void
  move(float _deltaTime, Vector3 _direction);

  /**
  * @brief Apply gravity to the spring.
  * @param _gravity Force of the gravity.
  */
  void
  gravity(float _gravity, float _deltaTime);

  /**
   * @brief Get the distance that a spring is moved.
   * @param _force How much force is applied.
   * @return The length difference.
   */
  float
  hookeLaw(float _force);

 public:
  Vector2 m_direction;
  Vector2 m_accumulatedForce = Vector2(0);
  float m_length;
  float m_elasticity;
  float m_mass;
  float m_gravity;
  SPtr<Actor> m_weight;
  SPtr<Actor> m_anchor;
};