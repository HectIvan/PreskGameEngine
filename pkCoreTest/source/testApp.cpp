#include "testApp.h"
#include "pkVector3.h"
#include "pkVector2.h"
#include "pkModel.h"

#include <iostream>

using pkEngineSDK::Vector3;
using pkEngineSDK::Vector2;
using pkEngineSDK::Matrix4;
using pkEngineSDK::Key;
using pkEngineSDK::Model;

void
TestApp::onInit()
{
  // create the player.
  player = new Player();
  // create a new game object.
  newGameObject();
  // assign a new model component to the game object.
  gameObjects[0]->addComponent(newModel("sprite.fbx"));
  gameObjects[0]->addComponent(newMaterial("circle.png"));
  // add the game object to the player.
  player->m_gameObject = gameObjects[0];

  newGameObject(Matrix4::IDENTITY, gameObjects[0]);
  player->m_gameObject->children[0]->addComponent(gameObjects[0]->getComponent<Model>());
  player->m_gameObject->children[0]->addComponent(newMaterial("arrowRed.png"));
  player->m_gameObject->children[0]->move(Vector3(-5.0f, 0.0f, 0.0f));
  // player->m_gameObject->children[0]->setRotation(Vector3(0.0f, 0.0f, 3.14159f));

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
    player->m_gameObject->children[0]->setRotation(Vector3(0.0f, 0.0f, -90.0f));
    input = true;
  }
  else {
    player->m_direction.x += 1.0f * _deltaTime;
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kRight)) {
    player->m_direction.x += 1.0f * _deltaTime;
    player->m_gameObject->children[0]->setRotation(Vector3(0.0f, 0.0f, 90.0f));
    input = true;
  }
  else {
    player->m_direction.x += -1.0f * _deltaTime;
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kDown)) {
    player->m_direction.y += 1.0f * _deltaTime;
    player->m_gameObject->children[0]->setRotation(Vector3(0.0f, 0.0f, -180.0f));
    input = true;
  }
  else {
    player->m_direction.y += -1.0f * _deltaTime;
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kUp)) {
    player->m_direction.y += -1.0f * _deltaTime;
    player->m_gameObject->children[0]->setRotation(Vector3(0.0f, 0.0f, 180.0f));
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

  player->screenBounce(30, 17);

  if (player->m_speed > player->m_maxSpeed) { player->m_speed = player->m_maxSpeed; }
  if (player->m_speed < 0) { player->m_speed = 0; }

  
}
