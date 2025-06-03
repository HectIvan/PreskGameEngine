#include "ShaderTest.h"
#include "pkLogger.h"
#include "pkGraphicsAPI.h"
#include "pkGraphicTypes.h"
#include "pkUInterface.h"
#include "pkLogger.h"
#include "pkRendererManager.h"
#include "pkSceneManager.h"
#include "pkTextureManager.h"
#include "pkTimeManager.h"
#include "pkGraphicsAPI.h"

using pkEngineSDK::GraphicsAPI;
using pkEngineSDK::g_GraphicAPI;
using pkEngineSDK::g_uInterface;
using pkEngineSDK::g_Logger;
using pkEngineSDK::g_RenderManager;
using pkEngineSDK::g_SceneManager;
using pkEngineSDK::g_TextureManager;
using pkEngineSDK::g_TimeManager;
using pkEngineSDK::UInterface;
using pkEngineSDK::Light;
using pkEngineSDK::Logger;
using pkEngineSDK::Material;
using pkEngineSDK::Matrix4;
using pkEngineSDK::to_string;
using pkEngineSDK::RendererManager;
using pkEngineSDK::Scene;
using pkEngineSDK::SceneManager;
using pkEngineSDK::SPtr;
using pkEngineSDK::String;
using pkEngineSDK::TEXTURE_FORMATS::kPK_FORMAT_R32G32B32_FLOAT;
using pkEngineSDK::TextureManager;
using pkEngineSDK::uint32;

void
ShaderTest::onInit()
{
  //start the interface
  UInterface::startUp();
  g_uInterface().init();
  g_uInterface().initWin(m_window.getWindowHandle());

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
  lightCom->LightDir = Vector3(0.0f, 0.0f, 1.0f);
  lightCom->LightPos = Vector3(0.0f, -50.0f, 0.0f);
  lightCom->LightColor = Vector3(1.0f, 1.0f, 1.0f);

  // add camera component
  light->addComponent(make_shared<Camera>());
  light->getComponent<Camera>()->init(30,
                                      17,
                                      3.1416f / 4.0f,
                                      0.01f,
                                      2000.0f,
                                      lightCom->LightPos, // position
                                      lightCom->LightDir, // target
                                      Vector3::UP,
                                      pkEngineSDK::CAMERA_PROJ::kOrthographic); // up vector);

  SPtr<Actor> pistol = g_SceneManager().getActiveScene()->instantiate("Pistol");
  pistol->addComponent(newModel("drakefire_pistol_low.obj"));

  SPtr<Actor> sponza = g_SceneManager().getActiveScene()->instantiate("Sponza");
  sponza->addComponent(newModel("sponza.obj"));

  m_shadows = true;
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

  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kC)) {
    g_RenderManager().compileShaders();
  }

  SPtr<Actor> actor = g_SceneManager().getActiveScene()->getActor(0);

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
ShaderTest::UInterfaceUpdate()
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
  input();

  // managers
  GraphicsAPI& api = g_GraphicAPI().instance();
  RendererManager& rm = g_RenderManager().instance();

  // buffer data
  SPtr<Camera> camData = m_camera->getComponent<Camera>();
  Matrix4 view = camData->m_view.getTransposed();
  Matrix4 proj = camData->m_projection.getTransposed();
  SPtr<Light> lightData = light->getComponent<Light>();

  // data type sizes
  uint32 m4x4Size = static_cast<uint32>(sizeof(Matrix4));
  uint32 camSize = static_cast<uint32>(sizeof(Camera));
  uint32 lightSize = static_cast<uint32>(sizeof(Light));

  // update normal pass buffers
  api.updateConstantBuffer(rm.m_passes[0]->getCBuffer(0), &view, m4x4Size);
  api.updateConstantBuffer(rm.m_passes[0]->getCBuffer(1), &proj, m4x4Size);
  api.updateConstantBuffer(rm.m_passes[0]->getCBuffer(3), &lightData, lightSize);
  api.updateConstantBuffer(rm.m_passes[0]->getCBuffer(4), &camData, camSize);

  // update shadow map buffers
  SPtr<Camera> lightCam = light->getComponent<Camera>();
  Matrix4 lightView = lightCam->m_view.getTransposed();
  Matrix4 lightProj = lightCam->m_projection.getTransposed();
  api.updateConstantBuffer(rm.m_passes[1]->getCBuffer(0), &lightView, m4x4Size);
  api.updateConstantBuffer(rm.m_passes[1]->getCBuffer(1), &lightProj, m4x4Size);
  api.updateConstantBuffer(rm.m_passes[1]->getCBuffer(3), &lightData, lightSize);
  api.updateConstantBuffer(rm.m_passes[1]->getCBuffer(4), &lightCam,  camSize);

  // update shadow deferred buffers
  SPtr<Camera> tempLightCam = light->getComponent<Camera>();
  SPtr<Camera> mainCam = m_camera->getComponent<Camera>();
  api.updateConstantBuffer(rm.m_passes[3]->getCBuffer(0), &tempLightCam, camSize);
  api.updateConstantBuffer(rm.m_passes[3]->getCBuffer(1), &mainCam, camSize);
  api.updateConstantBuffer(rm.m_passes[3]->getCBuffer(2), &light->m_transform, m4x4Size);
  api.updateConstantBuffer(rm.m_passes[3]->getCBuffer(3), &m_camera->m_transform, camSize);
}

