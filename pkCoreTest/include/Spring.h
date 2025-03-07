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
   * @brief Move the weight of the string.
   * @param _deltaTime Time between frames.
   */
  void
  move(float _deltaTime);

  /**
   * @brief Apply Force in a direction.
   * @param _dir Direction to apply the force to.
   * @param _force How much force to apply.
   * @param _deltaTime time between frames.
   */
  void
  applyForce(Vector3 _dir, float _force, float _deltaTime);

 public:
  Vector2 m_direction;
  float m_maxDistance;
  float m_minDistance;
  float m_length;
  float m_elasticity;
  float m_mass;
  float m_gravity;
  SPtr<Actor> m_weight;
  SPtr<Actor> m_anchor;
};