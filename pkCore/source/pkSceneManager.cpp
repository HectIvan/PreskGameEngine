/*****************************************************************************/
/**
 * @file    pkSceneManager.cpp
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    2025/09/05
 * @brief   Scene manager for every scene in the engine.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#include "pkLogger.h"
#include "pkPlatformMath.h"
#include "pkSceneManager.h"

namespace pkEngineSDK
{
void
SceneManager::init()
{
  clear();
  createScene();
  setActive(0);
  g_Logger().registerMessage("Initialized Scene Manager.");
}

void
SceneManager::createScene(String _name)
{
  SPtr<Scene> scene = make_shared<Scene>();
  scene->m_name = _name;
  scene->setActive(false);
  m_scenes.push_back(scene);
  g_Logger().registerMessage("Created scene " + _name + ".");
}

void
SceneManager::deleteScene(uint32 _index)
{
  Logger& log = g_Logger().instance();
  // check if the index is inside the range of existing scenes.
  if (_index >= m_scenes.size() - 1 && !(_index < 0)) {
    SPtr<Scene> scene = m_scenes[_index];
    log.registerMessage("Deleted Scene " + scene->m_name + ".");
    scene->clear();
    m_scenes.erase(m_scenes.begin() + _index);
  }
  else {
    log.registerMessage("Trying to delete an out of range scene.", LOG_MSG_TYPE::kWarning);
  }
}

void
SceneManager::setActive(uint32 _index)
{
  // check if the index is inside the range of existing scenes.
  if (_index > m_scenes.size() - 1 && !(_index < 0)) {
    String errMsg = "setActive call out of bounds. index accesed: " + to_string(_index);
    errMsg += ". Current scene count: " + to_string(static_cast<uint32>(m_scenes.size()));
    g_Logger().registerMessage(errMsg, LOG_MSG_TYPE::kWarning);
    return;
  }

  // set all scenes as false and set the desired scene as active
  for (uint32 i = 0; i < m_scenes.size(); ++i) {
    m_scenes[i]->setActive(false);
  }
  m_scenes[_index]->setActive(true);
}

SPtr<Scene>
SceneManager::getActiveScene()
{
  for (uint32 i = 0; i < m_scenes.size(); ++i) {
    if (m_scenes[i]->m_isActive) {
      return m_scenes[i];
    }
  }
  return nullptr;
}

void
SceneManager::clear()
{
  for (uint32 i = 0; i < m_scenes.size(); ++i) {
    m_scenes[i]->clear();
  }
  m_scenes.clear();
}

PK_CORE_EXPORT SceneManager&
g_SceneManager()
{
  return SceneManager::instance();
}
}