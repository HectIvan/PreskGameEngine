#include "testApp.h"
#include "pkVector3.h"
#include "pkVector2.h"

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
  gameObjects[0]->addComponent(newModel("Shadow_Leviathan_anim.fbx"));
  // add the game object to the player.
  player->gameObject = gameObjects[0];

  player->speed = 5.0f;
  player->acceleration = 0.1f;
}

void
TestApp::onUpdate(float _deltaTime)
{
  Vector2 axis(0);
  // rotate world
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kLeft)) {
    axis.x = -1.0f;
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kRight)) {
    axis.x = 1.0f;
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kDown)) {
    axis.y = -1.0f;
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kUp)) {
    axis.y = 1.0f;
  }

  float move = axis.x;
  float rotate = -axis.y;

  // manage innertia
  if (move > 0) { player->speed += player->acceleration * _deltaTime; }
  else { player->speed -= player->acceleration * _deltaTime; }

  // clamp speed
  if (player->speed > player->maxSpeed) { player->speed = player->maxSpeed; }
  if (player->speed < 0) { player->speed = 0; }

  // move forward
  // Vector3 newPos = transform.right * (speed * Time.deltaTime);
  // player->gameObject->transform.setTransation() += player->gameObject->transform.getTranslation()

  // object rotation
  // transform.Rotate(new Vector3(0, 0, rotSpeed * Time.deltaTime * rotate));
  // UpdateWindow();
}

/**
// update the camera speed
  float camSpeed = cameraSpeed * _deltaTime;
  // move forward/backward
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kW)) {
    camera.move(Vector3(0.0f, 0.0f, camSpeed));
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kS)) {
    camera.move(Vector3(0.0f, 0.0f, -camSpeed));
  }
  // move left/right
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kA)) {
    camera.move(Vector3(-camSpeed, 0.0f, 0.0f));
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kD)) {
    camera.move(Vector3(camSpeed, 0.0f, 0.0f));
  }
  // move up/down
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kE) ||
    eventQueue.iskeyPressed(pkEngineSDK::KEY::kSpace)) {
    camera.move(Vector3(0.0f, camSpeed, 0.0f));
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kQ) ||
      eventQueue.iskeyPressed(pkEngineSDK::KEY::kLControl)) {
    camera.move(Vector3(0.0f, -camSpeed, 0.0f));
  }
**/