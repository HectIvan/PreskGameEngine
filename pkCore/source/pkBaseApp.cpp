#include "pkAssetResourceManager.h"
#include "pkBaseApp.h"
#include "pkDllManager.h"
#include "pkGraphicsAPI.h"
#include "pkLogger.h"
#include "pkLight.h"
#include "pkMaterialCodec.h"
#include "pkMaterialManager.h"
#include "pkMath.h"
#include "pkModel.h"
#include "pkModelCodec.h"
#include "pkPrerequisitesCore.h"
#include "pkSceneManager.h"
#include "pkShaderCodec.h"
#include "pkShaderManager.h"
#include "pkSprite.h"
#include "pkTextureCodec.h"
#include "pkTextureManager.h"
#include "pkTimeManager.h"
#include "pkWindowDesc.h"

namespace pkEngineSDK
{

void
BaseApp::init(const ANSICHAR** _argv, int32 _count)
{
  AssetResourceManager::startUp();
  DLLManager::startUp();
  EventQueue::startUp();
  ModelManager::startUp();
  MaterialCodec::startUp();
  MaterialManager::startUp();
  RendererManager::startUp();
  SceneManager::startUp();
  ShaderManager::startUp();
  TextureManager::startUp();
  TimeManager::startUp();

  DLLManager& dllManager = g_DLLManager();
  AssetResourceManager& assetResource = g_AssetResourceManager();


  initWin();
  initAPI(_argv, _count);

#if PK_DEBUG_MODE
  dllManager.runDll("pkModelCodecd");
  dllManager.runDll("pkStbiImageCodecd");
#else
  dllManager.runDll("pkModelCodec");
  dllManager.runDll("pkStbiImageCodec");
#endif

  g_GraphicAPI().init(m_window);
  g_ShaderManager().createShaderResources();
  assetResource.init("resources");
  assetResource.loadAssetsFromResourcesFolder();
  g_MaterialManager().init();
  g_SceneManager().init();
  g_RenderManager().init();
  g_TextureManager().init();
  g_ModelManager().init();

  onInit();
}

// to do: class command line parser module to register arguments.
void
BaseApp::initAPI(const ANSICHAR** _argv, int32 _count)
{
  DLLManager& dllManager = g_DLLManager();
  String abstraction = "DX11APId";
  if (_count > 1) {
    abstraction = _argv[1]; // _argv[0] is always the executable path
  }

#if PK_DEBUG_MODE
  if (abstraction == "DX11APId") {
    dllManager.runDll("pkDX11APId");
  }
#else
  if (abstraction == "DX11API") {
    dllManager.runDll("pkDX11API");
  }
#endif
}

void
BaseApp::messageLoop()
{
  EventQueue& eventQueue = g_EventManager();
  TimeManager& timeManager = g_TimeManager();
  SceneManager& sceneManager = g_SceneManager();
  SPtr<Scene> activeScene = sceneManager.getActiveScene();
  // get the starting deltaTime
  high_resolution_clock::time_point delta = high_resolution_clock::now();

  while (m_run) {
    // reset scroll wheel input
    eventQueue.scrollWheel = 0;
    // event window specific input
    eventQueue.windowInput(m_window);
    // update the delta time
    // m_deltaTime = timeManager.getDeltaTime(delta);
    timeManager.m_deltaTime = timeManager.getDeltaTime(delta);
    // fixed update timer count.
    m_fixedTimer += timeManager.m_deltaTime;
    // base app update
    update();
    // child class app update
    onUpdate();
    // update scene
    if (m_fixedTimer > 0.016f) {
      // fixed update
      fixedUpdate();
      activeScene->update(timeManager.m_deltaTime);
      m_fixedTimer = 0;
    }
    else {
      activeScene->update(timeManager.m_fixedDeltaTime);
    }
    // event queue
    eventQueue.poll();
    // render the scene
    render();
  }

  // if the program is shut dowor closes for any reason, close everything properly.
  // Logger::shutDown();
  // RendererManager::shutDown();
  // ModelManager::shutDown();
  // SceneManager::shutDown();
  // TextureManager::shutDown();
  // TimeManager::shutDown();
  // EventQueue::shutDown();
}

void
BaseApp::update()
{
  // managers
  GraphicsAPI& api = g_GraphicAPI();
  RendererManager& rm = g_RenderManager();

  Vector2 winSize = api.getSwapChain()->getSize();

  // camera data
  Matrix4 view = Matrix4::IDENTITY;
  Matrix4 proj = Matrix4::IDENTITY;
  Matrix4 invView = Matrix4::IDENTITY;
  Matrix4 invProj = Matrix4::IDENTITY;
  Matrix4 invViewProj = Matrix4::IDENTITY;
  Matrix4 viewTransp = Matrix4::IDENTITY;
  Matrix4 projTransp = Matrix4::IDENTITY;
  // main camera buffer
  CBCamera cBCamera;
  CBVector2x2 lightsParam;
  lightsParam.vec1 = winSize; // to do: win size could change, swap this to use the specific texture size.
  lightsParam.vec2 = Vector2(0.0f);
  Vector4 SkyBoxWinSize(winSize.x, winSize.y, 0.0f, 0.0f);

  // light buffers
  CBLight cBLight;
  CBCamera cBLightCam;

  // update shadow depth map buffers
  Matrix4 lightView = Matrix4::IDENTITY;
  Matrix4 lightProj = Matrix4::IDENTITY;
  Matrix4 lightViewProj = Matrix4::IDENTITY;

  // luminance parameters.
  CBVector2x2 lum;
  lum.vec1 = winSize;
  lum.vec2.x = 90.0f;
  // blur parameters.
  CBBlur blur;
  blur.WinSize = winSize;
  blur.BlurDirection = Vector2(1.0f, 0.0f);
  blur.radius = 1.0f;
  blur.strength = 2.0f;
  // emissive blur parameters
  CBBlur emissiveBlur;
  emissiveBlur.WinSize = winSize;
  emissiveBlur.radius = 30.0f;
  emissiveBlur.strength = 30.0f;
  // IBR parameters.
  CBFloat IBRIntens;
  IBRIntens.value = 1.0f;
  CBVector3 viewPos;
  viewPos.vec1 = Vector3::ZERO;

  // data type sizes.
  const uint32 m4x4Size = sizeof(Matrix4);
  const uint32 v4Size = sizeof(Vector4);
  const uint32 cBCamSize = sizeof(CBCamera);
  const uint32 cBLightSize = sizeof(CBLight);
  const uint32 cBlurSize = sizeof(CBBlur);

  // get all passes.
  SPtr<Pass> lightPositions = rm.getPass(kP_LightPositions);
  SPtr<Pass> basePass = rm.getPass(kP_Base);
  SPtr<Pass> skyBoxPass = rm.getPass(kP_SkyBox);
  SPtr<Pass> quadLight = rm.getPass(kP_Light);
  SPtr<Pass> lumPass = rm.getPass(kP_Luminance);
  SPtr<Pass> lumBlurHPass = rm.getPass(kP_LumBlurH);
  SPtr<Pass> lumBlurPass = rm.getPass(kP_LumBlur);
  SPtr<Pass> emissHBlur = rm.getPass(kP_EmissiveHBlur);
  SPtr<Pass> emissBlur = rm.getPass(kP_EmissiveBlur);
  SPtr<Pass> tonePass = rm.getPass(kP_Tone);
  SPtr<Pass> transparencyPass = rm.getPass(kP_Transparency);
  SPtr<Pass> transpBRDF = rm.getPass(kP_LightTransparency);

  // update normal && base shadow pass buffers.
  basePass->updateCBuffer(0, &view, m4x4Size);
  basePass->updateCBuffer(1, &proj, m4x4Size);

  lightPositions->updateCBuffer(0, &lightView, m4x4Size);
  lightPositions->updateCBuffer(1, &lightProj, m4x4Size);

  transparencyPass->updateCBuffer(0, &lightView, m4x4Size);
  transparencyPass->updateCBuffer(1, &lightProj, m4x4Size);

  // update shadow-specular quad pass
  Vector4 iblParams = Vector4(1.0f);
  quadLight->updateCBuffers({ &cBLight, &cBCamera, &lightViewProj, &lightsParam, &iblParams },
                            { cBLightSize, cBCamSize, m4x4Size, v4Size, v4Size });

  transpBRDF->updateCBuffers({ &cBLight, &cBCamera, &lightViewProj, &lightsParam, &iblParams },
                             { cBLightSize, cBCamSize, m4x4Size, v4Size, v4Size });

  // skybox constant buffers.
  skyBoxPass->updateCBuffers({ &viewTransp, &projTransp }, { m4x4Size, m4x4Size });

  // luminance constant buffers.
  lumPass->updateCBuffer(0, &lum, v4Size);
  // Emissive blur constant buffers;
  emissiveBlur.BlurDirection = Vector2(1.0f, 0.0f);
  emissHBlur->updateCBuffer(0, &emissiveBlur, cBlurSize);
  emissiveBlur.BlurDirection = Vector2(0.0f, 1.0f); 
  emissBlur->updateCBuffer(0, &emissiveBlur, cBlurSize);
  // lum blur constant buffers
  blur.BlurDirection = Vector2(1.0f, 0.0f);
  lumBlurHPass->updateCBuffer(0, &blur, cBlurSize);
  blur.BlurDirection = Vector2(0.0f, 1.0f);
  lumBlurPass->updateCBuffer(0, &blur, cBlurSize);
}

void
BaseApp::render()
{
  // get managers
  GraphicsAPI& api = g_GraphicAPI();
  RendererManager& renderManager = g_RenderManager();
  // get all passes
  const SPtr<Pass> lightPositions = renderManager.getPass(kP_LightPositions);
  const SPtr<Pass> basePass = renderManager.getPass(kP_Base);
  const SPtr<Pass> transparencyPass = renderManager.getPass(kP_Transparency);
  const SPtr<Pass> skyBoxPass = renderManager.getPass(kP_SkyBox);
  const SPtr<Pass> quadLight = renderManager.getPass(kP_Light);
  const SPtr<Pass> ssaoPass = renderManager.getPass(kP_SSAO);
  const SPtr<Pass> emissHBlurPass = renderManager.getPass(kP_EmissiveHBlur);
  const SPtr<Pass> emissBlurPass = renderManager.getPass(kP_EmissiveBlur);
  const SPtr<Pass> lumPass = renderManager.getPass(kP_Luminance);
  const SPtr<Pass> lumBlurHPass = renderManager.getPass(kP_LumBlurH);
  const SPtr<Pass> lumBlurPass = renderManager.getPass(kP_LumBlur);
  const SPtr<Pass> tonePass = renderManager.getPass(kP_Tone);
  const SPtr<Pass> transparencyBRDF = renderManager.getPass(kP_LightTransparency);

  // Get all actors
  const Vector<SPtr<Actor>> actors = g_SceneManager().getActiveScene()->getAllActors();

  // first shadow pass
  lightPositions->beginPass();
  renderManager.renderActors(actors);
  lightPositions->endPass();

  // base pass
  basePass->beginPass(Color::BLACK);
  renderManager.renderActors(actors);
  basePass->endPass();

  transparencyPass->beginPass(Color::BLACK);
  renderManager.renderActors(actors);
  transparencyPass->endPass();

  // get texel size of compute passes
  //        Vector2 texSize = api.getSwapChain()->getSize();
  //        uint32 threadWidth = 16;
  //        uint32 threadHeight = 16;
  //        uint32 x = static_cast<uint32>((texSize.x + threadWidth - 1) / threadWidth);
  //        uint32 y = static_cast<uint32>((texSize.y + threadHeight - 1) / threadHeight);
  quadLight->beginPass(Color::WHITE);
  api.draw(3, 0);
  quadLight->endPass();

  transparencyBRDF->beginPass(Color::WHITE);
  api.draw(3, 0);
  transparencyBRDF->endPass();

  // ssao pass
  api.clearRenderTargetViews(Color::WHITE, ssaoPass->getOutputTextures());
  if (m_ssao) {
    ssaoPass->beginPass();
    api.draw(3, 0);
    ssaoPass->endPass();
  }

  // render the skybox
  skyBoxPass->beginPass();
  api.draw(3, 0);
  skyBoxPass->endPass();

  // emissive Horizontal Blur pass
  emissHBlurPass->beginPass();
  api.draw(3, 0);
  emissHBlurPass->endPass();

  // emissive pass
  emissBlurPass->beginPass();
  api.draw(3, 0);
  emissBlurPass->endPass();

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
  api.present(m_vSync, 0);
}
}
