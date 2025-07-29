#include "ActorInspector.h"
#include "pkLogger.h"
#include "pkGraphicsAPI.h"
#include "pkGraphicTypes.h"
#include "pkUInterface.h"
#include "pkPlatformMath.h"
#include "pkPath.h"
#include "pkRendererManager.h"
#include "pkResourceManager.h"
#include "pkSceneManager.h"
#include "pkTextureManager.h"
#include "pkTimeManager.h"
#include "ShaderTest.h"
#include "pkColor.h"

using pkEngineSDK::Color;
using pkEngineSDK::CBBlur;
using pkEngineSDK::CBLuminance;
using pkEngineSDK::CBShadowParam;
using pkEngineSDK::CreateCBCamera;
using pkEngineSDK::CreateCBLight;
using pkEngineSDK::D_BUFFERS::kDB_Base;
using pkEngineSDK::D_BUFFERS::kDB_Light;
using pkEngineSDK::EventQueue;
using pkEngineSDK::GraphicsAPI;
using pkEngineSDK::G_BUFFERS::kGB_Albedo;
using pkEngineSDK::G_BUFFERS::kGB_Normal;
using pkEngineSDK::g_eventManager;
using pkEngineSDK::g_GraphicAPI;
using pkEngineSDK::g_uInterface;
using pkEngineSDK::g_Logger;
using pkEngineSDK::g_RenderManager;
using pkEngineSDK::g_ResourceManager;
using pkEngineSDK::g_SceneManager;
using pkEngineSDK::g_TextureManager;
using pkEngineSDK::g_TimeManager;
using pkEngineSDK::int32;
using pkEngineSDK::UInterface;
using pkEngineSDK::Light;
using pkEngineSDK::Logger;
using pkEngineSDK::Material;
using pkEngineSDK::Math;
using pkEngineSDK::Matrix4;
using pkEngineSDK::Path;
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
using pkEngineSDK::PKWindowDesc;
using pkEngineSDK::PlatformPointer;
using pkEngineSDK::RendererManager;
using pkEngineSDK::ResourceManager;
using pkEngineSDK::Scene;
using pkEngineSDK::SceneManager;
using pkEngineSDK::SPtr;
using pkEngineSDK::String;
using pkEngineSDK::TextureManager;
using pkEngineSDK::to_string;
using pkEngineSDK::uint32;
using pkEngineSDK::Vector4;
// to do: create fileSystem.h in utilities
// create class Path

#if PK_PLATFORM == PK_PLATFORM_WIN32
#include "externals/imgui_impl_win32.h"
// to do: delete this and replace with own
#include "Windows.h"

// Win32 message handler your application need to call.
// - Intentionally commented out in a '#if 0' block to avoid dragging dependencies on <windows.h> from this helper.
// - You should COPY the line below into your .cpp code to forward declare the function and then you can call it.
// - Call from your application's message handler. Keep calling your message handler unless this function returns TRUE.

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
                                                             UINT msg,
                                                             WPARAM wParam,
                                                             LPARAM lParam);
#endif

