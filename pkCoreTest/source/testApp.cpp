#include "testApp.h"
#include "pkVector3.h"
#include "pkVector2.h"
#include "pkModel.h"
#include "pkPlatformMath.h"

#include <iostream>

using pkEngineSDK::Vector3;
using pkEngineSDK::Vector2;
using pkEngineSDK::Matrix4;
using pkEngineSDK::Key;
using pkEngineSDK::Model;
using pkEngineSDK::PlatformMath;

void
TestApp::onInit()
{
  // create the player.
  player = new Player();
  // create a new game object.
  instantiate();
  // assign a new model component to the game object.
  m_gameActors[0]->addComponent(newModel("sprite.fbx"));
  m_gameActors[0]->addComponent(newMaterial("circle.png"));
  // add the game object to the player.
  player->m_actor = m_gameActors[0];

  instantiate(Matrix4::IDENTITY, m_gameActors[0]);
  player->m_actor->m_children[0]->addComponent(m_gameActors[0]->getComponent<Model>());
  player->m_actor->m_children[0]->addComponent(newMaterial("arrowRed.png"));
  player->m_actor->m_children[0]->move(Vector3(-2.0f, 0.0f, 0.0f));
  // player->m_actor->m_children[0]->setRotation(Vector3(0.0f, 0.0f, 3.14159f));

  player->m_speed = 0.0f;
  player->m_maxSpeed = 10.0f;
  player->m_acceleration = 5.0f;
  player->m_direction = Vector2(0);
}

void
TestApp::onUpdate(float _deltaTime)
{
  bool input = false;
  // Player input
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kLeft)) {
    player->m_direction.x += -1.0f * _deltaTime;
    // player->m_actor->m_children[0]->setRotation(Vector3(0.0f, 0.0f, -90.0f));
    input = true;
  }
  else {
    player->m_direction.x += 1.0f * _deltaTime;
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kRight)) {
    player->m_direction.x += 1.0f * _deltaTime;
    // player->m_actor->m_children[0]->setRotation(Vector3(0.0f, 0.0f, 90.0f));
    input = true;
  }
  else {
    player->m_direction.x += -1.0f * _deltaTime;
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kDown)) {
    player->m_direction.y += 1.0f * _deltaTime;
    // player->m_actor->m_children[0]->setRotation(Vector3(0.0f, 0.0f, -180.0f));
    input = true;
  }
  else {
    player->m_direction.y += -1.0f * _deltaTime;
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kUp)) {
    player->m_direction.y += -1.0f * _deltaTime;
    // player->m_actor->m_children[0]->setRotation(Vector3(0.0f, 0.0f, 180.0f));
    input = true;
  }
  else {
    player->m_direction.y += 1.0f * _deltaTime;
  }

  // clamping the direction of the player
  player->m_direction.clamp(-1.0f, 1.0f);

  float move = player->m_direction.x;
  float rotate = -player->m_direction.y;

  // if no input has been made, slow down the player
  if (!input) {
    player->m_speed -= player->m_acceleration * _deltaTime;
  }
  // otherwise, accelerate the player
  else {
    player->m_speed += player->m_acceleration * _deltaTime;
  }

  // move and bounce the player
  player->move(_deltaTime, Vector3(player->m_direction.x * player->m_speed, player->m_direction.y * player->m_speed, 0.0f));
  player->screenBounce(30, 17);
  // clamp the player speed
  player->m_speed = PlatformMath::clamp(player->m_speed, 0, player->m_maxSpeed);
}
