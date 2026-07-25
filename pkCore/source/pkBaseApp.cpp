#include "pkAssetResourceManager.h"
#include "pkBaseApp.h"
#include "pkCBuffers.h"
#include "pkCamera.h"
#include "pkDllManager.h"
#include "pkEventQueue.h"
#include "pkGraphicsAPI.h"
#include "pkLogger.h"
#include "pkLight.h"
#include "pkMaterialCodec.h"
#include "pkMaterialManager.h"
#include "pkMath.h"
#include "pkModel.h"
#include "pkModelCodec.h"
#include "pkModelManager.h"
#include "pkPrerequisitesCore.h"
#include "pkRendererManager.h"
#include "pkSceneManager.h"
#include "pkShaderCodec.h"
#include "pkShaderManager.h"
#include "pkSprite.h"
#include "pkTextureCodec.h"
#include "pkTextureManager.h"
#include "pkTimeManager.h"
#include "pkWindowDesc.h"

using pkEngineSDK::PASS_TYPE::kP_Base;
using pkEngineSDK::PASS_TYPE::kP_Transparency;
using pkEngineSDK::PASS_TYPE::kP_LightPositions;
using pkEngineSDK::PASS_TYPE::kP_Merge;
using pkEngineSDK::PASS_TYPE::kP_SkyBox;
using pkEngineSDK::PASS_TYPE::kP_Light;
using pkEngineSDK::PASS_TYPE::kP_LightTransparency;
using pkEngineSDK::PASS_TYPE::kP_SSAO;
using pkEngineSDK::PASS_TYPE::kP_EmissiveHBlur;
using pkEngineSDK::PASS_TYPE::kP_EmissiveBlur;
using pkEngineSDK::PASS_TYPE::kP_Luminance;
using pkEngineSDK::PASS_TYPE::kP_LumBlurH;
using pkEngineSDK::PASS_TYPE::kP_LumBlur;
using pkEngineSDK::PASS_TYPE::kP_Tone;

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
  RendererManager& renderMan = g_RenderManager();

  initWin();
  // initAPI(_argv, _count);

  Vector<const ANSICHAR*> dlls = { "pkModelCodec", "pkStbiImageCodec", "pkDX11API" };

  const uint32 dllCount = toUint32(dlls.size());
#if PK_DEBUG_MODE
  for (uint32 i = 0; i < dllCount; ++i) {
    const String dllName = String(dlls[i]) + "d";
    if (!dllManager.runDll(dllName)) {
      const String msg = "Failed to load " + dllName;
      LOG_FATAL(msg, __FILE__, __LINE__);
      THROW_ERROR(msg);
    }
  }
#else
  for (uint32 i = 0; i < dllCount; ++i) {
    const ANSICHAR* dllName = dlls[i];
    if (!dllManager.runDll(dllName)) {
      const String msg = "Failed to load " + String(dllName);
      LOG_FATAL(msg, __FILE__, __LINE__);
      THROW_ERROR(msg);
    }
  }
#endif

  g_GraphicAPI().init(m_window);
  assetResource.init("resources");
  g_ShaderManager().createShaderResources();
  assetResource.loadAssetsFromResourcesFolder();
  g_ShaderManager().createShaders();
  g_MaterialManager().init();
  g_SceneManager().init();
  renderMan.init();
  g_TextureManager().init();
  g_ModelManager().init();

  SceneManager& sceneMan = g_SceneManager();
  SPtr<Scene> activeScene = sceneMan.getActiveScene();

  // create camera
  const Vector3 camPos = Vector3(0.0f, 0.0f, -30.0f);
  CameraDesc camDescription;
  camDescription.width = m_window.getWidth();
  camDescription.height = m_window.getHeight();
  camDescription.eye = camPos;
  camDescription.isMain = true;

  m_camera = activeScene->instantiate("Main Camera", camPos);
  m_camera->setPosition(camPos);
  m_camera->addComponent(renderMan.createCamera(camDescription));

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
    timeManager.m_deltaTime = timeManager.getDeltaTime(delta);
    m_fixedTimer += timeManager.m_deltaTime;
    // base app update
    update();
    // child class app update
    onUpdate();
    // update scene
    if (m_fixedTimer > 0.016f) {
      // fixed update
      fixedUpdate();
      activeScene->update(timeManager.m_fixedDeltaTime);
      m_fixedTimer = 0.0f;
    }
    else {
      activeScene->update(timeManager.m_deltaTime);
    }
    // event queue
    eventQueue.poll();
    // render the scene
    render();
  }

  g_Logger().createLogFiles();

  // if the program is shut down closes for any reason, close everything properly.
  g_GraphicAPI().debugDevice();
  RendererManager::shutDown();
  ModelManager::shutDown();
  MaterialManager::shutDown();
  ShaderManager::shutDown();
  TextureManager::shutDown();
  SceneManager::shutDown();
  MaterialCodec::shutDown();
  EventQueue::shutDown();
  DLLManager::shutDown();
  AssetResourceManager::shutDown();
  GraphicsAPI::shutDown();
  TimeManager::shutDown();
}