void
ShaderTest::onInit()
{
  //start the interface
  UInterface::startUp();
  g_uInterface().init();
  g_uInterface().initWin(m_window.getWindowHandle());
  // get the resource manager
  ResourceManager& resourceMan = g_ResourceManager().instance();

  // create camera
  m_cameraSpeed = 20.0f;
  m_camera = g_SceneManager().getActiveScene()->instantiate("Main Camera");
  m_camera->addComponent(make_shared<Camera>());
  Vector3 camPos = Vector3(0.0f, 0.0f, -30.0f);
  m_camera->getComponent<Camera>()->init(m_window.getWidth(),
                                         m_window.getHeight(),
                                         3.1416f / 4.0f,
                                         0.01f,
                                         5000.0f,
                                         camPos, // position
                                         Vector3::FORWARD + camPos * -1.0f, // target
                                         Vector3(0.0f, 1.0f, 0.0f)); // up vector
  // camera sensitivity
  m_sensX = 0.3f;
  m_sensY = 0.3f;

  // create light
  m_light = g_SceneManager().getActiveScene()->instantiate("Light");
  m_light->addComponent(make_shared<Light>());
  SPtr<Light> lightCom = m_light->getComponent<Light>();

  // add camera component
  m_light->addComponent(make_shared<Camera>());
  m_light->getComponent<Camera>()->init(1280,
                                        720,
                                        3.1416f / 4.0f,
                                        0.01f,
                                        2000.0f,
                                        lightCom->m_position, // position
                                        lightCom->m_direction, // target
                                        Vector3::FORWARD,
                                        pkEngineSDK::CAMERA_PROJ::kOrthographic); // up vector);

  SPtr<Actor> pistol = g_SceneManager().getActiveScene()->instantiate("Pistol");
  pistol->addComponent(resourceMan.loadModel(Path("models/drakefire_pistol_low.obj")));
  pistol->setScale(10.0f);
  pistol->setPosition(0.0f, 5.0f, 0.0f);

  // SPtr<Actor> leon = g_SceneManager().getActiveScene()->instantiate("Leon");
  // leon->addComponent(resourceMan.loadModel(Path("models/leon.obj")));

  SPtr<Actor> sponza = g_SceneManager().getActiveScene()->instantiate("Sponza");
  sponza->addComponent(resourceMan.loadModel(Path("models/sponza.obj")));

  // SPtr<Actor> rpd = g_SceneManager().getActiveScene()->instantiate("RPD");
  // rpd->addComponent(resourceMan.loadModel(Path("models/rpd.obj")));
  // rpd->setRotation(0, 90, 0);
  // // rpd->setScale(1.0f);
  // rpd->setPosition(0, 0, -100);

  SPtr<Actor> coat = g_SceneManager().getActiveScene()->instantiate("Coat");
  coat->addComponent(resourceMan.loadModel(Path("models/export3dcoat.obj")));
  coat->setPosition(11.0f, 5.2f, 0.0f);

  m_shadows = true;
  m_specular = true;

  m_sActorIndex = 0;
  m_fpsSize = 20;
}

void
ShaderTest::initWin()
{
  PKWindowDesc desc;
  desc.width = 1920;
  desc.height = 1080;
  std::string name = "Game Engine Window";
  // imgui input
#if PK_PLATFORM == PK_PLATFORM_WIN32
  desc.funct = [](PlatformPointer _hwnd,
                  int32 _msg,
                  PlatformPointer _wParam,
                  PlatformPointer _lParam) {
    LRESULT result = ImGui_ImplWin32_WndProcHandler(reinterpret_cast<HWND>(_hwnd),
                                                    _msg,
                                                    reinterpret_cast<WPARAM>(_wParam),
                                                    reinterpret_cast<LPARAM>(_lParam));
    return reinterpret_cast<PlatformPointer>(result);
  };
#endif
  m_window.create(desc);
}

void
ShaderTest::input()
{
  EventQueue& eventQueue = g_eventManager().instance();
  UInterface& im = g_uInterface().instance();
  // bool interfaceHovered = im.isHoveredWithItems();
  bool itemActive = im.isItemActive();
  float deltaTime = g_TimeManager().m_deltaTime;
  // set camera speed with deltaTime
  float speed = m_cameraSpeed * deltaTime;
  // move forward/backward
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kW) && !itemActive) {
    m_camera->getComponent<Camera>()->moveForwardLocal(speed);
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kS) && !itemActive) {
    m_camera->getComponent<Camera>()->moveForwardLocal(-speed);
  }
  // move left/right
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kA) && !itemActive) {
    m_camera->getComponent<Camera>()->moveRightLocal(-speed);
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kD) && !itemActive) {
    m_camera->getComponent<Camera>()->moveRightLocal(speed);
  }
  // move up/down
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kE) && !itemActive) {
    m_camera->getComponent<Camera>()->moveUpLocal(speed);
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kQ) && !itemActive) {
    m_camera->getComponent<Camera>()->moveUpLocal(-speed);
  }
  // rotate camera
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kLButton) && !itemActive) {
    Vector2 posDif = (m_lastCursorPos - eventQueue.mousePosition);
    // m_selectedActor = nullptr;
    posDif.x *= m_sensX;
    posDif.y *= m_sensY;
    m_lastCursorPos = eventQueue.mousePosition;
    posDif *= Math::DEG2RAD;
    m_camera->getComponent<Camera>()->rotate(-posDif.y, -posDif.x, 0.0f);
  }
  m_lastCursorPos = eventQueue.mousePosition;
}

