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
  * Move the player.
  * 
  * @param _deltaTime
  * Time between the previous frame and the current one.
  * 
  * @param _direction
  * Direction in which the player will move.
  **/
  void
  move(float _deltaTime, Vector3 _direction);

  void
  screenBounce(float _width, float _height);

  void
  gravity(float _deltaTime);

 public:
  float m_gravity;
  float m_speed;
  float m_maxSpeed;
  float m_acceleration;
  float m_bounceFactor;
  Vector3 m_position;
  Vector2 m_direction;
  SPtr<Actor> m_actor;
  bool m_fired;
};