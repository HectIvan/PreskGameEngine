#include "PhysicsApp.h"
#include "pkVector3.h"
#include "pkVector2.h"
#include "pkModel.h"
#include "pkMath.h"

#include <iostream>

using pkEngineSDK::Vector3;
using pkEngineSDK::Vector2;
using pkEngineSDK::Matrix4;
using pkEngineSDK::Key;
using pkEngineSDK::Math;
using pkEngineSDK::Model;

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
  // create the bullet.
  player = new Player();
  // create a new game object.
  m_scene.instantiate();
  SPtr<Actor> projectile = m_scene.m_actors[0];
  // assign a new model component to the game object.
  projectile->addComponent(newModel("sprite.fbx"));
  projectile->addComponent(newMaterial("circle.png"));
  // add the game object to the player.
  player->m_actor = projectile;

  // canon creation.
  m_scene.instantiate();
  SPtr<Actor> canon = m_scene.m_actors[1];
  canon->addComponent(m_scene.m_actors[0]->getComponent<Model>());
  canon->addComponent(newMaterial("Canon.png"));
  canon->move(Vector3(-2.0f, 0.0f, 0.0f));
  canon->setPosition(Vector3(-13.0f, 7.0f, 0.0f));
  projectile->setPosition(canon->m_transform.getTranslation3());
  canon->setPosition(Vector3(-13.0f, 7.0f, -1.0f));
  canon->setRotation(0.0f, 0.0f, -1.5708f);

  player->start();

  m_fireDirection = Vector2(0.0f);
}

void
PhysicsApp::fixedUpdate()
{
  SPtr<Actor> canon = m_scene.m_actors[1];
  // clamping the direction of the player
  // player->m_direction.clamp(-1.0f, 1.0f);

  // fire the projectile
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kF))
  {
    Vector3 newPos = canon->m_transform.getTranslation3();
    newPos.z += 1.0f;
    player->fire(newPos, m_fireDirection);
    std::cout << player->m_speed << std::endl;
  }
  // move left or right
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kA))
  {
    m_fireDirection.x -= 1.0f * m_fixedDeltaTime;
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kD))
  {
    m_fireDirection.x += 1.0f * m_fixedDeltaTime;
  }

  m_fireDirection.y = -1.0f + player->m_direction.x;


  if (player->m_fired) {
    player->gravity(m_fixedDeltaTime);
  }

  // move and bounce the player
  player->move(m_fixedDeltaTime, Vector3(player->m_direction.x * player->m_speed,
                                         player->m_direction.y * player->m_speed,
                                         0.0f));

  player->screenBounce(30, 17);
  // clamp the player speed
  player->m_speed = Math::clamp(player->m_speed, 0, player->m_maxSpeed);
}
