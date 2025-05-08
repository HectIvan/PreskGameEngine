#include "pkBaseApp.h"
#include "pkLogger.h"
#include "pkDllLoader.h"
#include "pkGameObject.h"
#include "pkGraphicsAPI.h"
#include "pkLight.h"
#include "pkModel.h"
#include "pkMath.h"
#include "pkPrerequisitesCore.h"
#include "pkSprite.h"
#include "pkTextureManager.h"
#include "pkTimeManager.h"
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
  Logger::startUp();
  RendererManager::startUp();
  ResourceManager::startUp();
  Scene::startUp();
  TextureManager::startUp();
  TimeManager::startUp();

  initWindow();
  initAPI(_argv);

  g_RenderManager().init(m_window);
  m_cameraSpeed = 20.0f;
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
  String abstraction = "DX11API";//  _argv[1];

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
update(float _deltaTime)
{
  for (uint32 i = 0; i < g_sceneManager().getAllActors().size(); ++i) {
    if (g_sceneManager().getActor(i)->m_active) {
      SPtr<Actor> actor = g_sceneManager().getActor(i);
      updateActor(actor, _deltaTime);
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
    // m_deltaTime = g_TimeManager().getDeltaTime(delta);
    g_TimeManager().m_deltaTime = g_TimeManager().getDeltaTime(delta);
    // fixed update timer count.
    m_fixedTimer += g_TimeManager().m_deltaTime;
    // child class app update
    onUpdate();
    // update game objects
    update(g_TimeManager().m_deltaTime);
    if (m_fixedTimer > 0.016f) {
      // fixed update
      fixedUpdate();
      m_fixedTimer = 0;
    }
    // event queue
    m_eventQueue.poll();
    // update camera
    // g_RenderManager().updateCameraBuffers(&m_camera);
    // render the scene
    render();
  }
}

void
BaseApp::render()
{
  g_RenderManager().render();
  // on scene specific app render
  onRender();
  // present the final result to the screen
  g_GraphicAPI().present(1, 0);
}

SPtr<Actor>
BaseApp::actorFind(String _objectName)
{
  // for each game object in the list
  for (uint32 i = 0; i < g_sceneManager().getAllActors().size(); ++i) {
    // check if the name is the one we're looking for
    if (g_sceneManager().getActor(i)->m_name == _objectName) {
      return g_sceneManager().getActor(i);
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
  for (uint32 i = 0; i < g_sceneManager().getAllActors().size(); ++i) {
    // check if the data type return is not null
    SPtr<T> check = g_sceneManager().getActor(i)->getComponent<T>();
    if (check) {
      // if its not null, return the final value
      return g_sceneManager().getActor(i);
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
  for (uint32 i = 0; i < g_sceneManager().getAllActors().size(); ++i) {
    // check if the data type return is not null
    SPtr<T> check = g_sceneManager().getActor(i)->getComponent<T>();
    if (check) {
      // if its not null, return the final value
      list.push_back(g_sceneManager().getActor(i));
    }
  }
  return list;
}
}