void
ShaderTest::uInterfaceUpdate()
{
  SceneManager& sm = g_SceneManager().instance();
  UInterface& im = g_uInterface().instance();

  im.setCurrentContext();
  im.newFrameAPI();
  im.windowNewFrame();
  im.uINewFrame();

  float yOffset = 0.0f;
  float winWidth = 450.0f;
  Vector2 winRect = m_window.getClientWidthHeight();

  float winAlpha = 0.4f;
  SPtr<Scene> currentScene = sm.getActiveScene();
  
  // --- Scene graph window --- //
  im.setNewWindowSize(Vector2(winRect.x * 0.1f, winRect.y));
  im.setNextWindowPos(Vector2(0.0f));
  im.SetNextWindowAlpha(winAlpha);
  im.startWindowCreate("Scene");
  if (im.createButton("+")) {
    sm.getActiveScene()->instantiate("Actor");
  }
  if (m_selectedActor) { 
    im.sameLine();
    if (im.createButton("Delete")) {
      m_selectedActor->~Actor();
      m_selectedActor = nullptr;
      m_sActorIndex = 0;
      currentScene->m_actors.erase(currentScene->m_actors.begin() + m_sActorIndex);
    }
    im.sameLine();
    if (im.createButton("^")) {
      m_selectedActor = nullptr;
      m_sActorIndex = 0;
    }
  }
  uint32 actorCount = currentScene->getActorCount();
  for (uint32 i = 0; i < actorCount; ++i) {
    SPtr<Actor> currentActor = currentScene->getActor(i);
    if (im.createButton(currentActor->getName(),
                        Color(0, 0, 0, 0),
                        Color(50, 50, 50, 50),
                        Color(100, 100, 100, 50),
                        true)) {
      m_selectedActor = currentActor;
      m_sActorIndex = i;
    }
  }
  im.endWindowCreate();
  // -------------------------- //

  float winHeight = 0.0f;
  // --- Transform window --- //
  if (m_selectedActor) {
    winHeight = 120.0f;
    im.setNewWindowSize(Vector2(winWidth, winHeight));
    im.setNextWindowPos(Vector2(im.getDisplaySize().x - winWidth, yOffset));
    im.SetNextWindowAlpha(winAlpha);
    im.startWindowCreate("Transform");

    ActorInspector inspector(m_selectedActor);
    
    im.endWindowCreate();
    yOffset += winHeight;

    // ---- Components window ---- //
    winHeight = 180.0f;
    im.setNewWindowSize(Vector2(winWidth, winHeight));
    im.setNextWindowPos(Vector2(im.getDisplaySize().x - winWidth, yOffset));
    im.SetNextWindowAlpha(winAlpha);
    im.startWindowCreate("Components");
    im.createButton("Add Component(non-functional)");
    for (uint32 i = 0; i < m_selectedActor->getComponents().size(); ++i) {
      inspector.createComponentWindow(m_selectedActor->getComponents()[i], m_selectedActor->m_transform);
    }
    im.endWindowCreate();
    yOffset += winHeight;
    // --------------------------- //
  }
  // -------------------------- //


  // get framerate
  float f_fps = 1.0f / g_TimeManager().m_deltaTime;
  uint32 fps = static_cast<uint32>(f_fps);
  String fpsStr = "FPS: " + to_string(fps);
  String camSpeed = "Camera Speed: " + to_string(static_cast<uint32>(m_cameraSpeed));

  // FPS parameters
  static const int fpsListSize = 100;
  static float fpsHistory[fpsListSize] = {};
  static int fpsOffset = 0;

  // Record the current FPS
  fpsHistory[fpsOffset] = f_fps;
  fpsOffset = (fpsOffset + 1) % fpsListSize;

  // --- Display window --- //
  winHeight = 75.0f;
  im.setNewWindowSize(Vector2(winWidth, winHeight));
  im.setNextWindowPos(Vector2(im.getDisplaySize().x - winWidth, yOffset));
  im.SetNextWindowAlpha(winAlpha);
  im.startWindowCreate("Display");
  im.createText(fpsStr.c_str());
  im.sameLine();
  im.plotLines("|", fpsHistory, fpsListSize, fpsOffset);
  // vSync
  im.createText("vSync");
  im.sameLine();
  im.createCheckBox("##vSync", m_vSync);
  im.endWindowCreate();
  yOffset += winHeight;
  // -------------------------- //

  // --- Camera window --- //
  winHeight = 125.0f;
  im.setNewWindowSize(Vector2(winWidth, winHeight));
  im.setNextWindowPos(Vector2(im.getDisplaySize().x - winWidth, yOffset));
  im.SetNextWindowAlpha(winAlpha);
  im.startWindowCreate("Editor Camera");
  // camera speed
  im.createText("Speed        ");
  im.sameLine();
  im.createDragF("##Speed", m_cameraSpeed);
  // X Sensitivity
  im.createText("X Sensitivity");
  im.sameLine();
  im.createDragF("##XSens", m_sensX, 0.1f);
  // Y Sensitivity
  im.createText("Y Sensitivity");
  im.sameLine();
  im.createDragF("##YSens", m_sensY, 0.1f);
  im.endWindowCreate();
  yOffset += winHeight;
  // -------------------------- //

  // --- Post-Process window --- //
  winHeight = 150.0f;
  im.setNewWindowSize(Vector2(winWidth, winHeight));
  im.setNextWindowPos(Vector2(im.getDisplaySize().x - winWidth, yOffset));
  im.SetNextWindowAlpha(winAlpha);
  im.startWindowCreate("Render");
  // shadows option
  im.createText("shadows  ");
  im.sameLine();
  im.createCheckBox("##Shadows", m_shadows);
  // Specular option
  im.createText("Specular ");
  im.sameLine();
  im.createCheckBox("##Specular", m_specular);
  // AO
  im.createText("AO       ");
  im.sameLine();
  im.createCheckBox("##AO", m_AO);
  // Luminance
  im.createText("Luminance");
  im.sameLine();
  im.createCheckBox("##Luminance", m_luminance);
  // compile shaders
  if (im.createButton("Compile Shaders")) {
    g_RenderManager().compileShaders();
  }
  im.endWindowCreate();
  // -------------------------- //

  im.render();
}

