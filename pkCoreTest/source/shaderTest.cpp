#include "ShaderTest.h"
#include "pkDebug.h"
#include "pkGraphicsAPI.h"
#include "pkGraphicTypes.h"
#include "pkRendererManager.h"
#include "pkScene.h"
#include "pkGraphicsAPI.h"

using pkEngineSDK::Debug;
using pkEngineSDK::g_GraphicAPI;
using pkEngineSDK::g_RenderManager;
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

  m_scene.instantiate();
  SPtr<Actor> pistol = m_scene.getLastActor();
  pistol->addComponent(newModel("drakefire_pistol_low.obj"));

  //g_sceneManager().instantiate();
  //SPtr<Actor> room = g_sceneManager().getLastActor();
  //room->addComponent(newModel("sponza.obj"));
}

void
ShaderTest::onUpdate(float _deltaTime)
{
  // update the camera m_speed
  float camm_speed = m_cameraSpeed * _deltaTime;
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
    Vector2 posDif = (m_lastCursorPos - m_eventQueue.mousePosition) * _deltaTime;
    m_camera.rotate(-posDif.y, posDif.x, 0.0f);
    m_lastCursorPos = m_eventQueue.mousePosition;
  }
  else {
    m_lastCursorPos = m_eventQueue.mousePosition;
  }

  SPtr<Actor> actor = m_scene.m_actors[0];

  float rot = 1.0f * m_deltaTime;
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
ShaderTest::onRender(Scene& _scene)
{
  g_RenderManager().render(_scene);
}
