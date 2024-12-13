#include "pkDllLoader.h"
#include "pkBaseApp.h"
#include "pkGameObject.h"
#include "pkGraphicsAPI.h"
#include "pkModel.h"
#include "pkWindowDesc.h"

#include <iostream>

namespace pkEngineSDK
{

void
run(String _name, Window& _window);

GameObject*
createGameObject(Model* _model);

void
insertGameObject(GameObject* _object, Vector<GameObject*>& _vector);

/*********************************************/
/**
* Definitions.
**/
/*********************************************/

void
BaseApp::init(const char** _argv, String& _modelName, String& _extension)
{
  initWindow();
  initAPI(_argv);
  camera.init(window.getWidth(),
              window.getHeight(),
              3.1416f / 4.0f,
              0.01f,
              1000.0f,
              Vector4(0.0f, 10.0f, -30.0f, 1.0f), // w is position in 1
              Vector4(0.0f, 0.0f, 0.0f, 1.0f),
              Vector4(0.0f, 1.0f, 0.0f, 0.0f));
  GraphicsAPI& api = GraphicsAPI::instance();
  String modelPath = "D:/Work/visual studio/PreskGameEngine/models/" +
                     _modelName +
                     "." +
                     _extension;
  Model* model = api.loadModel(modelPath);
  insertGameObject(createGameObject(model), api.gameObjects);
  cameraSpeed = 10.0f;
  messageLoop(&api);
}

void
BaseApp::initWindow()
{
  PKWindowDesc desc;
  desc.width = 1920;
  desc.height = 1080;
  std::string name = "window test";
  window.create(desc, name);
}

void
BaseApp::initAPI(const char** _argv)
{
  String abstraction = _argv[1];

#ifdef PK_DEBUG_MODE
  if (abstraction == "DX11API")
  {
    run("pkDX11APId", window);
  }
#else
  if (abstraction == "DX11API")
  {
    run("pkDX11API", window);
  }
#endif
}


void
run(String _name, Window& _window)
{
  DllLoader dll;
  dll.init(_name);
  auto dllSymbol = static_cast<void(*)(Window)>(dll.getMethod("loadPlugin"));
  if (dllSymbol)
  {
    dllSymbol(_window);
  }
}

void
BaseApp::messageLoop(GraphicsAPI* _api)
{
  high_resolution_clock::time_point delta = high_resolution_clock::now();
  bool run = true;
  while (run)
  {
    float deltaTime = getDeltaTime(delta);
    float camSpeed = cameraSpeed * deltaTime;
    eventQueue.poll();
    // move forward/backward
    if (eventQueue.iskeyPressed(KEY::kW))
    {
      camera.move(Vector3(0.0f, 0.0f, camSpeed));
    }
    if (eventQueue.iskeyPressed(KEY::kS))
    {
      camera.move(Vector3(0.0f, 0.0f, -camSpeed));
    }
    // move left/right
    if (eventQueue.iskeyPressed(KEY::kA))
    {
      camera.move(Vector3(-camSpeed, 0.0f, 0.0f));
    }
    if (eventQueue.iskeyPressed(KEY::kD))
    {
      camera.move(Vector3(camSpeed, 0.0f, 0.0f));
    }
    // move up/down
    if (eventQueue.iskeyPressed(KEY::kE))
    {
      camera.move(Vector3(0.0f, camSpeed, 0.0f));
    }
    if (eventQueue.iskeyPressed(KEY::kQ))
    {
      camera.move(Vector3(0.0f, -camSpeed, 0.0f));
    }
    // leave the program
    if (eventQueue.iskeyPressed(KEY::kEsc))
    {
      run = false;
    }
    // update camera
    _api->updateCamera(&camera);
    // render the scene
    render(_api);
  }
}

float
BaseApp::getDeltaTime(high_resolution_clock::time_point& _delta)
{
  high_resolution_clock::time_point end = high_resolution_clock::now();
  float deltaTime = duration<float>(end - _delta).count();
  _delta = high_resolution_clock::now();
  return deltaTime;
}

void
BaseApp::render(GraphicsAPI* _api)
{
  _api->render();
}

GameObject*
createGameObject(Model* _model)
{
  GameObject* gameObject = new GameObject();
  gameObject->init(Transform(0.0f));
  gameObject->setScale(Matrix4(1.0f));
  gameObject->insertModel(_model);
  return gameObject;
}

void
insertGameObject(GameObject* _object, Vector<GameObject*>& _vector)
{
  _vector.push_back(_object);
}
}