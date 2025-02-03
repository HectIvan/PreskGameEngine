/*****************************************************************************/
/**
 * @file    PhysicsApp.h
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    2025/01/10
 * @brief
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

#include "pkBaseApp.h"
#include "Player.h"

using pkEngineSDK::BaseApp;

class PhysicsApp : public BaseApp
{
 public:
  PhysicsApp() = default;
  ~PhysicsApp() = default;

  void
  onInit() override;

  void
  fixedUpdate() override;
  
  Player* player;
  Vector2 m_fireDirection;
};