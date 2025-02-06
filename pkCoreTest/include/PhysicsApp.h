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

#include "Cannon.h"
#include "pkMaterial.h"
#include "pkModel.h"
#include "pkBaseApp.h"
#include "Projectile.h"

using pkEngineSDK::BaseApp;
using pkEngineSDK::Material;
using pkEngineSDK::Model;
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

  void
  onInit() override;

  void
  onUpdate(float _deltaTime) override;

  void
  fixedUpdate() override;

  void
  physics(float _deltaTime);

  void
  fireProjectile();
  
 public:
  PHYSICS_TYPE::E m_type;

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