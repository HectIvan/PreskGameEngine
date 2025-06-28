#include "ShaderTest.h"
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

using pkEngineSDK::CBBlur;
using pkEngineSDK::CBLuminance;
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
  // g_uInterface().initWin(m_window.getWindowHandle());
  ResourceManager& resourceMan = g_ResourceManager().instance();

  // create camera
  m_baseCamSpeed = 20.0f;
  m_cameraSpeed = m_baseCamSpeed;
  m_maxCamSpeed = 200.0f;
  m_camAccelerate = 30.0f;
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

  // create light
  light = g_SceneManager().getActiveScene()->instantiate("Test Light");
  light->addComponent(make_shared<Light>());
  SPtr<Light> lightCom = light->getComponent<Light>();
  lightCom->Type = pkEngineSDK::LIGHT_TYPE::kDirectional;
  lightCom->SpotCutoff = 0.90f;
  lightCom->SpotExponent = 32.0f;
  lightCom->LightDir = Vector3(0.0f, -1.0f, 0.0f);
  lightCom->LightPos = Vector3(0.0f, 50.0f, 0.0f);
  lightCom->LightColor = Vector3(1.0f, 1.0f, 1.0f);

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

  SPtr<Actor> sponza = g_SceneManager().getActiveScene()->instantiate("Sponza");
  sponza->addComponent(resourceMan.loadModel(Path("models/sponza.obj")));

  m_shadows = false;
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
  // update the camera m_speed
  m_cameraSpeed += m_camAccelerate * deltaTime * eventQueue.scrollWheel;
  m_cameraSpeed = Math::clamp(m_cameraSpeed, 0.0f, m_maxCamSpeed);
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
    Vector2 posDif = (m_lastCursorPos - eventQueue.mousePosition) * deltaTime;
    m_lastCursorPos = eventQueue.mousePosition;
    m_camera->getComponent<Camera>()->rotate(-posDif.y, 0.0f, 0.0f);
    m_camera->getComponent<Camera>()->rotate(0.0f, posDif.x, 0.0f);
  }
  m_lastCursorPos = eventQueue.mousePosition;
  // compile shaders
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kC) && m_window.m_isFocused) {
    g_RenderManager().compileShaders();
  }
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
    im.createText(sm.getActiveScene()->getActor(i)->getNameCSTR());
  }
  im.endWindowCreate();
  // -------------------------- //

  // --- Transform window --- //
  Vector3 testPos = Vector3(0.0f);
  im.setNewWindowSize(Vector2(winWidth, 300.0f));
  im.setNextWindowPos(Vector2(im.getDisplaySize().x - winWidth, yOffset));
  im.startWindowCreate("Transform");
  im.createSliderVector3("Position",
                         testPos,
                         -2147483648.0f,
                         2147483647.0f);
  im.endWindowCreate();
  yOffset += 300;
  // -------------------------- //

  // get framerate
  uint32 fps = static_cast<uint32>(1.0f / g_TimeManager().m_deltaTime);
  String fpsStr = "FPS: " + to_string(fps);
  String camSpeed = "Camera Speed: " + to_string(static_cast<uint32>(m_cameraSpeed));

  // --- Display window --- //
  im.setNewWindowSize(Vector2(winWidth, 75.0f));
  im.setNextWindowPos(Vector2(im.getDisplaySize().x - winWidth, yOffset));
  im.startWindowCreate("Display");
  im.createText(fpsStr.c_str());
  im.createCheckBox("vSync", m_vSync);
  im.endWindowCreate();
  yOffset += 75.0f;
  // -------------------------- //

  // --- Camera window --- //
  im.setNewWindowSize(Vector2(winWidth, 75.0f));
  im.setNextWindowPos(Vector2(im.getDisplaySize().x - winWidth, yOffset));
  im.startWindowCreate("Camera");
  im.createSliderF("Camera Acceleration", m_camAccelerate, 0.0f, m_maxCamSpeed);
  im.createText(camSpeed.c_str());
  im.endWindowCreate();
  yOffset += 75.0f;
  // -------------------------- //

  // --- Post-Process window --- //
  im.setNewWindowSize(Vector2(winWidth, 300.0f));
  im.setNextWindowPos(Vector2(im.getDisplaySize().x - winWidth, yOffset));
  im.startWindowCreate("Render");
  im.createCheckBox("Shadows", m_shadows);
  im.createCheckBox("Ambient Oclussion", m_AO);
  im.endWindowCreate();
  yOffset += 300.0f;
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

  // buffer data
  SPtr<Camera> camData = m_camera->getComponent<Camera>();
  Matrix4 view = camData->m_view.getTransposed();
  Matrix4 proj = camData->m_projection.getTransposed();
  SPtr<Light> lightData = light->getComponent<Light>();
  CBLuminance lum;
  lum.tolerance = 0.9f;

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

  // update shadow map buffers
  SPtr<Camera> lightCam = light->getComponent<Camera>();
  Matrix4 lightView = lightCam->m_view.getTransposed();
  Matrix4 lightProj = lightCam->m_projection.getTransposed();
  api.updateConstantBuffer(rm.getPass(kP_Shadow)->getCBuffer(0), &lightView, m4x4Size);
  api.updateConstantBuffer(rm.getPass(kP_Shadow)->getCBuffer(1), &lightProj, m4x4Size);
  api.updateConstantBuffer(rm.getPass(kP_Shadow)->getCBuffer(3), &lightData, lightSize);
  api.updateConstantBuffer(rm.getPass(kP_Shadow)->getCBuffer(4), &lightCam,  camSize);

  // update shadow deferred buffers
  SPtr<Camera> tempLightCam = light->getComponent<Camera>();
  SPtr<Camera> mainCam = m_camera->getComponent<Camera>();
  api.updateConstantBuffer(rm.getPass(kP_ShadowDef)->getCBuffer(0), &tempLightCam, camSize);
  api.updateConstantBuffer(rm.getPass(kP_ShadowDef)->getCBuffer(1), &mainCam, camSize);
  api.updateConstantBuffer(rm.getPass(kP_ShadowDef)->getCBuffer(2), 
                           &light->m_transform,
                           m4x4Size);
  api.updateConstantBuffer(rm.getPass(kP_ShadowDef)->getCBuffer(3),
                           &m_camera->m_transform,
                           camSize);

  api.updateConstantBuffer(rm.getPass(kP_Luminance)->getCBuffer(0), &lum, sizeof(lum));

  api.updateConstantBuffer(rm.getPass(kP_HBlur)->getCBuffer(0), &blur, sizeof(blur));
  api.updateConstantBuffer(rm.getPass(kP_VBlur)->getCBuffer(0), &blur, sizeof(blur));
}

// to do: fix the deferred renderer to be able to show the final result
void
ShaderTest::onRender()
{
  // update the user interface
  uInterfaceUpdate();
}
