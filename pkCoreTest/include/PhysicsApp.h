/*****************************************************************************/
/**
 * @file    PhysicsApp.h
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    10/01/2025
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
#include "Cannon.h"
#include "Obstacle.h"
#include "pkBaseApp.h"
#include "pkMaterial.h"
#include "pkModel.h"
#include "pkScene.h"
#include "Projectile.h"

using pkEngineSDK::BaseApp;
using pkEngineSDK::Material;
using pkEngineSDK::Model;
using pkEngineSDK::Scene;
using pkEngineSDK::Vector;

namespace PHYSICS_TYPE
{
  enum E
  {
    kVerlet = 0,
    kEuler,
  };
}

class PhysicsApp : public BaseApp
{
 public:
  PhysicsApp() = default;
  ~PhysicsApp() = default;

  /**
   * @brief On initializing the app.
   */
  void
  onInit() override;

  /**
   * @brief On updating the app.
   * @param _deltaTime Time between frames.
   */
  void
  onUpdate(float _deltaTime) override;

  /**
   * @brief Update on a fixed timer.
   */
  void
  fixedUpdate() override;

  /**
   * @brief Simulate physics.
   * @param _deltaTime Time between frames.
   */
  void
  physics(float _deltaTime);

  void
  fireProjectile();

  void
    checkObstacles();

  void
  onRender(Scene& _scene);
  
 public:
  PHYSICS_TYPE::E m_type;

  Obstacle obstacle;

  uint32_t m_projectileCount;
  float m_projDuration;

  bool m_firing = false;
  bool m_changingType = false;

  SPtr<Cannon> m_cannon;
  Vector<SPtr<Projectile>> m_projectiles;

  SPtr<Model> m_spriteModel;
  SPtr<Material> m_projectileMaterial;
  Vector2 m_fireDirection;
};