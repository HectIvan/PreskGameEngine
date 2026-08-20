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
  createScene("Base scene");
  setActive(0);
  LOG_REGISTER("Initialized Scene Manager.", __FILE__, __LINE__);
}

void
SceneManager::createScene(const String& _name)
{
  SPtr<Scene> scene = pk_shared_ptr_new<Scene>();
  scene->setName(_name);
  scene->setActive(false);
  m_scenes.push_back(scene);
  LOG_REGISTER("Created scene '" + _name + "'.", __FILE__, __LINE__);
}

void
SceneManager::deleteScene(const uint32& _index)
{
  // check if the index is inside the range of existing scenes.
  if (_index >= m_scenes.size() - 1 && !(_index < 0)) {
    SPtr<Scene> scene = m_scenes[_index];
    scene->clear();
    m_scenes.erase(m_scenes.begin() + _index);
    LOG_REGISTER("Deleted Scene " + scene->getName() + ".", __FILE__, __LINE__);
  }
  else {
    const String msg = "Trying to delete an out of range scene.";
    LOG_WARNING(msg, __FILE__, __LINE__);
  }
}

void
SceneManager::setActive(const uint32& _index) const
{
  // check if the index is inside the range of existing scenes.
  const uint32 sceneCount = toUint32(m_scenes.size());
  if (_index > sceneCount - 1 && !(_index < 0)) {
    String errMsg = "setActive call out of bounds. index accesed: " + to_string(_index);
    errMsg += ". Current scene count: " + to_string(toUint32(m_scenes.size()));
    LOG_WARNING(errMsg, __FILE__, __LINE__);
    return;
  }

  // set all scenes as false and set the desired scene as active
  for (uint32 i = 0; i < sceneCount; ++i) {
    m_scenes[i]->setActive(false);
  }
  m_scenes[_index]->setActive(true);
}

SPtr<Scene>
SceneManager::getActiveScene() const
{
  const uint32 sceneCount = toUint32(m_scenes.size());
  for (uint32 i = 0; i < sceneCount; ++i) {
    if (m_scenes[i]->isActive()) {
      return m_scenes[i];
    }
  }
  return nullptr;
}

void
SceneManager::clear()
{
  const uint32 sceneCount = toUint32(m_scenes.size());
  for (uint32 i = 0; i < sceneCount; ++i) {
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