void
BaseApp::update()
{
  // managers
  GraphicsAPI& api = g_GraphicAPI();
  RendererManager& rm = g_RenderManager();

  const Vector2 winSize = api.getSwapChain()->getSize();
  const SPtr<Scene> activeScene = g_SceneManager().getActiveScene();

  /**
   *  to do: temporary.
   */
  const Vector<SPtr<Actor>> actors = activeScene->getAllActors();
  rm.m_lights.clear();
  const uint32 actorCount = toUint32(actors.size());
  for (uint32 i = 0; i < actorCount; ++i) {
    SPtr<Light> light = actors[i]->getComponent<Light>();
    if (light) {
      rm.m_lights.push_back(light);
    }
  }

  // camera data.
  Matrix4 view = Matrix4::IDENTITY;
  Matrix4 proj = Matrix4::IDENTITY;
  Matrix4 viewTransp = Matrix4::IDENTITY;
  Matrix4 projTransp = Matrix4::IDENTITY;

  SPtr<Camera> camera = rm.getMainCamera();
  CBCamera camCBuffer;
  if (camera) {
    view = camera->getView();
    proj = camera->getProjection();
    viewTransp = view.getTransposed();
    projTransp = proj.getTransposed();
    camCBuffer = CBCamera(camera);
  }

  // if there's no actor light, search the scene for an actor with a light.
  SPtr<Light> light = nullptr;
  CBLight lightCBuffer;
  if (!m_light) {
    m_light = activeScene->getActorWithComponent<Light>();
  }
  if (m_light) {
    light = m_light->getComponent<Light>();
    lightCBuffer = CBLight(light);
  }

  // get all passes.
  const SPtr<Pass> skyBoxPass = rm.getPass(kP_SkyBox);
  const SPtr<Pass> lumPass = rm.getPass(kP_Luminance);
  const SPtr<Pass> lumBlurHPass = rm.getPass(kP_LumBlurH);
  const SPtr<Pass> lumBlurPass = rm.getPass(kP_LumBlur);
  const SPtr<Pass> emissHBlur = rm.getPass(kP_EmissiveHBlur);
  const SPtr<Pass> emissBlur = rm.getPass(kP_EmissiveBlur);
  const SPtr<Pass> tonePass = rm.getPass(kP_Tone);
  const SPtr<Pass> ssaoPass = rm.getPass(kP_SSAO);
  const SPtr<Pass> brdfPass = rm.getPass(kP_Light);
  const SPtr<Pass> brdfTranspPass = rm.getPass(kP_LightTransparency);

  // constant buffers.
  CBVector2x2 lum(winSize, Vector2(90.0f));
  CBBlur blur(winSize, Vector2(1.0f, 0.0f), m_blurRad, m_blurStr);
  CBBlur emissiveBlur(winSize, Vector2(1.0f, 0.0f), m_emissRad, m_emissStr);
  CBVector2x2 shadowsParam(winSize, Vector2(0.0f)); // will be modified later.
  const CBFloat exposureCBuffer(m_exposure);
  const CBVector2x2 ssaoWin(ssaoPass->getViewportSize(), Vector2(0.0f));
  const CBVector2x2 ssao(m_ssaoSRad, m_ssaoScale, m_ssaoBias, m_ssaoInt);

  // data type sizes.
  const uint32 m4x4Size = sizeof(Matrix4);
  const uint32 v2x2Size = sizeof(CBVector2x2);
  const uint32 cBlurSize = sizeof(CBBlur);
  const SIZE_T cbCamSize = sizeof(CBCamera);
  const SIZE_T cbLightSize = sizeof(CBLight);

  // skybox constant buffers.
  skyBoxPass->updateCBuffers({ &viewTransp, &projTransp }, { m4x4Size, m4x4Size });


  brdfPass->updateCBuffers({ &lightCBuffer, &camCBuffer }, { cbLightSize, cbCamSize });
  brdfTranspPass->updateCBuffers({ &lightCBuffer, &camCBuffer }, { cbLightSize, cbCamSize });
  // luminance constant buffers.
  lumPass->updateCBuffer(0, &lum, sizeof(CBFloat));
  // Emissive blur constant buffers;
  emissHBlur->updateCBuffer(0, &emissiveBlur, cBlurSize);
  emissiveBlur.BlurDirection = Vector2(0.0f, 1.0f); 
  emissBlur->updateCBuffer(0, &emissiveBlur, cBlurSize);
  // lum blur constant buffers
  lumBlurHPass->updateCBuffer(0, &blur, cBlurSize);
  blur.BlurDirection = Vector2(0.0f, 1.0f);
  lumBlurPass->updateCBuffer(0, &blur, cBlurSize);

  tonePass->updateCBuffer(0, &exposureCBuffer, v2x2Size);

  ssaoPass->updateCBuffers({ &ssao, &ssaoWin }, { v2x2Size, v2x2Size });
}

