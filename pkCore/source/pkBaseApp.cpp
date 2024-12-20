#include <iostream>

#include "pkBaseApp.h"
#include "pkDllLoader.h"
#include "pkGameObject.h"
#include "pkGraphicsAPI.h"
#include "pkLight.h"
#include "pkModel.h"
#include "pkPrerequisitesCore.h"
#include "pkWindowDesc.h"

#include "pkPlatformMath.h"

namespace pkEngineSDK
{

void
run(String _name, Window& _window);

/**
* Create a game object.
* 
* @return
* A game object shared pointer.
**/
SPtr<GameObject>
createGameObject()
{
  SPtr<GameObject> gObject = make_shared<GameObject>();
  gObject->init(Transform(0.0f));
  gObject->setScale(Matrix4(1.0f));
  return gObject;
}

/**
* Insert a game object into the vector of the game object.
* 
* @param _pObject
* Object to insert.
* 
* @param _vector
* Vector where the game object will be inserted.
**/
void
insertGameObject(SPtr<GameObject> _pObject, Vector<SPtr<GameObject>>& _vector)
{
  _vector.push_back(_pObject);
}

/**
* Create a material from a file.
* 
* @param _fileName
* Name of th file to search for.
* 
* @return
* The material Shared pointer.
**/
SPtr<Material>
createMaterial(String& _fileName)
{
  // get api instance
  GraphicsAPI& api = GraphicsAPI::instance();
  // create the texture adress
  String textureName = "textures/" + _fileName;
  // create the texture
  SPtr<Texture> texture = api.createTextureFromFile(textureName, 8, false, 28);
  // if creating the texture failed, return the model without a texture
  if (!texture) { return nullptr; }
  // create the material component
  SPtr<Material> pMatComp = make_shared<Material>();
  pMatComp->setTexture(pMatComp->diffuse, texture);
  // return the texture
  return pMatComp;
}

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
  api.compileShaders();
  api.createShaders();
  api.createInputLayout();
  api.createSamplerState();
  createBuffers();
  camera.init(window.getWidth(),
              window.getHeight(),
              3.1416f / 4.0f,
              0.01f,
              1000.0f,
              Vector4(0.0f, 10.0f, -30.0f, 1.0f), // w is position in 1
              Vector4(0.0f, 0.0f, 0.0f, 1.0f),
              Vector4(0.0f, 1.0f, 0.0f, 0.0f));
  // load the model
  String modelName = "rat.fbx";
  SPtr<Model> model = loadModel(modelName);
  // create a material
  String texName = "rat_diffuse.png";
  SPtr<Material> pMat = createMaterial(texName);
  // insert the game object into the vector of game objects
  SPtr<GameObject> gObject = createGameObject();
  gObject->insertModel(model);
  gObject->addComponent(pMat);
  insertGameObject(gObject, gameObjects);
  cameraSpeed = 5.0f;
  messageLoop();
}

void
BaseApp::initWindow()
{
  PKWindowDesc desc;
  desc.width = 1920;
  desc.height = 1080;
  std::string name = "Presk Game Engine Window";
  window.create(desc, name);
}

