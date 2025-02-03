#pragma once
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

class Player
{
 public:
  Player() = default;
  virtual ~Player() = default;

  /**
   * @brief Start the projectile.
   */
  void
  start();

  /**
   * @brief Move the player.
   * @param _deltaTime Time between the previous frame and the current one.
   * @param _direction Direction in which the player will move.
   */
  void
  move(float _deltaTime, Vector3 _direction);

  /**
   * @brief Fire the projectile.
   * @param _startPos Start point of the projectile.
   * @param _direction Direction of the projectile.
   */
  void
  fire(Vector3 _startPos, Vector2 _direction);

  /**
   * @brief bounce the projectile on the walls of the screen.
   * @param _width Width of the screen.
   * @param _height Height of the window.
   */
  void
  screenBounce(float _width, float _height);

  /**
   * @brief Apply gravity to the player direction.
   * @param _deltaTime Delta Time of the frames.
   */
  void
  gravity(float _deltaTime);

 public:
  float m_gravity;
  float m_speed;
  float m_maxSpeed;
  float m_bounceFactor;
  Vector3 m_position;
  Vector2 m_direction;
  SPtr<Actor> m_actor;
  bool m_fired;
};