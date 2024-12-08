#include "pkDllLoader.h"
#include "pkBaseApp.h"
#include "pkGameObject.h"
#include "pkGraphicsAPI.h"
#include "pkModel.h"
#include "pkWindowDesc.h"

namespace pkEngineSDK
{

void
run(String _name, Window& _window);

GameObject*
createGameObject();

Model*
loadModel(String _path);

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

GameObject*
createGameObject()
{
  GameObject* gameObject = new GameObject();
  gameObject->init(Transform(0.0f));
  gameObject->setScale(Matrix4(1.0f));
  gameObject->insertModel(loadModel("models/maneater.fbx"));
  return gameObject;
}

Model*
loadModel(String _path)
{
  Model* model = new Model();
  model->load(_path);
  return model;
}

void
render()
{

}
}