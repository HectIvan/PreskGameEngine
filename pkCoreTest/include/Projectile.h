/*****************************************************************************/
/**
 * @file    Projectile.h
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    10/01/2025
 * @brief   Projectile for physics gravity and collision testing.
 *
 * @bug    Projectile collision with obstacles is scuffed
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkActor.h"
#include "pkVector2.h"
#include "pkVector3.h"
#include "pkGameObject.h"
#include "pkPrerequisitesCore.h"

using pkEngineSDK::Actor;
using pkEngineSDK::GameObject;
using pkEngineSDK::SPtr;
using pkEngineSDK::Vector2;
using pkEngineSDK::Vector3;

class Projectile
{
 public:
   Projectile() = default;
  virtual ~Projectile() = default;

  /**
   * @brief Start the projectile.
   */
  void
  start();

  /**
   * @brief Move the Projectile.
   * @param _deltaTime Time between the previous frame and the current one.
   * @param _direction Direction in which the Projectile will move.
   */
  void
  move(float _deltaTime, Vector3 _direction);

  /**
   * @brief Move the projectile using the verlet integration.
   * @param _dir Direction to move the projectile at.
   * @param _force Force to use to move it.
   */
  void
  moveVerlet(Vector3 _dir, float _force);

  /**
   * @brief Fire the projectile.
   * @param _startPos Start point of the projectile.
   * @param _direction Direction of the projectile.
   */
  void
  fire(Vector3 _startPos, Vector2 _direction);

  void
  projTimer(float _deltaTime);

  /**
   * @brief bounce the projectile on the walls of the screen.
   * @param _width Width of the screen.
   * @param _height Height of the window.
   */
  void
  screenBounce(float _width, float _height);

  /**
   * @brief reset te position to the correct location.
   * @param _P2 Position of the obstacle.
   * @param _R2 Radius of the obstacle.
   */
  void
  obstacleBounce(Vector3 _P2, float _R2);

  /**
   * @brief Apply gravity to the Projectile direction.
   * @param _deltaTime Delta Time of the frames.
   */
  void
  gravity(float _deltaTime);

 public:
  float m_radius;
  float m_gravity;
  float m_speed;
  float m_maxSpeed;
  float m_bounceFactor;
  Vector3 m_position;
  Vector2 m_direction;
  SPtr<Actor> m_actor;
  bool m_fired;
  float m_lifeTime = 0.0f;
  float m_lifeTimer;
};