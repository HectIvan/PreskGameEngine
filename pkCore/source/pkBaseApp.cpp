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
using pkEngineSDK::PASS_TYPE::kP_AO;
using pkEngineSDK::PASS_TYPE::kP_Base;
using pkEngineSDK::PASS_TYPE::kP_CShadows;
using pkEngineSDK::PASS_TYPE::kP_CSpecular;
using pkEngineSDK::PASS_TYPE::kP_CHBlur;
using pkEngineSDK::PASS_TYPE::kP_Luminance;
using pkEngineSDK::PASS_TYPE::kP_Shadow;
using pkEngineSDK::PASS_TYPE::kP_ShadowDef;
using pkEngineSDK::PASS_TYPE::kP_SkyBox;
using pkEngineSDK::PASS_TYPE::kP_Tone;
using pkEngineSDK::PASS_TYPE::kP_CVBlur;

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

  // SPtr<Actor> skybox = g_SceneManager().getActiveScene()->instantiate("SkyBox");
  // skybox->addComponent(g_ResourceManager().loadModel(Path("models/cube.obj")));
  // skybox->setScale(2000.0f);
  // skybox->setPosition(0.0f, -100.0f, 0.0f);
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
  // event loop, while the escape key has not been pressed
  while (!eventQueue.iskeyPressed(KEY::kEsc)) {
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
  SPtr<Pass> luminancePass = renderManager.getPass(kP_Luminance);
  SPtr<Pass> hBlurPass = renderManager.getPass(kP_CHBlur);
  SPtr<Pass> tonePass = renderManager.getPass(kP_Tone);
  SPtr<Pass> pCShadowPass = renderManager.getPass(kP_CShadows);
  SPtr<Pass> pCSpecPass = renderManager.getPass(kP_CSpecular);
  SPtr<Pass> skyBoxPass = renderManager.getPass(kP_SkyBox);

  // first shadow pass
  if (m_shadows) {
    baseShadow->beginPass();
    renderManager.renderActors(g_SceneManager().getActiveScene()->getAllActors());
    baseShadow->endPass();
  }
  // base pass
  basePass->beginPass();
  renderManager.renderActors(g_SceneManager().getActiveScene()->getAllActors());
  basePass->endPass();
  // Quad luminance pass
  if (m_luminance) {
    luminancePass->beginPass();
    api.draw(3, 0);
    luminancePass->endPass();
  }
  api.clearUnorderedAccessViews(pCSpecPass->getUAVTextures(), Color(0, 0, 0, 0));
  api.clearUnorderedAccessViews(hBlurPass->getUAVTextures(), Color(0, 0, 0, 0));
  api.clearUnorderedAccessViews(pCShadowPass->getUAVTextures());
  // get texel size of compute passes
  Vector2 texSize = api.getSwapChain()->getSize();
  uint32 threadWidth = 16;
  uint32 threadHeight = 16;
  uint32 x = static_cast<uint32>((texSize.x + threadWidth - 1) / threadWidth);
  uint32 y = static_cast<uint32>((texSize.y + threadHeight - 1) / threadHeight);
  // if shadows are set to be rendered
  if (m_shadows) {
    pCShadowPass->beginPass();
    api.dispatch(x, y, 1);
    pCShadowPass->endPass();
  }
  // if specular is set to be rendered
  if (m_specular) {
    pCSpecPass->beginPass();
    api.dispatch(x, y, 1);
    pCSpecPass->endPass();
    // horizontal blur quad pass
    hBlurPass->beginPass();
    api.dispatch(x, y, 1);
    hBlurPass->endPass();
  }

  // render the skybox
  //        skyBoxPass->beginPass();
  //        SPtr<Actor> skybox = g_SceneManager().getActiveScene()->actorFind("SkyBox");
  //        SPtr<VertexBuffer> vB = skybox->getComponent<Model>()->getVertexBuffer();
  //        SPtr<IndexBuffer> iB = skybox->getComponent<Model>()->getIndexBuffer();
  //        api.setVertexBuffer(vB);
  //        api.setIndexBuffer(iB);
  //        api.draw(3, 0);
  //        skyBoxPass->endPass();
  // Quad tone map pass
  tonePass->beginPass();
  api.draw(3, 0);
  // Scene specific app render
  onRender();
  tonePass->endPass();
  // Present the final result to the screen
  g_GraphicAPI().present(m_vSync, 0);
}
}
