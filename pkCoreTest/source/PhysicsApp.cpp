#include "PhysicsApp.h"
#include "pkDebug.h"
#include "pkMath.h"
#include "pkModel.h"
#include "pkVector3.h"
#include "pkVector2.h"

#include <iostream>

using pkEngineSDK::Debug;
using pkEngineSDK::Math;
using pkEngineSDK::Model;
using pkEngineSDK::Vector3;
using pkEngineSDK::Vector2;
using pkEngineSDK::Matrix4;
using pkEngineSDK::Key;

using std::make_shared;

void
PhysicsApp::onInit()
{
  m_camera.init(30,
                17,
                3.1416f / 4.0f,
                0.01f,
                1000.0f,
                Vector3(0.0f, 0.0f, -30.0f), // position
                Vector3(0.0f, 0.0f, 0.0f), // target
                Vector3(0.0f, 1.0f, 0.0f), // up vector
                pkEngineSDK::CAMERA_PROJ::kOrthographic);

  m_type = PHYSICS_TYPE::kEuler;

  // create one model instance
  m_projectileCount = 100;
  m_projDuration = 10.0f;
  float projSpeed = 80.0f;
  m_spriteModel = newModel("sprite.fbx");
  m_projectileMaterial = createMaterial("circle.png");

  /**
   * Cannon creation
   */
  m_scene.instantiate();
  m_cannon = std::make_shared<Cannon>();
  m_cannon->m_actor = m_scene.m_actors[0];
  m_cannon->m_actor->addComponent(m_spriteModel);
  m_cannon->m_actor->addComponent(createMaterial("Canon.png"));
  m_cannon->m_actor->move(Vector3(-0.0f, 0.0f, 0.0f));
  m_cannon->m_actor->setPosition(Vector3(0.0f, 7.0f, 0.0f));
  m_cannon->m_actor->setRotation(0.0f, 0.0f, -1.5708f);

  for (uint32_t i = 0; i < m_projectileCount; ++i) {
    // instantiate an actor
    m_scene.instantiate();
    // make new instance of a projectile
    SPtr<Projectile> proj = make_shared<Projectile>();
    // get the last instance
    proj->m_actor = m_scene.m_actors[m_scene.m_actors.size() - 1];
    // start the projectile
    proj->start();
    // set projectile speed
    proj->m_speed = projSpeed;
    proj->m_maxSpeed = projSpeed;
    // set the projectile lifetime
    proj->m_lifeTimer = m_projDuration;
    // assign a new model component to the game object.
    proj->m_actor->addComponent(m_spriteModel);
    proj->m_actor->addComponent(m_projectileMaterial);
    // add the game object to the vector of projectiles
    m_projectiles.push_back(proj);
  }

  m_fireDirection = Vector2(0.0f);
}


void
PhysicsApp::onUpdate(float _deltaTime)
{
  if (m_type == PHYSICS_TYPE::kEuler) {
    physics(_deltaTime);
  }
}

void
PhysicsApp::fixedUpdate()
{
  if (m_type == PHYSICS_TYPE::kVerlet) {
    physics(m_fixedDeltaTime);
  }
}

void
PhysicsApp::physics(float _deltaTime)
{
  // fire the projectile
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kF) &&
    !m_firing)
  {
    fireProjectile();
    m_firing = true;
  }
  else if (!m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kF))
  {
    m_firing = false;
  }
  // move left or right
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kA))
  {
    m_fireDirection.x -= 1.0f * _deltaTime;
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kD))
  {
    m_fireDirection.x += 1.0f * _deltaTime;
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kC) &&
      !m_changingType) {
    m_changingType = true;
    // change simulation type
    if (m_type == PHYSICS_TYPE::kEuler) {
      m_type = PHYSICS_TYPE::kVerlet;
      Debug::print("Verlet integration active");
    }
    else {
      m_type = PHYSICS_TYPE::kEuler;
      Debug::print("Euler integration active");
    }
  }
  else if (!m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kC)) {
    m_changingType = false;
  }
  // calculate the Y direction of the fire direction
  m_fireDirection.y = -1.0f + Math::abs(m_fireDirection.x);
  m_fireDirection.y = Math::clamp(m_fireDirection.y, -1.0f, 0.0f);
  m_fireDirection.x = Math::clamp(m_fireDirection.x, -1.0f, 1.0f);

  // apply gravity to every projectile
  for (uint32_t i = 0; i < m_projectiles.size(); ++i) {
    if (m_projectiles[i]->m_fired) {
      m_projectiles[i]->gravity(_deltaTime);
    }
    // move and bounce the player
    m_projectiles[i]->move(_deltaTime, Vector3(m_projectiles[i]->m_direction.x *
                                                     m_projectiles[i]->m_speed,
                                                     m_projectiles[i]->m_direction.y *
                                                     m_projectiles[i]->m_speed,
                                                     0.0f));
    m_projectiles[i]->screenBounce(30, 17);
    // clamp the projectile speed
    m_projectiles[i]->m_speed = Math::clamp(m_projectiles[i]->m_speed,
                                            0,
                                            m_projectiles[i]->m_maxSpeed);
    if (m_projectiles[i]->m_actor->isActive()) {
      m_projectiles[i]->projTimer(_deltaTime);
    }
  }
}

void
PhysicsApp::fireProjectile()
{
  Vector3 newPos = m_cannon->m_actor->m_transform.getTranslation3();
  newPos.z += 1.0f;
  for (uint32_t i = 0; i < m_projectiles.size(); ++i) {
    if (!m_projectiles[i]->m_actor->isActive())
    {
      m_projectiles[i]->fire(newPos, m_fireDirection);
      return;
    }
  }
}
