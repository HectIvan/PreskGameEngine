#include "ShaderTest.h"
#include "pkLogger.h"
#include "pkGraphicsAPI.h"
#include "pkGraphicTypes.h"
#include "pkRendererManager.h"
#include "pkScene.h"
#include "pkTimeManager.h"
#include "pkGraphicsAPI.h"

using pkEngineSDK::Logger;
using pkEngineSDK::g_GraphicAPI;
using pkEngineSDK::g_RenderManager;
using pkEngineSDK::g_TimeManager;
using pkEngineSDK::g_sceneManager;
using pkEngineSDK::Light;
using pkEngineSDK::Material;
using pkEngineSDK::Matrix4;
using pkEngineSDK::Scene;
using pkEngineSDK::SPtr;
using pkEngineSDK::TEXTURE_FORMATS::kPK_FORMAT_R32G32B32_FLOAT;
using pkEngineSDK::uint32;

void
ShaderTest::onInit()
{
  m_camera.init(m_window.getWidth(),
                m_window.getHeight(),
                3.1416f / 4.0f,
                0.01f,
                1000.0f,
                Vector3(0.0f, 0.0f, -30.0f), // position
                Vector3(0.0f, 0.0f, 0.0f), // target
                Vector3(0.0f, 1.0f, 0.0f)); // up vector

  g_sceneManager().instantiate();
  SPtr<Actor> pistol = g_sceneManager().getLastActor();
  pistol->addComponent(newModel("drakefire_pistol_low.obj"));
  pistol->addComponent(createMaterial());
  SPtr<Material> pMaterial = pistol->getComponent<Material>();
  pMaterial->setDiffuse(createTexture("drakePistol/base_albedo.png"));
  pMaterial->setOcclusion(createTexture("drakePistol/base_AO.png"));
  pMaterial->setMetallic(createTexture("drakePistol/base_metallic.png"));
  pMaterial->setNormal(createTexture("drakePistol/base_normal.png"));
}

void
ShaderTest::onUpdate()
{
  float deltaTime = g_TimeManager().m_deltaTime;
  // update the camera m_speed
  float camm_speed = m_cameraSpeed * deltaTime;
  // move forward/backward
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kW)) {
    m_camera.move(Vector3(0.0f, 0.0f, camm_speed));
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kS)) {
    m_camera.move(Vector3(0.0f, 0.0f, -camm_speed));
  }
  // move left/right
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kA)) {
    m_camera.move(Vector3(camm_speed, 0.0f, 0.0f));
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kD)) {
    m_camera.move(Vector3(-camm_speed, 0.0f, 0.0f));
  }
  // move up/down
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kE) ||
    m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kSpace)) {
    m_camera.move(Vector3(0.0f, camm_speed, 0.0f));
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kQ) ||
    m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kLControl)) {
    m_camera.move(Vector3(0.0f, -camm_speed, 0.0f));
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kLButton)) {
    Vector2 posDif = (m_lastCursorPos - m_eventQueue.mousePosition) * deltaTime;
    m_camera.rotate(-posDif.y, posDif.x, 0.0f);
    m_lastCursorPos = m_eventQueue.mousePosition;
  }
  else {
    m_lastCursorPos = m_eventQueue.mousePosition;
  }

  SPtr<Actor> actor = g_sceneManager().getActor(0);

  float rot = 1.0f * deltaTime;
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kLeft)) {
    rot *= -1.0f;
    actor->m_transform *= Matrix4::rotationY(rot);
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kRight)) {
    actor->m_transform *= Matrix4::rotationY(rot);
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kUp)) {
    rot *= -1.0f;
    actor->m_transform *= Matrix4::rotationX(rot);
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kDown)) {
    actor->m_transform *= Matrix4::rotationX(rot);
  }
}

void
ShaderTest::onRender()
{
  g_RenderManager().render();
  // update the light buffer
  // g_GraphicAPI().updateConstantBuffer(g_RenderManager().m_cbLight,
  //                                     &g_RenderManager().light,
  //                                     static_cast<uint32>(sizeof(Light)));
  // // Set shaders
  // g_GraphicAPI().setPSShader(g_RenderManager().getPSShader());
  // g_GraphicAPI().setVSShader(g_RenderManager().getVSShader());
  // // set light
  // g_RenderManager().light.Type = pkEngineSDK::LIGHT_TYPE::kDirectional;
  // g_RenderManager().light.LightDir = Vector3::FORWARD;
  // // set constant buffers for the pixel and vertex shaders
  // g_RenderManager().VSSetConstantBuffers();
  // g_RenderManager().PSSetConstantBuffers();
  // // render the objects
  // g_RenderManager().renderActors(_scene.m_actors);

  // turn the render target view into a texture.  
}
