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

void
render();

/*********************************************/
/**
* Definitions.
**/
/*********************************************/

void
BaseApp::init(const char** _argv)
{
  initWindow();
  initAPI(_argv);
  GraphicsAPI& api = GraphicsAPI::instance();
  String modelPath = "D:/Work/visual studio/PreskGameEngine/models/maneater.fbx";
  Model* model = api.loadModel(modelPath);
  insertGameObject(createGameObject(model), api.gameObjects);
  cameraSpeed = 100.0f;
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
printVal(Vector4 _val)
{
  std::cout << _val.x << std::endl <<
               _val.y << std::endl <<
               _val.z << std::endl;
}

void
BaseApp::messageLoop(GraphicsAPI* _api)
{
  while (true)
  {
    eventQueue.poll();
    // move forward/backward
    if (eventQueue.iskeyPressed(KEY::kW))
    {
      _api->m_camera.move(Vector3(0.0f, 0.0f, cameraSpeed));
    }
    if (eventQueue.iskeyPressed(KEY::kS))
    {
      _api->m_camera.move(Vector3(0.0f, 0.0f, -cameraSpeed));
    }
    // move left/right
    if (eventQueue.iskeyPressed(KEY::kA))
    {
      _api->m_camera.move(Vector3(-cameraSpeed, 0.0f, 0.0f));
    }
    if (eventQueue.iskeyPressed(KEY::kD))
    {
      _api->m_camera.move(Vector3(cameraSpeed, 0.0f, 0.0f));
    }
    // move up/down
    if (eventQueue.iskeyPressed(KEY::kE))
    {
      _api->m_camera.move(Vector3(0.0f, cameraSpeed, 0.0f));
    }
    if (eventQueue.iskeyPressed(KEY::kQ))
    {
      _api->m_camera.move(Vector3(0.0f, -cameraSpeed, 0.0f));
    }
    // update camera
    _api->updateCamera(&_api->m_camera);
    // render the scene
    render(_api);
  }
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

void
render()
{

}
}