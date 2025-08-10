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

using pkEngineSDK::D_BUFFERS::kDB_Base;
using pkEngineSDK::D_BUFFERS::kDB_Light;
using pkEngineSDK::G_BUFFERS::kGB_Albedo;
using pkEngineSDK::G_BUFFERS::kGB_Normal;
using pkEngineSDK::G_BUFFERS::kGB_Shadow;
using pkEngineSDK::PASS_TYPE::kP_Base;
using pkEngineSDK::PASS_TYPE::kP_EmissiveHBlur;
using pkEngineSDK::PASS_TYPE::kP_EmissiveBlur;
using pkEngineSDK::PASS_TYPE::kP_IBR;
using pkEngineSDK::PASS_TYPE::kP_Shadow;
using pkEngineSDK::PASS_TYPE::kP_ShadowQuad;
using pkEngineSDK::PASS_TYPE::kP_SkyBox;
using pkEngineSDK::PASS_TYPE::kP_Tone;
using pkEngineSDK::PASS_TYPE::kP_Merge;
using pkEngineSDK::PASS_TYPE::kP_Luminance;
using pkEngineSDK::PASS_TYPE::kP_LumBlurH;
using pkEngineSDK::PASS_TYPE::kP_LumBlur;

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
  EventQueue::startUp();

  initWin();
  initAPI(_argv, _count);

  g_SceneManager().init();
  g_RenderManager().init();
  g_TextureManager().loadDefaultMatTextures();

  onInit();
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
  EventQueue& eventQueue = g_eventManager();
  // get the starting deltaTime
  high_resolution_clock::time_point delta = high_resolution_clock::now();

  while (m_run) {
    // reset scroll wheel input
    eventQueue.scrollWheel = 0;
    // event window specific input
    eventQueue.windowInput(m_window);
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
    eventQueue.poll();
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
  // get all passes
  SPtr<Pass> baseShadow = renderManager.getPass(kP_Shadow);
  SPtr<Pass> basePass = renderManager.getPass(kP_Base);
  SPtr<Pass> shadowQuad = renderManager.getPass(kP_ShadowQuad);
  SPtr<Pass> skyBoxPass = renderManager.getPass(kP_SkyBox);
  SPtr<Pass> IBRPass = renderManager.getPass(kP_IBR);
  SPtr<Pass> emissHBlurPass = renderManager.getPass(kP_EmissiveHBlur);
  SPtr<Pass> emissBlurPass = renderManager.getPass(kP_EmissiveBlur);
  SPtr<Pass> mergePass = renderManager.getPass(kP_Merge);
  SPtr<Pass> lumPass = renderManager.getPass(kP_Luminance);
  SPtr<Pass> lumBlurHPass = renderManager.getPass(kP_LumBlurH);
  SPtr<Pass> lumBlurPass = renderManager.getPass(kP_LumBlur);

  SPtr<Pass> tonePass = renderManager.getPass(kP_Tone);
  // Get all actors
  Vector<SPtr<Actor>> actors = g_SceneManager().getActiveScene()->getAllActors();

  // first shadow pass
  if (m_shadows) {
    baseShadow->beginPass();
    renderManager.renderActors(actors);
    baseShadow->endPass();
  }

  // base pass
  basePass->beginPass(Color(0, 0, 0, 0));
  renderManager.renderActors(actors);
  basePass->endPass();

  api.clearRenderTargetViews(Color(1, 1, 1, 1), shadowQuad->getOutputTextures());
  // get texel size of compute passes
  //        Vector2 texSize = api.getSwapChain()->getSize();
  //        uint32 threadWidth = 16;
  //        uint32 threadHeight = 16;
  //        uint32 x = static_cast<uint32>((texSize.x + threadWidth - 1) / threadWidth);
  //        uint32 y = static_cast<uint32>((texSize.y + threadHeight - 1) / threadHeight);
  // if shadows are set to be rendered
  shadowQuad->beginPass();
  api.draw(3, 0);
  shadowQuad->endPass();

  // render the skybox
  skyBoxPass->beginPass();
  api.draw(3, 0);
  skyBoxPass->endPass();
  // IBR Pass.
  api.clearRenderTargetViews(Color(0.0f, 0.0f, 0.0f), IBRPass->getOutputTextures());
  if (m_IBR) {
    IBRPass->beginPass();
    api.draw(3, 0);
    IBRPass->endPass();
  }

  // emissive Horizontal Blur pass
  emissHBlurPass->beginPass();
  api.draw(3, 0);
  emissHBlurPass->endPass();

  // emissive pass
  emissBlurPass->beginPass();
  api.draw(3, 0);
  emissBlurPass->endPass();

  // Quad merge pass.
  mergePass->beginPass();
  api.draw(3, 0);
  mergePass->endPass();

  // Quad luminosity pass.
  lumPass->beginPass();
  api.draw(3, 0);
  lumPass->endPass();

  // quad lum blur Horizontal pass.
  lumBlurHPass->beginPass();
  api.draw(3, 0);
  lumBlurHPass->endPass();

  // Quad lum blur Vertical pass.
  lumBlurPass->beginPass();
  api.draw(3, 0);
  lumBlurPass->endPass();

  // Quad Tone pass
  tonePass->beginPass();
  api.draw(3, 0);
  // Scene specific app render
  onRender();
  tonePass->endPass();
  // Present the final result to the screen
  g_GraphicAPI().present(m_vSync, 0);
}
}
