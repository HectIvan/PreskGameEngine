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
BaseApp::init(const char** _argv, int32 _count)
{
  AssetResourceManager::startUp();
  DLLManager::startUp();
  EventQueue::startUp();
  GPUResourceManager::startUp();
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
  // g_ShaderManager().createShaderResources();
  assetResource.init("resources");
  assetResource.loadAssetsFromResourcesFolder();
  g_SceneManager().init();
  g_RenderManager().init();
  g_TextureManager().init();
  g_GPUResourceManager().init();

  onInit();
}

// to do: class command line parser module to register arguments.
void
BaseApp::initAPI(const char** _argv, int32 _count)
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
  // GPUResourceManager::shutDown();
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
  CBVector2x2 shadowsParam;
  shadowsParam.vec1 = winSize; // to do: win size could change, swap this to use the specific texture size.
  shadowsParam.vec2 = Vector2(0.0f);
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
  uint32 m4x4Size = sizeof(Matrix4);
  uint32 cBCamSize = sizeof(CBCamera);
  uint32 cBLightSize = sizeof(CBLight);

  // get all passes.
  SPtr<Pass> baseShadow = rm.getPass(kP_Shadow);
  SPtr<Pass> basePass = rm.getPass(kP_Base);
  SPtr<Pass> skyBoxPass = rm.getPass(kP_SkyBox);
  SPtr<Pass> IBRPass = rm.getPass(kP_IBL);
  SPtr<Pass> quadShadows = rm.getPass(kP_ShadowQuad);
  SPtr<Pass> lumPass = rm.getPass(kP_Luminance);
  SPtr<Pass> lumBlurHPass = rm.getPass(kP_LumBlurH);
  SPtr<Pass> lumBlurPass = rm.getPass(kP_LumBlur);
  SPtr<Pass> emissHBlur = rm.getPass(kP_EmissiveHBlur);
  SPtr<Pass> emissBlur = rm.getPass(kP_EmissiveBlur);
  SPtr<Pass> tonePass = rm.getPass(kP_Tone);

  // update normal && base shadow pass buffers.
  api.updateConstantBuffer(basePass->getCBuffer(0), &view, m4x4Size);
  api.updateConstantBuffer(basePass->getCBuffer(1), &proj, m4x4Size);

  api.updateConstantBuffer(baseShadow->getCBuffer(0), &lightView, m4x4Size);
  api.updateConstantBuffer(baseShadow->getCBuffer(1), &lightProj, m4x4Size);

  // update shadow-specular quad pass
  api.updateConstantBuffer(quadShadows->getCBuffer(0), &cBLight, cBLightSize);
  api.updateConstantBuffer(quadShadows->getCBuffer(1), &cBCamera, cBCamSize);
  api.updateConstantBuffer(quadShadows->getCBuffer(2), &lightViewProj, m4x4Size);
  api.updateConstantBuffer(quadShadows->getCBuffer(3), &shadowsParam, sizeof(Vector4));

  // skybox constant buffers.
  api.updateConstantBuffer(skyBoxPass->getCBuffer(0), &viewTransp, m4x4Size);
  api.updateConstantBuffer(skyBoxPass->getCBuffer(1), &projTransp, m4x4Size);

  // ibr constant buffers.
  api.updateConstantBuffer(IBRPass->getCBuffer(0), &IBRIntens, sizeof(Vector4));
  api.updateConstantBuffer(IBRPass->getCBuffer(1), &viewPos, sizeof(CBVector3));

  // luminance constant buffers.
  api.updateConstantBuffer(lumPass->getCBuffer(0), &lum, sizeof(CBVector2x2));
  // Emissive blur constant buffers;
  emissiveBlur.BlurDirection = Vector2(1.0f, 0.0f);
  api.updateConstantBuffer(emissHBlur->getCBuffer(0), &emissiveBlur, sizeof(CBBlur));
  emissiveBlur.BlurDirection = Vector2(0.0f, 1.0f);
  api.updateConstantBuffer(emissBlur->getCBuffer(0), &emissiveBlur, sizeof(CBBlur));
  // lum blur constant buffers
  blur.BlurDirection = Vector2(1.0f, 0.0f);
  api.updateConstantBuffer(lumBlurHPass->getCBuffer(0), &blur, sizeof(CBBlur));
  blur.BlurDirection = Vector2(0.0f, 1.0f);
  api.updateConstantBuffer(lumBlurPass->getCBuffer(0), &blur, sizeof(CBBlur));
}

void
BaseApp::render()
{
  // get managers
  GraphicsAPI& api = g_GraphicAPI();
  RendererManager& renderManager = g_RenderManager();
  // get all passes
  SPtr<Pass> baseShadow = renderManager.getPass(kP_Shadow);
  SPtr<Pass> basePass = renderManager.getPass(kP_Base);
  SPtr<Pass> shadowQuad = renderManager.getPass(kP_ShadowQuad);
  SPtr<Pass> skyBoxPass = renderManager.getPass(kP_SkyBox);
  SPtr<Pass> ssaoPass = renderManager.getPass(kP_SSAO);
  SPtr<Pass> IBRPass = renderManager.getPass(kP_IBL);
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
  baseShadow->beginPass();
  renderManager.renderActors(actors);
  baseShadow->endPass();

  // base pass
  basePass->beginPass(Color::BLACK);
  renderManager.renderActors(actors);
  basePass->endPass();

  // get texel size of compute passes
  //        Vector2 texSize = api.getSwapChain()->getSize();
  //        uint32 threadWidth = 16;
  //        uint32 threadHeight = 16;
  //        uint32 x = static_cast<uint32>((texSize.x + threadWidth - 1) / threadWidth);
  //        uint32 y = static_cast<uint32>((texSize.y + threadHeight - 1) / threadHeight);
  // if shadows are set to be rendered
  api.clearRenderTargetViews(Color::WHITE, shadowQuad->getOutputTextures());
  shadowQuad->beginPass();
  api.draw(3, 0);
  shadowQuad->endPass();

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
  // IBR Pass.
  api.clearRenderTargetViews(Color::BLACK, IBRPass->getOutputTextures());
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
  api.present(m_vSync, 0);
}
}