void
ShaderTest::onRender()
{
  // screen clear color
  float clearColor[4] = { 0.0f, 0.123f, 0.3f, 1.0f };
  GraphicsAPI& api = g_GraphicAPI().instance();
  RendererManager& rm = g_RenderManager().instance();

  Vector<SPtr<Texture>> rTVector;
  /**
   * Shadow Mapping.
   */
  if (m_shadows) {
    rTVector.push_back(rm.m_pRTargetView);
    rTVector.push_back(rm.m_pNormalRT);
    rTVector.push_back(rm.m_pShadowDepth);

    // set the render targets
    api.setRenderTargets(rTVector, rm.m_pShadowDepthSV);

    api.clearRenderTargetView(clearColor, rm.m_pRTargetView);
    api.clearDepthBuffer(1.0f, rm.m_pShadowDepthSV);
    
    api.setPSShader(rm.m_passes.find(1)->second->getPShader());
    api.setVSShader(rm.m_passes.find(1)->second->getVShader());
    api.setSampler(rm.m_passes.find(1)->second->getSamplerState());
    
    // set constant buffers for the pixel and vertex shaders
    rm.PSSetConstantBuffers(rm.m_passes.find(1)->second->getCBuffers());
    rm.VSSetConstantBuffers(rm.m_passes.find(1)->second->getCBuffers());
    
    rm.renderActors(g_SceneManager().getActiveScene()->getAllActors());
  }

  /**
   * Normal Render.
   */
  rTVector.clear();
  rTVector.push_back(rm.m_pRTargetView);
  rTVector.push_back(rm.m_pNormalRT);
  rTVector.push_back(rm.m_pDepthRT);

  // set the render targets
  api.setRenderTargets(rTVector, rm.m_pDepthSView);

  api.clearRenderTargetView(clearColor, rm.m_pRTargetView);
  api.clearDepthBuffer(1.0f, rm.m_pDepthSView);
  
  // set the base pass for the first rendering stage
  api.setPSShader(rm.m_passes.find(0)->second->getPShader());
  api.setVSShader(rm.m_passes.find(0)->second->getVShader());
  api.setSampler(rm.m_passes.find(0)->second->getSamplerState());
  
  // set constant buffers for the pixel and vertex shaders
  rm.PSSetConstantBuffers(rm.m_passes.find(0)->second->getCBuffers());
  rm.VSSetConstantBuffers(rm.m_passes.find(0)->second->getCBuffers());
  // render the objects
  rm.renderActors(g_SceneManager().getActiveScene()->getAllActors());

  /**
   * Ambient Occlussion. (Deferred)
   */
  if (m_AO) {
    api.setPSShader(rm.m_passes.find(2)->second->getPShader());
    api.setVSShader(rm.m_passes.find(2)->second->getVShader());
    api.setSampler(rm.m_passes.find(2)->second->getSamplerState());
    
    api.PSSetShaderResourceView(rm.m_pRTargetView);
    
    // set constant buffers for the pixel and vertex shaders
    rm.PSSetConstantBuffers(rm.m_passes.find(2)->second->getCBuffers());
    rm.VSSetConstantBuffers(rm.m_passes.find(2)->second->getCBuffers());
    
    api.draw(3, 0);
  }

  /**
   * Shadow pass. (Deferred)
   */
  if (m_shadows) {
    rTVector.clear();
    rTVector.push_back(rm.m_pRTargetView);
    rTVector.push_back(rm.m_pNormalRT);
    rTVector.push_back(rm.m_pDepthRT);

    // set the render targets
    api.setRenderTargets(rTVector, rm.m_pDepthSView);

    api.setPSShader(rm.m_passes.find(3)->second->getPShader());
    api.setVSShader(rm.m_passes.find(3)->second->getVShader());
    api.setSampler(rm.m_passes.find(3)->second->getSamplerState());
    
    // set constant buffers for the pixel and vertex shaders
    rm.PSSetConstantBuffers(rm.m_passes.find(3)->second->getCBuffers());

    api.setShaderResourceView(rm.m_pShadowDepth, 0);
    api.setShaderResourceView(rm.m_pDepthRT, 1);

    api.draw(3, 0);
  }

  UInterfaceUpdate();
}
