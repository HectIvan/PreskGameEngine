#include "pkBaseApp.h"
#include "pkLogger.h"
#include "pkDllLoader.h"
#include "pkGraphicsAPI.h"
#include "pkLight.h"
#include "pkModel.h"
#include "pkMath.h"
#include "pkPrerequisitesCore.h"
#include "pkSceneManager.h"
#include "pkSprite.h"
#include "pkTextureManager.h"
#include "pkTimeManager.h"
#include "pkWindowDesc.h"

using pkEngineSDK::PASS_TYPE::kP_AO;
using pkEngineSDK::PASS_TYPE::kP_Base;
using pkEngineSDK::PASS_TYPE::kP_Shadow;
using pkEngineSDK::PASS_TYPE::kP_ShadowDef;
using pkEngineSDK::PASS_TYPE::kP_Test;
using pkEngineSDK::D_BUFFERS::kDB_Base;
using pkEngineSDK::D_BUFFERS::kDB_Shadow;
using pkEngineSDK::G_BUFFERS::kGB_Albedo;
using pkEngineSDK::G_BUFFERS::kGB_Normal;

namespace pkEngineSDK
{

void
run(String _name, Window& _window);

SPtr<Material>
BaseApp::createMaterial()
{
  return g_ResourceManager().newMaterial();
}

// to do: transfer this to a resource manager
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
BaseApp::init(const char** _argv, int32 _count)
{
  Logger::startUp();
  RendererManager::startUp();
  ResourceManager::startUp();
  SceneManager::startUp();
  TextureManager::startUp();
  TimeManager::startUp();

  initWindow();
  initAPI(_argv, _count);

  g_SceneManager().init();

  g_RenderManager().init(m_window);
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
BaseApp::initAPI(const char** _argv, int32 _count)
{
  String abstraction = "DX11APId";
  if (_count > 1) {
    abstraction = _argv[1]; // _argv[0] is always the executable path
  }

#if PK_DEBUG_MODE
  if (abstraction == "DX11APId") {
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
BaseApp::messageLoop()
{
  // get the starting deltaTime
  high_resolution_clock::time_point delta = high_resolution_clock::now();
  // event loop, while the escape key has not been pressed
  while (!m_eventQueue.iskeyPressed(KEY::kEsc)) {
    // event window specific input
    m_eventQueue.windowInput(m_window);
    // update the delta time
    // m_deltaTime = g_TimeManager().getDeltaTime(delta);
    g_TimeManager().m_deltaTime = g_TimeManager().getDeltaTime(delta);
    // fixed update timer count.
    m_fixedTimer += g_TimeManager().m_deltaTime;
    // child class app update
    onUpdate();
    // update scene
    if (m_fixedTimer > 0.016f) {
      // fixed update
      fixedUpdate();
      g_SceneManager().getActiveScene()->update(g_TimeManager().m_deltaTime);
      m_fixedTimer = 0;
    }
    else {
      g_SceneManager().getActiveScene()->update(g_TimeManager().m_fixedDeltaTime);
    }
    // event queue
    m_eventQueue.poll();
    // render the scene
    render();
  }
}

void
BaseApp::render()
{
  // screen clear color
  float clearColor[4] = { 0.0f, 0.123f, 0.3f, 1.0f };
  // get managers
  GraphicsAPI& api = g_GraphicAPI().instance();
  RendererManager& renderManager = g_RenderManager().instance();
  /**
   * Normal Render.
   */
   // clear the render targets
   // to do: change this to the render targets created in the renderer
  SPtr<Pass> currentPass = renderManager.getPass(kP_Base);
  api.clearRenderTargetView(clearColor, renderManager.getGBuffer(kGB_Albedo));
  api.clearRenderTargetView(clearColor, renderManager.getGBuffer(kGB_Normal));
  api.clearDepthBuffer(1.0f, renderManager.getDepthBuffer(kDB_Base));

  // set the render targets
  api.setRenderTargets(renderManager.getGBuffers(),
    renderManager.getDepthBuffer(kDB_Base));
  api.setInputLayout(currentPass->getInputLayout());
  // set the base pass for the first rendering stage
  api.setPSShader(currentPass->getPShader());
  api.setVSShader(currentPass->getVShader());
  api.setSampler(currentPass->getSamplerState());

  // set constant buffers for the pixel and vertex shaders
  renderManager.PSSetConstantBuffers(currentPass->getCBuffers());
  renderManager.VSSetConstantBuffers(currentPass->getCBuffers());
  // render the objects
  renderManager.renderActors(g_SceneManager().getActiveScene()->getAllActors());
  api.setRenderTarget(nullptr);

  /**
   * Deferred render test
   */
   // get the current pass
  currentPass = renderManager.getPass(kP_Test);
  // clear the back buffer
  api.clearRenderTargetView(clearColor, api.getSwapChain()->getBuffer(0));
  // set the back buffer as render target
  api.setRenderTarget(api.getSwapChain()->getBuffer(0));
  api.setInputLayout(nullptr);
  // set the shaders needed
  api.setVSShader(currentPass->getVShader());
  api.setPSShader(currentPass->getPShader());
  api.setSampler(currentPass->getSamplerState());
  // set the resources for the render
  api.PSSetShaderResourceView(renderManager.getGBuffer(kGB_Albedo));
  // draw in deferred
  api.draw(3, 0);
  // Scene specific app render
  onRender();
  // Present the final result to the screen
  g_GraphicAPI().present(m_vSync, 0);
}
}