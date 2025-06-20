#include "ShaderTest.h"
#include "pkLogger.h"
#include "pkGraphicsAPI.h"
#include "pkGraphicTypes.h"
#include "pkUInterface.h"
#include "pkLogger.h"
#include "pkPath.h"
#include "pkRendererManager.h"
#include "pkResourceManager.h"
#include "pkSceneManager.h"
#include "pkTextureManager.h"
#include "pkTimeManager.h"
#include "pkGraphicsAPI.h"

using pkEngineSDK::CBLuminance;
using pkEngineSDK::D_BUFFERS::kDB_Base;
using pkEngineSDK::D_BUFFERS::kDB_Shadow;
using pkEngineSDK::GraphicsAPI;
using pkEngineSDK::G_BUFFERS::kGB_Albedo;
using pkEngineSDK::G_BUFFERS::kGB_Normal;
using pkEngineSDK::g_GraphicAPI;
using pkEngineSDK::g_uInterface;
using pkEngineSDK::g_Logger;
using pkEngineSDK::g_RenderManager;
using pkEngineSDK::g_ResourceManager;
using pkEngineSDK::g_SceneManager;
using pkEngineSDK::g_TextureManager;
using pkEngineSDK::g_TimeManager;
using pkEngineSDK::UInterface;
using pkEngineSDK::Light;
using pkEngineSDK::Logger;
using pkEngineSDK::Material;
using pkEngineSDK::Matrix4;
using pkEngineSDK::Path;
using pkEngineSDK::PASS_TYPE::kP_AO;
using pkEngineSDK::PASS_TYPE::kP_Base;
using pkEngineSDK::PASS_TYPE::kP_Shadow;
using pkEngineSDK::PASS_TYPE::kP_ShadowDef;
using pkEngineSDK::PASS_TYPE::kP_Tone;
using pkEngineSDK::PASS_TYPE::kP_Luminance;
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

void
ShaderTest::onInit()
{
  //start the interface
  UInterface::startUp();
  g_uInterface().init();
  g_uInterface().initWin(m_window.getWindowHandle());
  ResourceManager& resourceMan = g_ResourceManager().instance();

  m_cameraSpeed = 20.0f;
  m_camera = g_SceneManager().getActiveScene()->instantiate("Main Camera");
  m_camera->addComponent(make_shared<Camera>());
  m_camera->getComponent<Camera>()->init(m_window.getWidth(),
                                         m_window.getHeight(),
                                         3.1416f / 4.0f,
                                         0.01f,
                                         2000.0f,
                                         Vector3(0.0f, 0.0f, -30.0f), // position
                                         Vector3(0.0f, 0.0f, 1.0f), // target
                                         Vector3(0.0f, 1.0f, 0.0f)); // up vector

  // create light
  light = g_SceneManager().getActiveScene()->instantiate("Test Light");
  // add light component
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
ShaderTest::input()
{
  float deltaTime = g_TimeManager().m_deltaTime;
  // update the camera m_speed
  float cam_speed = m_cameraSpeed * deltaTime;
  // move forward/backward
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kW)) {
    m_camera->getComponent<Camera>()->move(Vector3(0.0f, 0.0f, cam_speed));
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kS)) {
    m_camera->getComponent<Camera>()->move(Vector3(0.0f, 0.0f, -cam_speed));
  }
  // move left/right
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kA)) {
    m_camera->getComponent<Camera>()->move(Vector3(cam_speed, 0.0f, 0.0f));
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kD)) {
    m_camera->getComponent<Camera>()->move(Vector3(-cam_speed, 0.0f, 0.0f));
  }
  // move up/down
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kE)) {
    m_camera->getComponent<Camera>()->move(Vector3(0.0f, cam_speed, 0.0f));
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kQ)) {
    m_camera->getComponent<Camera>()->move(Vector3(0.0f, -cam_speed, 0.0f));
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kLButton)) {
    Vector2 posDif = (m_lastCursorPos - m_eventQueue.mousePosition) * deltaTime;
    // m_camera->rotate(-posDif.y, posDif.x, 0.0f);
    m_lastCursorPos = m_eventQueue.mousePosition;
  }
  else {
    m_lastCursorPos = m_eventQueue.mousePosition;
  }

  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kC) && m_window.m_isFocused) {
    g_RenderManager().compileShaders();
  }

  float rot = 1.0f * deltaTime;
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kLeft)) {
    // m_camera->getComponent<Camera>()->m_view *= Matrix4::rotationY(rot);
    m_camera->getComponent<Camera>()->rotate(Vector3(0.0f, rot, 0.0f));
  }
  else if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kRight)) {
    rot *= -1.0f;
    // m_camera->getComponent<Camera>()->m_view *= Matrix4::rotationY(rot);
    m_camera->getComponent<Camera>()->rotate(Vector3(0.0f, rot, 0.0f));
  }
  else if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kUp)) {
    rot *= -1.0f;
    m_camera->getComponent<Camera>()->m_view *= Matrix4::rotationX(rot);
  }
  else if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kDown)) {
    m_camera->getComponent<Camera>()->m_view *= Matrix4::rotationX(rot);
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

  Vector2 winRect = m_window.getClientWidthHeight();
  
  // --- Scene graph window --- //
  im.setNewWindowSize(Vector2(winRect.x * 0.2f, winRect.y));
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
  im.setNewWindowSize(Vector2(500.0f, 300.0f));
  im.setNextWindowPos(Vector2(im.getDisplaySize().x - 500.0f, 0.0f));
  im.startWindowCreate("Transform");
  im.createSliderVector3("Position",
                         testPos,
                         -2147483648.0f,
                         2147483647.0f);
  im.endWindowCreate();
  // -------------------------- //

  // get framerate
  uint32 fps = static_cast<uint32>(1.0f / g_TimeManager().m_deltaTime);
  String str = "FPS: " + to_string(fps);

  // --- Display window --- //
  testPos = Vector3(0.0f);
  im.setNewWindowSize(Vector2(500.0f, 100.0f));
  im.setNextWindowPos(Vector2(im.getDisplaySize().x - 500.0f, 300.0f));
  im.startWindowCreate("Display");
  im.createText(str.c_str());
  im.createCheckBox("vSync", m_vSync);
  im.endWindowCreate();
  // -------------------------- //

  // --- Post-Process window --- //
  testPos = Vector3(0.0f);
  im.setNewWindowSize(Vector2(500.0f, 300.0f));
  im.setNextWindowPos(Vector2(im.getDisplaySize().x - 500.0f, 400.0f));
  im.startWindowCreate("Render");
  im.createCheckBox("Shadows", m_shadows);
  im.createCheckBox("Ambient Oclussion", m_AO);
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

  // buffer data
  SPtr<Camera> camData = m_camera->getComponent<Camera>();
  Matrix4 view = camData->m_view.getTransposed();
  Matrix4 proj = camData->m_projection.getTransposed();
  SPtr<Light> lightData = light->getComponent<Light>();
  CBLuminance lum;
  lum.tolerance = 0.9f;

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
}

// to do: fix the deferred renderer to be able to show the final result
void
ShaderTest::onRender()
{
  // update the user interface
  uInterfaceUpdate();
}
