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
  String modelPath = "D:/Work/visual studio/PreskGameEngine/models/" +
                     _modelName +
                     "." +
                     _extension;
  Model* model = loadModel(modelPath);
  insertGameObject(createGameObject(model), gameObjects);
  cameraSpeed = 10.0f;
  messageLoop();
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
BaseApp::messageLoop()
{
  // get the api
  GraphicsAPI& api = GraphicsAPI::instance();
  // get the deltaTime
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
    api.updateCamera(&camera);
    // render the scene
    render();
  }
}

Model*
BaseApp::loadModel(String& _path)
{
  // get the api
  GraphicsAPI& api = GraphicsAPI::instance();

  Model* model = new Model();
  String modelPath = _path;
  model->load(modelPath);
  model->vertexB = api.createVertexBuffer(model->vertex);
  model->indexB = api.createIndexBuffer(model->index);
  api.setIndexBuffer(model->indexB);
  api.setVertexBuffer(model->vertexB);
  return model;
}

void
BaseApp::setGameObjectsBuffers()
{
  // get the api
  GraphicsAPI& api = GraphicsAPI::instance();

  // fr each game object in the world
  for (uint32 i = 0; i < gameObjects.size(); ++i)
  {
    // for each model in the game object
    for (uint32 j = 0; j < gameObjects[i]->models.size(); ++j)
    {
      // set its vertex and index buffers
      api.setVertexBuffer(gameObjects[i]->models[j]->vertexB);
      api.setIndexBuffer(gameObjects[i]->models[j]->indexB); ;
    }
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
BaseApp::render()
{
  GraphicsAPI& api = GraphicsAPI::instance();
  // screen clear color
  float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
  api.clearRenderTargetView(clearColor);
  api.clearDepthBuffer(1.0f);

  api.updateWorldAndLightCB();

  api.setShaders();
  setGameObjectsBuffers();
  api.VSSetConstantBuffers();
  api.PSSetConstantBuffers();
  api.setSampler();
  renderGameObjects();
  api.present(1, 0);
}

void
BaseApp::renderGameObjects()
{
  // for each Game Object
  for (uint32 i = 0; i < gameObjects.size(); ++i)
  {
    // check all their models
    for (uint32 j = 0; j < gameObjects[i]->models.size(); ++j)
    {
      // draw the model
      renderModel(*gameObjects[i]->models[j]);
    }
  }
}

void
BaseApp::renderModel(Model& _model)
{
  // get a reference from the api
  GraphicsAPI& api = GraphicsAPI::instance();
  // offsets
  uint32 currentVertexOrigin = 0;
  uint32 currentIndexOrigin = 0;
  // for each mesh in the model
  for (uint32 i = 0; i < _model.meshes.size(); ++i)
  {
    // draw the mesh
    api.drawIndexed(static_cast<uint32>(_model.meshes[i].numIndex),
                                        currentIndexOrigin,
                                        currentVertexOrigin);
    // update the offsets
    currentIndexOrigin += static_cast<uint32>(_model.meshes[i].numIndex);
    currentVertexOrigin += static_cast<uint32>(_model.meshes[i].vertexCount);
  }
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