void
BaseApp::render()
{
  // get managers
  GraphicsAPI& api = g_GraphicAPI();
  RendererManager& renderManager = g_RenderManager();

  // get all passes
  // const SPtr<Pass> basePass = renderManager.getPass(kP_Base);
  // const SPtr<Pass> transparencyPass = renderManager.getPass(kP_Transparency);
  const SPtr<Pass> skyBoxPass = renderManager.getPass(kP_SkyBox);
  const SPtr<Pass> mergePass = renderManager.getPass(kP_Merge);
  const SPtr<Pass> BRDF = renderManager.getPass(kP_Light);
  const SPtr<Pass> transparencyBRDF = renderManager.getPass(kP_LightTransparency);
  const SPtr<Pass> ssaoPass = renderManager.getPass(kP_SSAO);
  const SPtr<Pass> emissHBlurPass = renderManager.getPass(kP_EmissiveHBlur);
  const SPtr<Pass> emissBlurPass = renderManager.getPass(kP_EmissiveBlur);
  const SPtr<Pass> lumPass = renderManager.getPass(kP_Luminance);
  const SPtr<Pass> lumBlurHPass = renderManager.getPass(kP_LumBlurH);
  const SPtr<Pass> lumBlurPass = renderManager.getPass(kP_LumBlur);
  const SPtr<Pass> tonePass = renderManager.getPass(kP_Tone);

  // Get all actors
  const Vector<SPtr<Actor>> actors = g_SceneManager().getActiveScene()->getAllActors();

  const SPtr<Pass> lightPositions = renderManager.getPass(kP_LightPositions);
  // first shadow pass

  const SPtr<Pass> basePass = renderManager.getPass(PASS_TYPE::kP_Base);
  const SPtr<Pass> transparencyPass = renderManager.getPass(PASS_TYPE::kP_Transparency);

  basePass->beginPass(FColor::BLACK);
  renderManager.renderActors(actors);
  basePass->endPass();

  // transparencyPass->beginPass(Color::BLACK);
  // renderManager.renderActors(actors);
  // transparencyPass->endPass();
  // 
  // lightPositions->beginPass(Color::BLACK);
  // renderManager.renderActors(actors);
  // lightPositions->endPass();


  // get texel size of compute passes

  // const Vector2 texSize = api.getSwapChain()->getSize();
  // const uint32 threadWidth = 16;
  // const uint32 threadHeight = 16;
  // const uint32 x = toUint32((texSize.x + threadWidth - 1) / threadWidth);
  // const uint32 y = toUint32((texSize.y + threadHeight - 1) / threadHeight);

  BRDF->beginPass(FColor::WHITE);
  api.draw(3, 0);
  BRDF->endPass();
  
  // transparencyBRDF->beginPass(FColor(0, 0, 0, 1));
  // api.draw(3, 0);
  // transparencyBRDF->endPass();

  // render the skybox
  skyBoxPass->beginPass(FColor::CYAN);
  api.draw(3, 0);
  skyBoxPass->endPass();

  mergePass->beginPass(FColor::CYAN);
  api.draw(3, 0);
  mergePass->endPass();

  // --------------- post processing --------------- //

  // ssao pass
  // api.clearRenderTargetViews(Color::WHITE, ssaoPass->getOutputTextures());
  // if (m_ssao) {
  //   ssaoPass->beginPass();
  //   api.draw(3, 0);
  //   ssaoPass->endPass();
  // }
  // 
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
  tonePass->beginPass(FColor::CYAN);
  api.draw(3, 0);
  // Scene specific app render
  onRender();
  tonePass->endPass();
  // Present the final result to the screen
  api.present(m_vSync, 0);
}
}
