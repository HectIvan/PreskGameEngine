#include "ShaderTest.h"
#include "pkLogger.h"
#include "pkGraphicsAPI.h"
#include "pkGraphicTypes.h"
#include "pkLogger.h"
#include "pkRendererManager.h"
#include "pkSceneManager.h"
#include "pkTextureManager.h"
#include "pkTimeManager.h"
#include "pkGraphicsAPI.h"

using pkEngineSDK::Logger;
using pkEngineSDK::GraphicsAPI;
using pkEngineSDK::g_GraphicAPI;
using pkEngineSDK::g_RenderManager;
using pkEngineSDK::g_TimeManager;
using pkEngineSDK::g_SceneManager;
using pkEngineSDK::g_TextureManager;
using pkEngineSDK::g_Logger;
using pkEngineSDK::Light;
using pkEngineSDK::Material;
using pkEngineSDK::Matrix4;
using pkEngineSDK::RendererManager;
using pkEngineSDK::Scene;
using pkEngineSDK::SPtr;
using pkEngineSDK::TEXTURE_FORMATS::kPK_FORMAT_R32G32B32_FLOAT;
using pkEngineSDK::TextureManager;
using pkEngineSDK::uint32;

void
ShaderTest::onInit()
{
  createConstantBuffers();

  RendererManager& rm = g_RenderManager().instance();

  m_camera = make_shared<Actor>();
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
  light = make_shared<Actor>();
  // add light component
  light->addComponent(make_shared<Light>());
  SPtr<Light> lightCom = light->getComponent<Light>();
  lightCom->Type = pkEngineSDK::LIGHT_TYPE::kDirectional;
  lightCom->SpotCutoff = 0.90f;
  lightCom->SpotExponent = 32.0f;
  lightCom->LightDir = Vector3::UP;
  lightCom->LightPos = Vector3(0.0f, 50.0f, 0.0f);
  lightCom->LightColor = Vector3(1.0f, 1.0f, 1.0f);

  // add camera component
  light->addComponent(make_shared<Camera>());
  light->getComponent<Camera>()->init(30,
                                      17,
                                      3.1416f / 4.0f,
                                      0.01f,
                                      1000.0f,
                                      lightCom->LightPos, // position
                                      lightCom->LightDir + lightCom->LightPos, // target
                                      Vector3::UP,
                                      pkEngineSDK::CAMERA_PROJ::kOrthographic); // up vector);

  m_cbCamera = rm.m_cbCamera;
  m_cBProjection = rm.m_cBProjection;
  m_cBTransform = rm.m_cBTransform;
  m_cBView = rm.m_cBView;
  m_cbLight = rm.m_cbLight;

  SPtr<Actor> pistol = g_SceneManager().getActiveScene()->instantiate();
  pistol->addComponent(newModel("drakefire_pistol_low.obj"));

  // SPtr<Actor> sponza = g_SceneManager().getActiveScene()->instantiate();
  // sponza->addComponent(newModel("sponza.obj"));
}