void
BaseApp::initAPI(const char** _argv)
{
  String abstraction = _argv[1];

#ifdef PK_DEBUG_MODE
  if (abstraction == "DX11API") {
    run("pkDX11APId", window);
  }
#else
  if (abstraction == "DX11API") {
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
  if (dllSymbol) {
    dllSymbol(_window);
  }
}

void
BaseApp::createBuffers()
{
  GraphicsAPI& api = GraphicsAPI::instance();
  cBView = api.createConstantBuffer(static_cast<uint32>(sizeof(CBView)), nullptr, 0);
  cBProjection = api.createConstantBuffer(static_cast<uint32>(sizeof(CBProjection)), nullptr, 0);
  cBWorld = api.createConstantBuffer(static_cast<uint32>(sizeof(CBWorld)), nullptr, 0);
  cbLight = api.createConstantBuffer(static_cast<uint32>(sizeof(Light)), nullptr, 0);
}

void
BaseApp::messageLoop()
{
  // get api reference
  GraphicsAPI& api = GraphicsAPI::instance();
  float rot = 0.0f;
  // get the starting deltaTime
  high_resolution_clock::time_point delta = high_resolution_clock::now();
  // event loop
  while (!eventQueue.iskeyPressed(KEY::kEsc))
  {
    // update the delta time
    float deltaTime = getDeltaTime(delta);
    // update the camera speed
    float camSpeed = cameraSpeed * deltaTime;
    eventQueue.poll();
    // move forward/backward
    if (eventQueue.iskeyPressed(KEY::kW)) {
      camera.move(Vector3(0.0f, 0.0f, camSpeed));
    }
    if (eventQueue.iskeyPressed(KEY::kS)) {
      camera.move(Vector3(0.0f, 0.0f, -camSpeed));
    }
    // move left/right
    if (eventQueue.iskeyPressed(KEY::kA)) {
      camera.move(Vector3(-camSpeed, 0.0f, 0.0f));
    }
    if (eventQueue.iskeyPressed(KEY::kD)) {
      camera.move(Vector3(camSpeed, 0.0f, 0.0f));
    }
    // move up/down
    if (eventQueue.iskeyPressed(KEY::kE)) {
      camera.move(Vector3(0.0f, camSpeed, 0.0f));
    }
    if (eventQueue.iskeyPressed(KEY::kQ)) {
      camera.move(Vector3(0.0f, -camSpeed, 0.0f));
    }
    // rotate world
    if (eventQueue.iskeyPressed(KEY::kLeft)) {
      rot += deltaTime * 10.0f;
      api.world = Matrix4::rotationY(rot);
    }
    if (eventQueue.iskeyPressed(KEY::kRight)) {
      rot += deltaTime * -10.0f;
      api.world = Matrix4::rotationY(rot);
    }
    if (eventQueue.iskeyPressed(KEY::kDown)) {
      rot += deltaTime * 10.0f;
      api.world = Matrix4::rotationX(rot);
    }
    if (eventQueue.iskeyPressed(KEY::kUp)) {
      rot += deltaTime * -10.0f;
      api.world = Matrix4::rotationX(rot);
    }
    // update camera
    updateCamera(&camera);
    // render the scene
    render();
  }
}

void
BaseApp::updateCamera(Camera* _pCamera)
{
  // get the api instance to work with
  GraphicsAPI& api = GraphicsAPI::instance();

  /*****************/
  /**
  * Update view
  **/
  /*****************/
  CBView viewBuffer = CBView();
  viewBuffer.view = _pCamera->view.getTransposed();
  api.updateConstantBuffer(cBView, &viewBuffer, 0);

  /*****************/
  /**
  * Update projection
  **/
  /*****************/
  CBProjection projectionBuffer = CBProjection();
  projectionBuffer.projection = _pCamera->projection.getTransposed();
  api.updateConstantBuffer(cBProjection, &projectionBuffer, 0);
}

void
BaseApp::setGameObjectsBuffers()
{
  // get the api
  GraphicsAPI& api = GraphicsAPI::instance();

  // fr each game object in the world
  for (uint32 i = 0; i < gameObjects.size(); ++i) {
    // for each model in the game object
    for (uint32 j = 0; j < gameObjects[i]->models.size(); ++j) {
      // set its vertex and index buffers
      api.setVertexBuffer(gameObjects[i]->models[j]->vertexB);
      api.setIndexBuffer(gameObjects[i]->models[j]->indexB); ;
    }
  }
}

void
BaseApp::VSSetConstantBuffers()
{
  // get the api instance
  GraphicsAPI& api = GraphicsAPI::instance();

  // set the constant buffers
  api.VSSetConstantBuffer(cBView, 0, 1);
  api.VSSetConstantBuffer(cBProjection, 1, 1);
  api.VSSetConstantBuffer(cBWorld, 2, 1);
  api.VSSetConstantBuffer(cbLight, 3, 1);
}

void
BaseApp::PSSetConstantBuffers()
{
  // get the api instance
  GraphicsAPI& api = GraphicsAPI::instance();

  // set the constant buffers
  api.PSSetConstantBuffer(cBView, 0, 1);
  api.PSSetConstantBuffer(cBProjection, 1, 1);
  api.PSSetConstantBuffer(cBWorld, 2, 1);
  api.PSSetConstantBuffer(cbLight, 3, 1);
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
  // update the world and light buffers
  CBWorld ef;
  ef.world = api.world;
  api.updateConstantBuffer(cBWorld, &ef, static_cast<uint32>(sizeof(CBWorld)));
  api.updateConstantBuffer(cbLight, &light, static_cast<uint32>(sizeof(Light)));
  // Set shaders
  api.setShaders();
  // set light
  light.Type = LIGHT_TYPE::kDirectional;
  light.LightDir = Vector3::FORWARD;
  // set constant buffers for the pixel and vertex shaders
  VSSetConstantBuffers();
  PSSetConstantBuffers();
  // render the objects
  renderGameObjects();
  api.present(1, 0);
}

void
BaseApp::renderGameObjects()
{
  // get a reference from the api
  GraphicsAPI& api = GraphicsAPI::instance();
  // for each Game Object
  for (uint32 i = 0; i < gameObjects.size(); ++i) {
    // set the diffuse texture to the resource view
    api.setShaderResourceView(gameObjects[i]->getComponent<Material>()->diffuse);
    api.setSampler();
    // check all their models
    for (uint32 j = 0; j < gameObjects[i]->models.size(); ++j) {
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
  for (uint32 i = 0; i < _model.meshes.size(); ++i) {
    // draw the mesh
    api.drawIndexed(static_cast<uint32>(_model.meshes[i].numIndex),
                                        currentIndexOrigin,
                                        currentVertexOrigin);
    // update the offsets
    currentIndexOrigin += static_cast<uint32>(_model.meshes[i].numIndex);
    currentVertexOrigin += static_cast<uint32>(_model.meshes[i].vertexCount);
  }
}

SPtr<Model>
BaseApp::loadModel(String& _fileName)
{
  // create the model path
  String modelPath = "models/" +  _fileName;
  // get the api
  GraphicsAPI& api = GraphicsAPI::instance();
  // create the model pointer
  SPtr<Model> model = make_shared<Model>();
  // load the model from the path
  model->load(modelPath);
  // create the index and vertex buffers
  model->vertexB = api.createVertexBuffer(model->vertex);
  model->indexB = api.createIndexBuffer(model->index);
  api.setIndexBuffer(model->indexB);
  api.setVertexBuffer(model->vertexB);
  // return the final model
  return model;
}
}