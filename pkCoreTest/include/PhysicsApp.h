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
#include "pkInverseKinematics.h"
#include "pkMaterial.h"
#include "pkModel.h"
#include "pkScene.h"
#include "Projectile.h"
#include "Spring.h"

using pkEngineSDK::BaseApp;
using pkEngineSDK::InverseKinematics;
using pkEngineSDK::Material;
using pkEngineSDK::Model;
using pkEngineSDK::Scene;
using pkEngineSDK::uint32;
using pkEngineSDK::Vector;

namespace PHYSICS_TYPE
{
  enum E
  {
    kVerlet = 0,
    kEuler,
  };
}

namespace IK_TYPE
{
  enum E
  {
    kFabrik = 0,
    kCCD,
  };
}

class PhysicsApp : public BaseApp
{
 public:
  PhysicsApp() = default;
  virtual ~PhysicsApp() = default;

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
  onUpdate() override;

  /**
   * @brief Update on a fixed timer.
   */
  void
  fixedUpdate() override;

  /**
   * @brief Initialize a spring.
   * @param _pos Position of the spring.
   * @param _length Length of the spring.
   * @param _stiffness How stiff the spring will be.
   */
  void
  initSpring(Vector3 _pos, float _length, float _stiffness);

  /**
   * @brief Simulate physics.
   * @param _deltaTime Time between frames.
   */
  void
  physics(float _deltaTime);

  /**
   * @brief Moevemnts based on the verlet integration.
   */
  void
  verletMove();

  /**
   * @brief Fire a projectile from the pool.
   */
  void
  fireProjectile();

  /**
   * @brief Check obstacle collissions with projectiles.
   */
  void
  checkObstacles();

  /**
   * @brief Render the app.
   * @param _scene Scene to render.
   */
  void
  onRender() override;
  
 public:
  PHYSICS_TYPE::E m_type;
  IK_TYPE::E m_ikType = IK_TYPE::kFabrik;

  Vector<SPtr<Obstacle>> obstacles;

  uint32_t m_projectileCount;
  float m_projDuration;

  bool m_firing = false;
  bool m_changingType = false;

  SPtr<Cannon> m_cannon;
  SPtr<Spring> m_spring;
  Vector<SPtr<Projectile>> m_projectiles;
  SPtr<InverseKinematics> m_ik;

  Vector2 m_fireDirection;

  Vector3 m_target;
  SPtr<Actor> m_targetShape;
  bool m_add = false;
  bool m_subtract = false;
  uint32 currentBone = 0;
};