void
ShaderTest::input()
{
  float deltaTime = g_TimeManager().m_deltaTime;
  // update the camera m_speed
  float cam_speed = m_cameraSpeed * deltaTime;
  // move forward/backward
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kW)) {
    m_camera->getComponent<Camera>()->m_view *= Matrix4::translationA(Vector3(0, 0, cam_speed));
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kS)) {
    m_camera->getComponent<Camera>()->m_view *= Matrix4::translationA(Vector3(0, 0, -cam_speed));
  }
  // move left/right
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kA)) {
    m_camera->getComponent<Camera>()->m_view *= Matrix4::translationA(Vector3(cam_speed, 0, 0));
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kD)) {
    m_camera->getComponent<Camera>()->m_view *= Matrix4::translationA(Vector3(cam_speed, 0, 0));
  }
  // move up/down
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kE) ||
    m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kSpace)) {
    m_camera->getComponent<Camera>()->m_view *= Matrix4::translationA(Vector3(0, cam_speed, 0));
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kQ) ||
    m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kLControl)) {
    m_camera->getComponent<Camera>()->m_view *= Matrix4::translationA(Vector3(0, -cam_speed, 0));
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
ShaderTest::createConstantBuffers()
{
  GraphicsAPI& api = g_GraphicAPI().instance();
  RendererManager& rm = g_RenderManager().instance();

  rm.m_cBView = api.createConstantBuffer(static_cast<uint32>(sizeof(CBView)),nullptr, 0);
  rm.m_cBProjection = api.createConstantBuffer(static_cast<uint32>(sizeof(CBProjection)),
                                               nullptr,
                                               0);
  rm.m_cBTransform = api.createConstantBuffer(static_cast<uint32>(sizeof(CBTransform)),
                                              nullptr,
                                              0);
  rm.m_cbLight = api.createConstantBuffer(static_cast<uint32>(sizeof(CBLight)), nullptr, 0);
  rm.m_cbCamera = api.createConstantBuffer(static_cast<uint32>(sizeof(CBCamera)), nullptr, 0);
}

void
ShaderTest::onUpdate()
{
  GraphicsAPI& api = g_GraphicAPI().instance();
  input();
  SPtr<Camera> camData = m_camera->getComponent<Camera>();
  Matrix4 view = camData->m_view.getTransposed();
  Matrix4 proj = camData->m_projection.getTransposed();
  SPtr<Light> lightData = light->getComponent<Light>();
  api.updateConstantBuffer(m_cBView, &view, static_cast<uint32>(sizeof(Matrix4)));
  api.updateConstantBuffer(m_cBProjection, &proj, static_cast<uint32>(sizeof(Matrix4)));
  api.updateConstantBuffer(m_cbLight, &lightData, static_cast<uint32>(sizeof(Light)));
  api.updateConstantBuffer(m_cbCamera, &camData, static_cast<uint32>(sizeof(Camera)));
}

void
ShaderTest::onRender()
{
  // screen clear color
  float clearColor[4] = { 0.0f, 0.123f, 0.3f, 1.0f };
  GraphicsAPI& api = g_GraphicAPI().instance();
  RendererManager& rm = g_RenderManager().instance();

  /**
   * Shadow Mapping.
   */
  SPtr<Camera> lightCam = light->getComponent<Camera>();
  Matrix4 projMatrix = lightCam->m_projection.getTransposed();
  api.updateConstantBuffer(m_cbCamera, &lightCam, static_cast<uint32>(sizeof(Camera)));
  api.updateConstantBuffer(m_cBProjection, &projMatrix, static_cast<uint32>(sizeof(Matrix4)));
  
  api.clearRenderTargetView(clearColor, rm.m_pRTargetView);
  api.clearDepthBuffer(1.0f, rm.m_pShadowDepthSV);
  
  api.setPSShader(rm.m_passes.find(1)->second->getPShader());
  api.setVSShader(rm.m_passes.find(1)->second->getVShader());
  api.setSampler(rm.m_passes.find(1)->second->getSamplerState());
  
  rm.VSSetConstantBuffers();
  rm.PSSetConstantBuffers();
  
  rm.renderActors(g_SceneManager().getActiveScene()->getAllActors());

  /**
   * Normal Render.
   */

  SPtr<Camera> cam = m_camera->getComponent<Camera>();
  Matrix4 camProj = cam->m_projection.getTransposed();
  api.updateConstantBuffer(m_cbCamera, &cam, static_cast<uint32>(sizeof(Camera)));
  api.updateConstantBuffer(m_cBProjection, &camProj, static_cast<uint32>(sizeof(Matrix4)));
  
  api.clearRenderTargetView(clearColor, rm.m_pRTargetView);
  api.clearDepthBuffer(1.0f, rm.m_pDepthSView);
  
  // set the base pass for the first rendering stage
  api.setPSShader(rm.m_passes.find(0)->second->getPShader());
  api.setVSShader(rm.m_passes.find(0)->second->getVShader());
  api.setSampler(rm.m_passes.find(0)->second->getSamplerState());
  
  // set constant buffers for the pixel and vertex shaders
  rm.VSSetConstantBuffers();
  rm.PSSetConstantBuffers();
  // render the objects
  rm.renderActors(g_SceneManager().getActiveScene()->getAllActors());

  /**
   * Ambient Occlussion. (Deferred)
   */

  api.setPSShader(rm.m_passes.find(2)->second->getPShader());
  api.setVSShader(rm.m_passes.find(2)->second->getVShader());
  api.setSampler(rm.m_passes.find(2)->second->getSamplerState());

  rm.PSSetConstantBuffers();
  rm.VSSetConstantBuffers();

  api.draw(3, 0);
}