void
ShaderTest::onUpdate()
{
  // user input
  if (m_window.m_isFocused) {
    input();
  }

  // managers
  GraphicsAPI& api = g_GraphicAPI().instance();
  RendererManager& rm = g_RenderManager().instance();

  Vector2 winSize = api.getSwapChain()->getBuffer(0)->getSize();

  // camera data
  SPtr<Camera> camera = m_camera->getComponent<Camera>();
  Matrix4 view = Matrix4::IDENTITY;
  Matrix4 proj = Matrix4::IDENTITY;
  Matrix4 invView = Matrix4::IDENTITY;
  Matrix4 invProj = Matrix4::IDENTITY;
  // main camera buffer
  CBCamera cBCamera;
  CBShadowParam shadowsParam;
  shadowsParam.farNear = Vector2(0.0f);
  shadowsParam.winSize = winSize; // to do: win size could change, swap this to use the specific texture size.
  // to do: change this to another method
  if (camera) {
    view = camera->m_view.getTransposed();
    proj = camera->m_projection.getTransposed();
    invView = view.inverse();
    invProj = proj.inverse();
    CreateCBCamera::create(cBCamera, camera);

    shadowsParam.farNear = camera->m_farNear;
  }
  // light data
  SPtr<Light> light = m_light->getComponent<Light>();
  SPtr<Camera> lightCamera = m_light->getComponent<Camera>();
  // light buffers
  CBLight cBLight;
  CBCamera cBLightCam;

  // update shadow depth map buffers
  Matrix4 lightView = Matrix4::IDENTITY;
  Matrix4 lightProj = Matrix4::IDENTITY;
  // to do: change this to another method
  if (light) {
    lightView = lightCamera->m_view.getTransposed();
    lightProj = lightCamera->m_projection.getTransposed();
    CreateCBLight::create(cBLight, light);
    CreateCBCamera::create(cBLightCam, lightCamera);
  }

  // luminance parameters
  CBLuminance lum;
  lum.tolerance = 0.9f;
  // blur parameters
  CBBlur blur;
  blur.winSize = winSize;

  // data type sizes
  uint32 m4x4Size = sizeof(Matrix4);
  uint32 cBCamSize = sizeof(CBCamera);
  uint32 cBLightSize = sizeof(CBLight);

  // get all passes
  SPtr<Pass> baseShadow = rm.getPass(kP_Shadow);
  SPtr<Pass> basePass = rm.getPass(kP_Base);
  SPtr<Pass> luminancePass = rm.getPass(kP_Luminance);
  SPtr<Pass> hBlurPass = rm.getPass(kP_CHBlur);
  // SPtr<Pass> vBlurPass = rm.getPass(kP_CVBlur);
  SPtr<Pass> tonePass = rm.getPass(kP_Tone);
  SPtr<Pass> pCShadowPass = rm.getPass(kP_CShadows);
  SPtr<Pass> pCSpecPass = rm.getPass(kP_CSpecular);
  SPtr<Pass> skyBoxPass = rm.getPass(kP_SkyBox);

  // update normal && base shadow pass buffers
  api.updateConstantBuffer(basePass->getCBuffer(0), &view, m4x4Size);
  api.updateConstantBuffer(basePass->getCBuffer(1), &proj, m4x4Size);
  api.updateConstantBuffer(basePass->getCBuffer(3), &cBLight, cBLightSize);
  api.updateConstantBuffer(basePass->getCBuffer(4), &cBCamera, cBCamSize);

  api.updateConstantBuffer(baseShadow->getCBuffer(0), &lightView, m4x4Size);
  api.updateConstantBuffer(baseShadow->getCBuffer(1), &lightProj, m4x4Size);
  api.updateConstantBuffer(baseShadow->getCBuffer(3), &cBLight, cBLightSize);
  api.updateConstantBuffer(baseShadow->getCBuffer(4), &cBLightCam, cBCamSize);

  // update shadow compute buffers
  api.updateConstantBuffer(pCShadowPass->getCBuffer(0), &cBLight, cBLightSize);
  api.updateConstantBuffer(pCShadowPass->getCBuffer(1), &cBCamera, cBCamSize);
  api.updateConstantBuffer(pCShadowPass->getCBuffer(2), &cBLightCam, cBCamSize);
  api.updateConstantBuffer(pCShadowPass->getCBuffer(3), &invProj, m4x4Size);
  api.updateConstantBuffer(pCShadowPass->getCBuffer(4), &invView, m4x4Size);
  api.updateConstantBuffer(pCShadowPass->getCBuffer(5), &shadowsParam, sizeof(CBShadowParam));

  // update specular compute buffers
  api.updateConstantBuffer(pCSpecPass->getCBuffer(0), &cBLight, cBLightSize);
  api.updateConstantBuffer(pCSpecPass->getCBuffer(1), &cBCamera, cBCamSize);
  api.updateConstantBuffer(pCSpecPass->getCBuffer(2), &cBLightCam, cBCamSize);
  api.updateConstantBuffer(pCSpecPass->getCBuffer(3), &invProj, m4x4Size);
  api.updateConstantBuffer(pCSpecPass->getCBuffer(4), &invView, m4x4Size);
  api.updateConstantBuffer(pCSpecPass->getCBuffer(5), &shadowsParam, sizeof(CBShadowParam));

  // update the luminance pass buffer
  api.updateConstantBuffer(luminancePass->getCBuffer(0), &lum, sizeof(CBLuminance));
  // update the Horizontal/Vertical blur pass buffer
  api.updateConstantBuffer(hBlurPass->getCBuffer(0), &blur, sizeof(CBBlur));
  //      api.updateConstantBuffer(vBlurPass->getCBuffer(0), &blur, sizeof(CBBlur));

  // skybox constant buffer
  Matrix4 transform = Matrix4::IDENTITY;
  Matrix4 invViewProj = (camera->m_projection * camera->m_view).inverse();
  api.updateConstantBuffer(skyBoxPass->getCBuffer(0), &cBCamera, cBCamSize);
  api.updateConstantBuffer(skyBoxPass->getCBuffer(1), &transform, m4x4Size);
  api.updateConstantBuffer(skyBoxPass->getCBuffer(2), &invViewProj, m4x4Size);
}

void
ShaderTest::onRender()
{
  // update the user interface
  uInterfaceUpdate();
}
