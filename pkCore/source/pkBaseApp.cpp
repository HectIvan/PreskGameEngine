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
using pkEngineSDK::PASS_TYPE::kP_Tone;
using pkEngineSDK::D_BUFFERS::kDB_Base;
using pkEngineSDK::D_BUFFERS::kDB_Shadow;
using pkEngineSDK::G_BUFFERS::kGB_Albedo;
using pkEngineSDK::G_BUFFERS::kGB_Normal;
using pkEngineSDK::G_BUFFERS::kGB_Shadow;

namespace pkEngineSDK
{

void
run(String _name, Window& _window);

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
  g_RenderManager().init();
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
  // get managers
  GraphicsAPI& api = g_GraphicAPI().instance();
  RendererManager& renderManager = g_RenderManager().instance();
  // first shadow pass
  renderManager.getPass(PASS_TYPE::kP_Shadow)->beginPass();
  renderManager.renderActors(g_SceneManager().getActiveScene()->getAllActors());
  renderManager.getPass(PASS_TYPE::kP_Shadow)->endPass();
  // base pass
  renderManager.getPass(PASS_TYPE::kP_Base)->beginPass();
  renderManager.renderActors(g_SceneManager().getActiveScene()->getAllActors());
  renderManager.getPass(PASS_TYPE::kP_Base)->endPass();
  // Quad luminance pass
  renderManager.getPass(PASS_TYPE::kP_Luminance)->beginPass();
  api.draw(3, 0);
  renderManager.getPass(PASS_TYPE::kP_Luminance)->endPass();
  // Quad shadow pass
  renderManager.getPass(PASS_TYPE::kP_ShadowDef)->beginPass();
  api.draw(3, 0);
  renderManager.getPass(PASS_TYPE::kP_ShadowDef)->endPass();
  // Quad tone map pass
  renderManager.getPass(PASS_TYPE::kP_Tone)->beginPass();
  api.draw(3, 0);
  // Scene specific app render
  onRender();
  // Present the final result to the screen
  g_GraphicAPI().present(m_vSync, 0);
}
}