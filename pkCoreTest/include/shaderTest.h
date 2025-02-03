#pragma once

#include "pkActor.h"
#include "pkBaseApp.h"
#include "pkVector2.h"
#include "pkVector3.h"

using pkEngineSDK::Actor;
using pkEngineSDK::BaseApp;
using pkEngineSDK::Vector2;
using pkEngineSDK::Vector3;

class ShaderTest : public BaseApp
{
public:
  ShaderTest() = default;
  ~ShaderTest() = default;

  void
  onInit() override;

  void
  onUpdate(float _deltaTime) override;

  Actor* m_actor;
  Vector2 m_lastCursorPos;
};