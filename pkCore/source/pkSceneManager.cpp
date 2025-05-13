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
#include "pkSceneManager.h"
#include "pkLogger.h"

namespace pkEngineSDK
{
void
SceneManager::createScene()
{
  SPtr<Scene> scene = make_shared<Scene>();
  scene->setActive(false);
  m_scenes.push_back(scene);
}

void
SceneManager::setActive(uint32 _index)
{
  // check if index is in array size
  if (_index >= m_scenes.size())
  {
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
PK_CORE_EXPORT SceneManager&
g_SceneManager()
{
  return SceneManager::instance();
}
}