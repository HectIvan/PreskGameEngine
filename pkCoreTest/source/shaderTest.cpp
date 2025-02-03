#include "ShaderTest.h"
#include "pkDebug.h"

using pkEngineSDK::Debug;

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
  m_scene.m_actors[0]->addComponent(newModel("drakefire_pistol_low.obj"));
  m_scene.m_actors[0]->addComponent(newMaterial("base_albedo.png"));
}

void
ShaderTest::onUpdate(float _deltaTime)
{
  // last cursor position
  // Vector2 lastCursorPos = m_eventQueue.mousePosition;
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
    Debug::print(posDif);
    m_lastCursorPos = m_eventQueue.mousePosition;
  }
  else {
    m_lastCursorPos = m_eventQueue.mousePosition;
  }
}
