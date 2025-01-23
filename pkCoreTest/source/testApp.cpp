#include "testApp.h"
#include "pkVector3.h"
#include "pkVector2.h"

#include <iostream>

using pkEngineSDK::Vector3;
using pkEngineSDK::Vector2;
using pkEngineSDK::Matrix4;
using pkEngineSDK::Key;

void
TestApp::onInit()
{
  // create the player.
  player = new Player();
  // create a new game object.
  newGameObject();
  // assign a new model component to the game object.
  gameObjects[0]->addComponent(newModel("Grass_Block.obj  "));
  gameObjects[0]->addComponent(newMaterial("cube_Circle.png"));
  // add the game object to the player.
  player->m_gameObject = gameObjects[0];

  player->m_speed = 0.0f;
  player->m_maxSpeed = 10.0f;
  player->m_acceleration = 1.0f;
  player->m_direction = Vector2(0);
}

void
TestApp::onUpdate(float _deltaTime)
{
  bool input = false;
  // rotate world
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kLeft)) {
    player->m_direction.x += -1.0f * _deltaTime;
    input = true;
  }
  else {
    player->m_direction.x += 1.0f * _deltaTime;
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kRight)) {
    player->m_direction.x += 1.0f * _deltaTime;
    input = true;
  }
  else {
    player->m_direction.x += -1.0f * _deltaTime;
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kDown)) {
    player->m_direction.y += 1.0f * _deltaTime;
    input = true;
  }
  else {
    player->m_direction.y += -1.0f * _deltaTime;
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kUp)) {
    player->m_direction.y += -1.0f * _deltaTime;
    input = true;
  }
  else {
    player->m_direction.y += 1.0f * _deltaTime;
  }

  player->m_direction.clamp(-1.0f, 1.0f);

  float move = player->m_direction.x;
  float rotate = -player->m_direction.y;

  if (!input) {
    player->m_speed -= player->m_acceleration * _deltaTime;
  }
  else {
    player->m_speed += player->m_acceleration * _deltaTime;
  }

  player->move(_deltaTime, Vector3(player->m_direction.x * player->m_speed, player->m_direction.y * player->m_speed, 0.0f));

  // manage innertia
  // if (move > 0) { player->m_speed += player->m_acceleration * _deltaTime; }
  // else { player->m_speed -= player->m_acceleration * _deltaTime; }

  // clamp m_speed
  if (player->m_speed > player->m_maxSpeed) { player->m_speed = player->m_maxSpeed; }
  if (player->m_speed < 0) { player->m_speed = 0; }

  // move forward
  // Vector3 newPos = transform.right * (m_speed * Time.deltaTime);
  // player->gameObject->transform.setTransation() += player->gameObject->transform.getTranslation()

  // object rotation
  // transform.Rotate(new Vector3(0, 0, rotm_speed * Time.deltaTime * rotate));
  // UpdateWindow();
}

/**

**/