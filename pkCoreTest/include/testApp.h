#pragma once

#include "pkBaseApp.h"
#include "Player.h"

using pkEngineSDK::BaseApp;

class TestApp : public BaseApp
{
 public:
  TestApp() = default;
  ~TestApp() = default;

  void
  onInit() override;

  void
  onUpdate(float _deltaTime) override;

  Player* player;
};