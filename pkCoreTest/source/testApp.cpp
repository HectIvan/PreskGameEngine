#include "testApp.h"
#include "pkVector3.h"

using pkEngineSDK::Vector3;
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
  player->friction = 0.1f;
}

void
TestApp::onUpdate(float _deltaTime)
{
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
  // rotate world
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kLeft)) {
    player->innertia += player->speed * _deltaTime;
    player->move(_deltaTime, Vector3(-player->innertia, 0.0f, 0.0f));
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kRight)) {
    player->innertia += player->speed * _deltaTime;
    player->move(_deltaTime, Vector3(player->innertia, 0.0f, 0.0f));
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kDown)) {
    player->innertia += player->speed * _deltaTime;
    player->move(_deltaTime, Vector3(0.0f, -player->innertia, 0.0f));
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kUp)) {
    player->innertia += player->speed * _deltaTime;
    player->move(_deltaTime, Vector3(0.0f, player->innertia, 0.0f));
  }
  if (player->innertia > player->speed) { player->innertia = player->speed; }
  player->innertia -= player->friction * _deltaTime;
  if (player->innertia < 0) { player->innertia = 0.0f; }
  // backspace input
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kBackSpace)) {
    // if the game object pool is not empty
    if (!gameObjects.empty()) {
      // pop the last gameobject
      gameObjects.pop_back();
    }
  }
}