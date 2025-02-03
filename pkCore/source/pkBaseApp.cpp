#include <iostream>

#include "pkBaseApp.h"
#include "pkDllLoader.h"
#include "pkGameObject.h"
#include "pkGraphicsAPI.h"
#include "pkLight.h"
#include "pkModel.h"
#include "pkMath.h"
#include "pkPrerequisitesCore.h"
#include "pkSprite.h"
#include "pkWindowDesc.h"

namespace pkEngineSDK
{

void
run(String _name, Window& _window);

SPtr<Material>
BaseApp::newMaterial(String _textureName)
{
  // get api instance.
  GraphicsAPI& api = GraphicsAPI::instance();
  // create the texture adress.
  String textureName = "textures/" + _textureName;
  // create the texture.
  SPtr<Texture> texture = api.createTextureFromFile(textureName, 8, false, 28);
  // if creating the texture failed, return the model without a texture.
  if (!texture) { return nullptr; }
  // create the material component.
  SPtr<Material> pMatComp = make_shared<Material>();
  pMatComp->setTexture(pMatComp->diffuse, texture);
  // return the texture.
  return pMatComp;
}

SPtr<Model>
BaseApp::newModel(String _modelName)
{
  // load the model.
  SPtr<Model> model = loadModel(_modelName);
  // create a material.
  for (uint32 i = 0; i < model->meshes.size(); ++i) {
    model->meshes[i].material = newMaterial(model->meshes[i].materialPath);
  }
  return model;
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
  m_cameraSpeed = 5.0f;
  onInit();
}

void
BaseApp::initWindow()
{
  PKWindowDesc desc;
  desc.width = 1920;
  desc.height = 1080;
  std::string name = "Game Engine Window";
  m_window.create(desc, name);
}

void
BaseApp::initAPI(const char** _argv)
{
  String abstraction = _argv[1];

#ifdef PK_DEBUG_MODE
  if (abstraction == "DX11API") {
    run("pkDX11APId", m_window);
  }
#else
  if (abstraction == "DX11API") {
    run("pkDX11API", m_window);
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
  m_cBView = api.createConstantBuffer(static_cast<uint32>(sizeof(CBView)), nullptr, 0);
  m_cBProjection = api.createConstantBuffer(static_cast<uint32>(sizeof(CBProjection)), nullptr, 0);
  m_cBWorld = api.createConstantBuffer(static_cast<uint32>(sizeof(CBWorld)), nullptr, 0);
  m_cbLight = api.createConstantBuffer(static_cast<uint32>(sizeof(Light)), nullptr, 0);
}


float
getDeltaTime(high_resolution_clock::time_point& _delta)
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
updateActor(SPtr<Actor>& _pActor, float _deltaTime)
{
  _pActor->update(_deltaTime);
  for (uint32 i = 0; i < _pActor->m_children.size(); ++i) {
    updateActor(_pActor->m_children[i], _deltaTime);
  }
}

void
update(Scene& _scene, float _deltaTime)
{
  for (uint32 i = 0; i < _scene.m_actors.size(); ++i) {
    if (_scene.m_actors[i]->m_active) {
      updateActor(_scene.m_actors[i], _deltaTime);
    }
  }
}

void
BaseApp::messageLoop()
{
  // get the starting deltaTime
  high_resolution_clock::time_point delta = high_resolution_clock::now();
  // event loop, while the escape key has not been pressed
  while (!m_eventQueue.iskeyPressed(KEY::kEsc))
  {
    // event window specific input
    m_eventQueue.windowInput();
    // update the delta time
    float deltaTime = getDeltaTime(delta);
    // fixed update timer count.
    m_fixedTimer += deltaTime;
    // child class app update
    onUpdate(deltaTime);
    // update game objects
    update(m_scene, deltaTime);
    if (m_fixedTimer > 0.016f) {
      // fixed update
      fixedUpdate();
      m_fixedTimer = 0;
    }
    // event queue
    m_eventQueue.poll();
    // update camera
    updateCamera(&m_camera);
    // render the scene
    render();
  }
}

SPtr<Actor>
BaseApp::actorFind(String _objectName)
{
  // for each game object in the list
  for (uint32 i = 0; i < m_scene.m_actors.size(); ++i) {
    // check if the name is the one we're looking for
    if (m_scene.m_actors[i]->m_name == _objectName) {
      return m_scene.m_actors[i];
    }
  }
  // if no game object fits the name
  return nullptr;
}

template<typename T>
SPtr<Actor>
BaseApp::getActorWithComponent()
{
  // check each game object
  for (uint32 i = 0; i < m_scene.m_actors.size(); ++i) {
    // check if the data type return is not null
    SPtr<T> check = m_scene.m_actors[i]->getComponent<T>();
    if (check) {
      // if its not null, return the final value
      return m_scene.m_actors[i];
    }
  }
}

template<typename T>
Vector<SPtr<Actor>>
BaseApp::getAllActorsWithComponent()
{
  // game object list
  Vector<SPtr<GameObject>> list;
  // check each game object
  for (uint32 i = 0; i < m_scene.m_actors.size(); ++i) {
    // check if the data type return is not null
    SPtr<T> check = m_scene.m_actors[i]->getComponent<T>();
    if (check) {
      // if its not null, return the final value
      list.push_back(m_scene.m_actors[i]);
    }
  }
  return list;
}

void
BaseApp::updateCamera(Camera* _pCamera)
{
  // get the api instance to work with
  GraphicsAPI& api = GraphicsAPI::instance();

  // Update view
  CBView viewBuffer = CBView();
  viewBuffer.view = _pCamera->view.getTransposed();
  api.updateConstantBuffer(m_cBView, &viewBuffer, 0);

  // Update projection
  CBProjection projectionBuffer = CBProjection();
  projectionBuffer.projection = _pCamera->projection.getTransposed();
  api.updateConstantBuffer(m_cBProjection, &projectionBuffer, 0);
}

void
BaseApp::setActorsBuffers()
{
  // get the api
  GraphicsAPI& api = GraphicsAPI::instance();

  // for each actor in the world
  for (uint32 i = 0; i < m_scene.m_actors.size(); ++i) {
    /**
     * Cast to a gameObject, if it fails, do none of the following process
     */
    SPtr<GameObject> gameObject = actorToClass<GameObject>(m_scene.m_actors[i]);
    if (gameObject) {
      // for each model in the game object
      for (uint32 j = 0; j < gameObject->models.size(); ++j) {
        // set its vertex and index buffers
        api.setVertexBuffer(gameObject->models[j]->vertexB);
        api.setIndexBuffer(gameObject->models[j]->indexB); ;
      }
    }
  }
}

void
BaseApp::VSSetConstantBuffers()
{
  // get the api instance
  GraphicsAPI& api = GraphicsAPI::instance();

  // set the constant buffers
  api.VSSetConstantBuffer(m_cBView, 0, 1);
  api.VSSetConstantBuffer(m_cBProjection, 1, 1);
  api.VSSetConstantBuffer(m_cBWorld, 2, 1);
  api.VSSetConstantBuffer(m_cbLight, 3, 1);
}

void
BaseApp::PSSetConstantBuffers()
{
  // get the api instance
  GraphicsAPI& api = GraphicsAPI::instance();

  // set the constant buffers
  api.PSSetConstantBuffer(m_cBView, 0, 1);
  api.PSSetConstantBuffer(m_cBProjection, 1, 1);
  api.PSSetConstantBuffer(m_cBWorld, 2, 1);
  api.PSSetConstantBuffer(m_cbLight, 3, 1);
}

void
BaseApp::render()
{
  // on render child specific function call
  onRender();
  // get the api instance
  GraphicsAPI& api = GraphicsAPI::instance();
  // screen clear color
  float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
  api.clearRenderTargetView(clearColor);
  api.clearDepthBuffer(1.0f);
  // update the light buffer
  api.updateConstantBuffer(m_cbLight, &light, static_cast<uint32>(sizeof(Light)));
  // Set shaders
  api.setShaders();
  // set light
  light.Type = LIGHT_TYPE::kDirectional;
  light.LightDir = Vector3::FORWARD;
  // set constant buffers for the pixel and vertex shaders
  VSSetConstantBuffers();
  PSSetConstantBuffers();
  // render the objects
  renderActors(m_scene.m_actors);
  // present the final result to the screen
  api.present(1, 0);
}

void
renderModel(Model& _model)
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

template<typename T>
SPtr<T>
BaseApp::actorToClass(SPtr<Actor>& _subject)
{
  // cast to a game actor
  SPtr<T> aTC = reinterpret_pointer_cast<T>(_subject);
  // casting was successful
  if (aTC) { return aTC; }
  // casting failed
  return nullptr;
}

void
BaseApp::renderActors(Vector<SPtr<Actor>> _gameActors)
{
  // get a reference from the api
  GraphicsAPI& api = GraphicsAPI::instance();
  // for each actor
  for (uint32 i = 0; i < _gameActors.size(); ++i) {
    if (!_gameActors[i]->m_active) {
      continue;
    }
    // Get the final matrix by taking into account the parent actors
    SPtr<Actor> parent = _gameActors[i]->m_parent;
    Matrix4 transform = _gameActors[i]->m_transform;
    // while there's a parent
    while (parent) {
      // add the parent transform to the current transform matrix
      transform *= parent->m_transform;
      // the next parent will be the parent of this parent
      parent = parent->m_parent;
    }
    // set the current actor transform as the world in which the shader will work in
    api.updateConstantBuffer(m_cBWorld, &transform, static_cast<uint32>(sizeof(CBWorld)));

    /**
     * Recast to a gameobject. If it fails, do none of this
     */
    SPtr<GameObject> gameObject = actorToClass<GameObject>(_gameActors[i]);
    if (gameObject) {
      // set the diffuse texture to the resource view if the model has a material
      if (gameObject->getComponent<Material>()) {
        // set the material texture to the shader
        api.setShaderResourceView(gameObject->getComponent<Material>()->diffuse);
        api.setSampler();
      }
      // render the model component
      renderModel(*gameObject->getComponent<Model>());
    }
     // if the actor has children, do the same for them
    if (!_gameActors[i]->m_children.empty()) {
      renderActors(_gameActors[i]->m_children);
    }
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
  if (!model->index.empty()) {
    // create the index and vertex buffers
    model->vertexB = api.createVertexBuffer(model->vertex);
    model->indexB = api.createIndexBuffer(model->index);
    api.setIndexBuffer(model->indexB);
    api.setVertexBuffer(model->vertexB);
  }
  // return the final model
  return model;
}
}