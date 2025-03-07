#include "pkBaseApp.h"
#include "pkDebug.h"
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
BaseApp::createMaterial()
{
  return g_ResourceManager().newMaterial();
}

SPtr<Texture>
BaseApp::createTexture(String _name)
{
  // search if the texture has been stored before
  for (uint32 i = 0; i < m_textures.size(); ++i) {
    if (m_textures[i]->name == _name) {
      return m_textures[i]->texture;
    }
  }

  // create the texture
  SPtr<Texture> texture = g_ResourceManager().newTexture(_name);

  // store the new texture in the memory
  SPtr<TextureMemory> newTexture = make_shared<TextureMemory>();
  newTexture->name = _name;
  newTexture->texture = texture;
  m_textures.push_back(newTexture);

  // return the final texture
  return texture;
}

SPtr<Model>
BaseApp::newModel(String _modelName)
{
  // search if the model has been stored before
  for (uint32 i = 0; i < m_models.size(); ++i) {
    if (m_models[i]->name == _modelName) {
      return m_models[i]->model;
    }
  }
  // load the model.
  SPtr<Model> model = g_ResourceManager().loadModel(_modelName);
  // create a material.
  for (uint32 i = 0; i < model->meshes.size(); ++i) {
    model->meshes[i].material = createMaterial();
    model->meshes[i].material->diffuse = createTexture(model->meshes[i].materialPath);
  }
  // insetr the new model to the model memory.
  SPtr<ModelMemory> newModelMem = make_shared<ModelMemory>();
  newModelMem->name = _modelName;
  newModelMem->model = model;
  m_models.push_back(newModelMem);

  // return the model
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
  RendererManager::startUp();
  ResourceManager::startUp();
  TimeManager::startUp();
  
  initWindow();
  initAPI(_argv);

  createBuffers();
  g_RenderManager().init(m_window);
  /*g_GraphicAPI().makeShaders();
  g_GraphicAPI().compileShaders();
  g_GraphicAPI().createShaders();
  g_GraphicAPI().createInputLayoutFromVShader();
  g_GraphicAPI().createSamplerState();*/
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

#if PK_DEBUG_MODE
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
  g_RenderManager().m_cBView = g_GraphicAPI().createConstantBuffer(static_cast<uint32>(sizeof(CBView)), nullptr, 0);
  g_RenderManager().m_cBProjection = g_GraphicAPI().createConstantBuffer(static_cast<uint32>(sizeof(CBProjection)), nullptr, 0);
  g_RenderManager().m_cBWorld = g_GraphicAPI().createConstantBuffer(static_cast<uint32>(sizeof(CBWorld)), nullptr, 0);
  g_RenderManager().m_cbLight = g_GraphicAPI().createConstantBuffer(static_cast<uint32>(sizeof(Light)), nullptr, 0);
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
  while (!m_eventQueue.iskeyPressed(KEY::kEsc)) {
    // event window specific input
    m_eventQueue.windowInput();
    // update the delta time
    m_deltaTime = g_TimeManager().getDeltaTime(delta);
    g_TimeManager().m_deltaTime = g_TimeManager().getDeltaTime(delta);
    // fixed update timer count.
    m_fixedTimer += m_deltaTime;
    // child class app update
    onUpdate(m_deltaTime);
    // update game objects
    update(m_scene, m_deltaTime);
    if (m_fixedTimer > 0.016f) {
      // fixed update
      fixedUpdate();
      m_fixedTimer = 0;
    }
    // event queue
    m_eventQueue.poll();
    // update camera
    g_RenderManager().updateCameraBuffers(&m_camera);
    // render the scene
    render(m_scene);
  }
}

void
BaseApp::render(Scene& _scene)
{
  g_RenderManager().render(_scene);
  // on scene specific app render
  onRender(_scene);
  // present the final result to the screen
  g_GraphicAPI().present(1, 0);
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
}