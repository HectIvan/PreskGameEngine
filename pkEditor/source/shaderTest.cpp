#include "ActorInspector.h"
#include "pkLogger.h"
#include "pkGraphicsAPI.h"
#include "pkGraphicTypes.h"
#include "pkUInterface.h"
#include "pkLogger.h"
#include "pkPlatformMath.h"
#include "pkPath.h"
#include "pkRendererManager.h"
#include "pkResourceManager.h"
#include "pkSceneManager.h"
#include "pkTextureManager.h"
#include "pkTimeManager.h"
#include "pkGraphicsAPI.h"
#include "ShaderTest.h"

using pkEngineSDK::CBBlur;
using pkEngineSDK::CBLuminance;
using pkEngineSDK::CBShadowParam;
using pkEngineSDK::D_BUFFERS::kDB_Base;
using pkEngineSDK::D_BUFFERS::kDB_Shadow;
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
using pkEngineSDK::PASS_TYPE::kP_HBlur;
using pkEngineSDK::PASS_TYPE::kP_Luminance;
using pkEngineSDK::PASS_TYPE::kP_Shadow;
using pkEngineSDK::PASS_TYPE::kP_ShadowDef;
using pkEngineSDK::PASS_TYPE::kP_Tone;
using pkEngineSDK::PASS_TYPE::kP_VBlur;
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
                                         3000.0f,
                                         camPos, // position
                                         Vector3::FORWARD + camPos * -1.0f, // target
                                         Vector3(0.0f, 1.0f, 0.0f)); // up vector
  // camera sensitivity
  m_sensX = 0.3f;
  m_sensY = 0.3f;

  // create light
  light = g_SceneManager().getActiveScene()->instantiate("Test Light");
  light->addComponent(make_shared<Light>());
  SPtr<Light> lightCom = light->getComponent<Light>();
  lightCom->Type = pkEngineSDK::LIGHT_TYPE::kDirectional;
  lightCom->SpotCutoff = 0.90f;
  lightCom->SpotExponent = 32.0f;
  lightCom->LightDir = Vector3(0, 1.0f, 0);
  lightCom->LightPos = Vector3(0.0f, 50.0f, 0.0f);
  lightCom->LightColor = Vector3(1.0f);
  lightCom->shadowIntensity = 0.85f;

  // add camera component
  light->addComponent(make_shared<Camera>());
  light->getComponent<Camera>()->init(1280,
                                      720,
                                      3.1416f / 4.0f,
                                      0.01f,
                                      2000.0f,
                                      lightCom->LightPos, // position
                                      lightCom->LightDir, // target
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

  m_shadows = true;
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
  float deltaTime = g_TimeManager().m_deltaTime;
  // set camera speed with deltaTime
  float speed = m_cameraSpeed * deltaTime;
  // move forward/backward
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kW) && m_window.m_isFocused) {
    m_camera->getComponent<Camera>()->moveForwardLocal(speed);
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kS) && m_window.m_isFocused) {
    m_camera->getComponent<Camera>()->moveForwardLocal(-speed);
  }
  // move left/right
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kA) && m_window.m_isFocused) {
    m_camera->getComponent<Camera>()->moveRightLocal(-speed);
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kD) && m_window.m_isFocused) {
    m_camera->getComponent<Camera>()->moveRightLocal(speed);
  }
  // move up/down
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kE)) {
    m_camera->getComponent<Camera>()->moveUpLocal(speed);
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kQ)) {
    m_camera->getComponent<Camera>()->moveUpLocal(-speed);
  }
  // rotate camera
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kLButton) && m_window.m_isFocused) {
    Vector2 posDif = (m_lastCursorPos - eventQueue.mousePosition);
    posDif.x *= m_sensX;
    posDif.y *= m_sensY;
    m_lastCursorPos = eventQueue.mousePosition;
    m_camera->getComponent<Camera>()->rotate(-posDif.y * Math::DEG2RAD, 0.0f, 0.0f);
    m_camera->getComponent<Camera>()->rotate(0.0f, posDif.x * Math::DEG2RAD, 0.0f);
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
  float winWidth = 420.0f;
  Vector2 winRect = m_window.getClientWidthHeight();
  
  // --- Scene graph window --- //
  im.setNewWindowSize(Vector2(winRect.x * 0.1f, winRect.y));
  im.setNextWindowPos(Vector2(0.0f));
  im.startWindowCreate("Scene");
  uint32 actorCount = sm.getActiveScene()->getActorCount();
  for (uint32 i = 0; i < actorCount; ++i) {
    if (im.createButton(sm.getActiveScene()->getActor(i)->getNameCSTR())) {
      m_selectedActor = sm.getActiveScene()->getActor(i);
    }
  }
  im.endWindowCreate();
  // -------------------------- //

  float winHeight = 0.0f;
  // --- Transform window --- //
  if (m_selectedActor) {
    winHeight = 100.0f;
    im.setNewWindowSize(Vector2(winWidth, winHeight));
    im.setNextWindowPos(Vector2(im.getDisplaySize().x - winWidth, yOffset));
    im.startWindowCreate("Transform");

    ActorInspector inspector(m_selectedActor);
    
    im.endWindowCreate();
    yOffset += winHeight;

    // ---- Components window ---- //
    winHeight = 180.0f;
    im.setNewWindowSize(Vector2(winWidth, winHeight));
    im.setNextWindowPos(Vector2(im.getDisplaySize().x - winWidth, yOffset));
    im.startWindowCreate("Components");
    for (uint32 i = 0; i < m_selectedActor->getComponents().size(); ++i) {
      inspector.createComponentWindow(m_selectedActor->getComponents()[i]);
    }
    im.endWindowCreate();
    yOffset += winHeight;
    // --------------------------- //
  }
  // -------------------------- //


  // get framerate
  uint32 fps = static_cast<uint32>(1.0f / g_TimeManager().m_deltaTime);
  String fpsStr = "FPS: " + to_string(fps);
  String camSpeed = "Camera Speed: " + to_string(static_cast<uint32>(m_cameraSpeed));

  // --- Display window --- //
  winHeight = 75.0f;
  im.setNewWindowSize(Vector2(winWidth, winHeight));
  im.setNextWindowPos(Vector2(im.getDisplaySize().x - winWidth, yOffset));
  im.startWindowCreate("Display");
  im.createText(fpsStr.c_str());
  im.createCheckBox("vSync", m_vSync);
  im.endWindowCreate();
  yOffset += winHeight;
  // -------------------------- //

  // --- Camera window --- //
  winHeight = 125.0f;
  im.setNewWindowSize(Vector2(winWidth, winHeight));
  im.setNextWindowPos(Vector2(im.getDisplaySize().x - winWidth, yOffset));
  im.startWindowCreate("Camera");
  im.createInputF("Speed", m_cameraSpeed);
  im.createInputF("X Sensitivity", m_sensX, 1.0f, 10.0f);
  im.createInputF("Y Sensitivity", m_sensY, 1.0f, 10.0f);
  im.endWindowCreate();
  yOffset += winHeight;
  // -------------------------- //

  // --- Post-Process window --- //
  winHeight = 125.0f;
  im.setNewWindowSize(Vector2(winWidth, winHeight));
  im.setNextWindowPos(Vector2(im.getDisplaySize().x - winWidth, yOffset));
  im.startWindowCreate("Render");
  im.createCheckBox("Shadows", m_shadows);
  im.createCheckBox("Ambient Oclussion", m_AO);
  im.createCheckBox("Luminance", m_luminance);
  if (im.createButton("Compile Shaders")) {
    g_RenderManager().compileShaders();
  }
  im.endWindowCreate();
  yOffset += winHeight;
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

  // camera data
  SPtr<Camera> camData = m_camera->getComponent<Camera>();
  Matrix4 view = camData->m_view.getTransposed();
  Matrix4 proj = camData->m_projection.getTransposed();
  Matrix4 invView = view.inverse();
  Matrix4 invProj = proj.inverse();
  // light data
  SPtr<Light> lightData = light->getComponent<Light>();
  CBLight lData;
  lData.LightDir = Vector4(lightData->LightDir, 1.0f);
  lData.LightPos = Vector4(lightData->LightPos, 1.0f);
  lData.LightColor = Vector4(lightData->LightColor, 1.0f);
  lData.shadowIntensity = lightData->shadowIntensity;
  lData.spotExponent = lightData->SpotExponent;
  // luminance parameters
  CBLuminance lum;
  lum.tolerance = 0.9f;
  // blur parameters
  CBBlur blur;
  blur.targetSize = Vector2(10.0f, 10.0f);

  // data type sizes
  uint32 m4x4Size = sizeof(Matrix4);
  uint32 camSize = sizeof(Camera);
  uint32 lightSize = sizeof(Light);

  // update normal pass buffers
  api.updateConstantBuffer(rm.getPass(kP_Base)->getCBuffer(0), &view, m4x4Size);
  api.updateConstantBuffer(rm.getPass(kP_Base)->getCBuffer(1), &proj, m4x4Size);
  api.updateConstantBuffer(rm.getPass(kP_Base)->getCBuffer(3), &lightData, lightSize);
  api.updateConstantBuffer(rm.getPass(kP_Base)->getCBuffer(4), &camData, camSize);

  // update shadow depth map buffers
  SPtr<Camera> lightCam = light->getComponent<Camera>();
  Matrix4 lightView = lightCam->m_view.getTransposed();
  Matrix4 lightProj = lightCam->m_projection.getTransposed();
  api.updateConstantBuffer(rm.getPass(kP_Shadow)->getCBuffer(0), &lightView, m4x4Size);
  api.updateConstantBuffer(rm.getPass(kP_Shadow)->getCBuffer(1), &lightProj, m4x4Size);
  api.updateConstantBuffer(rm.getPass(kP_Shadow)->getCBuffer(3), &lightData, lightSize);
  api.updateConstantBuffer(rm.getPass(kP_Shadow)->getCBuffer(4), &lightCam,  camSize);

  // update shadow quad buffers
  // light buffer data
  api.updateConstantBuffer(rm.getPass(kP_ShadowDef)->getCBuffer(0),
                           &lData,
                           sizeof(CBLight));
  // main camera data
  api.updateConstantBuffer(rm.getPass(kP_ShadowDef)->getCBuffer(1), &camData, sizeof(camData));
  // light camera data
  api.updateConstantBuffer(rm.getPass(kP_ShadowDef)->getCBuffer(2),
                           &lightCam,
                           sizeof(Camera));
  // inverse projection data
  api.updateConstantBuffer(rm.getPass(kP_ShadowDef)->getCBuffer(3),
                           &invProj,
                           sizeof(Matrix4));
  // inverse view data
  api.updateConstantBuffer(rm.getPass(kP_ShadowDef)->getCBuffer(4),
                           &invView,
                           sizeof(Matrix4));
  // get the shadow data needed
  CBShadowParam shadowsParam;
  shadowsParam.farNear = m_camera->getComponent<Camera>()->m_farNear;
  shadowsParam.winSize = api.getSwapChain()->getBuffer(0)->getSize();
  api.updateConstantBuffer(rm.getPass(kP_ShadowDef)->getCBuffer(5),
                           &shadowsParam,
                           sizeof(CBShadowParam));
  // update the luminance pass buffer
  api.updateConstantBuffer(rm.getPass(kP_Luminance)->getCBuffer(0), &lum, sizeof(CBLuminance));
  // update the Horizontal/Vertical blur pass buffer
  api.updateConstantBuffer(rm.getPass(kP_HBlur)->getCBuffer(0), &blur, sizeof(CBBlur));
  api.updateConstantBuffer(rm.getPass(kP_VBlur)->getCBuffer(0), &blur, sizeof(CBBlur));
}

void
ShaderTest::onRender()
{
  // update the user interface
  uInterfaceUpdate();
}
