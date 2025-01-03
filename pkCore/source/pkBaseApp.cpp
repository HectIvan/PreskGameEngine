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
  gObject->setTransform(Matrix4(0.0f));
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

void
BaseApp::newGameObject(String _modelName,
                       String _textureName,
                       Matrix4 _transform,
                       SPtr<GameObject> _pParent)
{
  // load the model
  SPtr<Model> model = loadModel(_modelName);
  // create a material
  SPtr<Material> pMat = createMaterial(_textureName);
  // insert the game object into the vector of game objects
  SPtr<GameObject> gObject = createGameObject();
  // add the components and set the transform
  gObject->setTransform(_transform);
  gObject->addComponent(model);
  gObject->addComponent(pMat);
  /**
  * set the game objects parent as a null, it will be reassigned if the gameObject is supposed
  * to have a parent
  **/
  gObject->parent = nullptr;
  // if the parent is not a nullptr (there is a parent that will have this game object)
  if (_pParent) {
    // insert the current gameObject to the children vector of the parent
    insertGameObject(gObject, _pParent->children);
    // set the parent as the parent of the current game Object
    gObject->parent = _pParent;
  }
  // otherwise, the object is part of the scene
  else { insertGameObject(gObject, gameObjects); }
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
              Vector4(0.0f, 1.0f, 0.0f, 1.0f),
              Vector4(0.0f, 1.0f, 0.0f, 0.0f));
  newGameObject("Shadow_Leviathan_anim.fbx", "Shadow_Leviathan_01.png", Matrix4::IDENTITY);
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
  // last cursor position
  Vector2 lastCursorPos = eventQueue.mousePosition;
  // get the starting deltaTime
  high_resolution_clock::time_point delta = high_resolution_clock::now();
  // event loop, while the escape key has not been pressed
  while (!eventQueue.iskeyPressed(KEY::kEsc))
  {
    // event window specific input
    eventQueue.windowInput();
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
    if (eventQueue.iskeyPressed(KEY::kE) ||
        eventQueue.iskeyPressed(KEY::kSpace)) {
      camera.move(Vector3(0.0f, camSpeed, 0.0f));
    }
    if (eventQueue.iskeyPressed(KEY::kQ) ||
        eventQueue.iskeyPressed(KEY::kLControl)) {
      camera.move(Vector3(0.0f, -camSpeed, 0.0f));
    }
    // rotate world
    if (eventQueue.iskeyPressed(KEY::kLeft)) {
      camera.rotate(0.0f, 1.0f * deltaTime, 0.0f);
    }
    if (eventQueue.iskeyPressed(KEY::kRight)) {
      camera.rotate(0.0f, -1.0f * deltaTime, 0.0f);
    }
    if (eventQueue.iskeyPressed(KEY::kDown)) {
      camera.rotate(1.0f * deltaTime, 0.0f, 0.0f);
    }
    if (eventQueue.iskeyPressed(KEY::kUp)) {
      camera.rotate(-1.0f * deltaTime, 0.0f, 0.0f);
    }
    // mouse input
    if (eventQueue.iskeyPressed(KEY::kLButton)) {
      Vector2 posDif = (lastCursorPos - eventQueue.mousePosition) * deltaTime;
      camera.rotate(-posDif.y, posDif.x, 0.0f);
      camera.addRotation(Vector3(-posDif.y, posDif.x, 0.0f));
      lastCursorPos = eventQueue.mousePosition;
    }
    else {
      lastCursorPos = eventQueue.mousePosition;
    }
    if (eventQueue.iskeyPressed(KEY::kRButton)) {}
    if (eventQueue.iskeyPressed(KEY::kCButton)) {}
    // backspace input
    if (eventQueue.iskeyPressed(KEY::kBackSpace)) {
      // if the game object pool is not empty
      if (!gameObjects.empty()) {
        // pop the last gameobject
        gameObjects.pop_back();
      }
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

  // Update view
  CBView viewBuffer = CBView();
  viewBuffer.view = _pCamera->view.getTransposed();
  api.updateConstantBuffer(cBView, &viewBuffer, 0);

  // Update projection
  CBProjection projectionBuffer = CBProjection();
  projectionBuffer.projection = _pCamera->projection.getTransposed();
  api.updateConstantBuffer(cBProjection, &projectionBuffer, 0);
}

void
BaseApp::setGameObjectsBuffers()
{
  // get the api
  GraphicsAPI& api = GraphicsAPI::instance();

  // for each game object in the world
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
  // get the current time
  high_resolution_clock::time_point end = high_resolution_clock::now();
  // subtract the previous time to the current time to get the difference (delta time)
  float deltaTime = duration<float>(end - _delta).count();
  // previous time is now the current time
  _delta = high_resolution_clock::now();
  // return the difference
  return deltaTime;
}

void
BaseApp::render()
{
  // get the api instance
  GraphicsAPI& api = GraphicsAPI::instance();
  // screen clear color
  float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
  api.clearRenderTargetView(clearColor);
  api.clearDepthBuffer(1.0f);
  // update the light buffer
  api.updateConstantBuffer(cbLight, &light, static_cast<uint32>(sizeof(Light)));
  // Set shaders
  api.setShaders();
  // set light
  light.Type = LIGHT_TYPE::kDirectional;
  light.LightDir = Vector3::FORWARD;
  // render the objects
  renderGameObjects(gameObjects);
  // present the final result to the screen
  api.present(1, 0);
}

void
BaseApp::renderGameObjects(Vector<SPtr<GameObject>> _gameObjects)
{
  // set constant buffers for the pixel and vertex shaders
  VSSetConstantBuffers();
  PSSetConstantBuffers();
  // get a reference from the api
  GraphicsAPI& api = GraphicsAPI::instance();
  // for each Game Object
  for (uint32 i = 0; i < _gameObjects.size(); ++i) {
    // Get the final matrix by taking into account the parent opjects
    SPtr<GameObject> parent = _gameObjects[i]->parent;
    Matrix4 transform = _gameObjects[i]->transform;
    // while there's a parent
    while (parent) {
      // add the parent transform to the current transform matrix
      transform += parent->transform;
      // the next parent will be the parent of this parent
      parent = parent->parent;
    }
    // set the current gameObject transform as the world in which the shader will work in
    api.updateConstantBuffer(cBWorld, &transform, static_cast<uint32>(sizeof(CBWorld)));
    // set the diffuse texture to the resource view
    api.setShaderResourceView(_gameObjects[i]->getComponent<Material>()->diffuse);
    api.setSampler();
    // render the model component
    renderModel(*_gameObjects[i]->getComponent<Model>());
    // if the game object has children, do the same for them
    if (!_gameObjects[i]->children.empty()) {
      renderGameObjects(_gameObjects[i]->children);
    }
  }
}

void
BaseApp::renderModel(Model& _model)
{
  // get a reference from the api
  GraphicsAPI& api = GraphicsAPI::instance();
  api.setInputLayout();
  api.setVertexBuffer(_model.vertexB);
  api.setIndexBuffer(_model.indexB);
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