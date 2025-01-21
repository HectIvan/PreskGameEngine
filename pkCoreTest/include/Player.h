#pragma once
#include "pkVector3.h"
#include "pkGameObject.h"
#include "pkPrerequisitesCore.h"

using pkEngineSDK::Vector3;
using pkEngineSDK::GameObject;
using pkEngineSDK::SPtr;

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

  float speed;
  float maxSpeed;
  float innertia;
  float acceleration;
  Vector3 position;
  SPtr<GameObject> gameObject;
};