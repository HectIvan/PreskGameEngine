/*****************************************************************************/
/**
 * @file    pkSceneManager.h
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    2025/09/05
 * @brief   Scene manager for every scene in the engine.
 *
 * @bug    No known bugs.
 */
/*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkScene.h"
#include "pkModule.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT SceneManager : public Module<SceneManager>
{
 public:
  SceneManager() = default;
  virtual ~SceneManager() = default;

  /**
   * @brief Clear all and initialize a single scene.
   */
  void
  init();

  /**
   * @brief Create a new scene.
   * @param _name Name of the scene.
   */
  void
  createScene(const String& _name = "");

  /**
   * @brief Deletes a desired scene.
   * @param _index Index of the scene to delete.
   */
  void
  deleteScene(const uint32& _index);

  /**
   * @brief Set the scene as active.
   * @param _index Scene to set as active.
   */
  void
  setActive(const uint32& _index) const;

  /**
   * @brief Get the currently active scene.
   * @return Pointer to the currently active scene.
   */
  SPtr<Scene>
  getActiveScene() const;

  /**
   * @brief Clear the scene manager.
   */
  void
  clear();

 private:
  Vector<SPtr<Scene>> m_scenes;
};

PK_CORE_EXPORT SceneManager&
g_SceneManager();
}
