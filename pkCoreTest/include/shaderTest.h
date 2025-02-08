#pragma once

#include "pkActor.h"
#include "pkBaseApp.h"
#include "pkScene.h"
#include "pkTexture.h"
#include "pkVector2.h"
#include "pkVector3.h"

using pkEngineSDK::Actor;
using pkEngineSDK::BaseApp;
using pkEngineSDK::Scene;
using pkEngineSDK::SPtr;
using pkEngineSDK::Texture;
using pkEngineSDK::Vector2;
using pkEngineSDK::Vector3;

class ShaderTest : public BaseApp
{
 public:
  ShaderTest() = default;
  virtual ~ShaderTest() = default;

  void
  onInit() override;

  void
  onUpdate(float _deltaTime) override;

  void
  onRender(Scene& _scene) override;

  Actor* m_actor;
  Vector2 m_lastCursorPos;

  SPtr<Texture> m_pRTDepth;
  SPtr<Texture> m_pRTNormal;
  SPtr<Texture> m_pRTColor;
};