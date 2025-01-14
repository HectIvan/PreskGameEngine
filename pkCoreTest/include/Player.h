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

  void
  move(float _deltaTime, Vector3 _direction);

  float speed;
  float acceleration;
  float friction;
  Vector3 position;
  SPtr<GameObject> gameObject;
};