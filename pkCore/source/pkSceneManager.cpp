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
}

void
SceneManager::createScene(String _name)
{
  SPtr<Scene> scene = make_shared<Scene>();
  scene->m_name = _name;
  scene->setActive(false);
  m_scenes.push_back(scene);
}

void
SceneManager::deleteScene(uint32 _index)
{
  if (Math::isInRange(static_cast<float>(_index),
                      static_cast<float>(0),
                      static_cast<float>(m_scenes.size()))) {
    m_scenes[_index]->clear();
    m_scenes.erase(m_scenes.begin() + _index);
  }
  else {
    g_Logger().print("WARNING:: Trying to delete an out of range scene.");
  }
}

void
SceneManager::setActive(uint32 _index)
{
  // check if index is in array size
  if (_index >= m_scenes.size()) {
    String errMsg = "ERROR: setActive call out of bounds. index accesed: " + _index;
    errMsg += ". Current scene count: " + static_cast<uint32>(m_scenes.size());
    g_Logger().print(errMsg);
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