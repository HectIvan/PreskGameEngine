/*****************************************************************************/
/**
 * @file    SceneInspector.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    17/01/2026
 * @brief   Inspector to show a scene graph in a window.
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

using pkEngineSDK::Actor;
using pkEngineSDK::SPtr;
using pkEngineSDK::Scene;
using pkEngineSDK::uint32;

class SceneInspector
{
 public:
  SceneInspector() = default;
  SceneInspector(const SPtr<Scene>& _pScene);
  virtual ~SceneInspector() = default;

  /**
   * @brief Set the scene to inspect.
   * @param _pScene Pointer to the scene.
   */
  void
  setScene(const SPtr<Scene>& _pScene);

  SPtr<Actor>
  getActor() { return m_pSelectedActor; }

  /**
   * @brief Create the scene graph inspector window.
   * @param _pSelectedActor Pointer to the selected actor.
   */
  void
  createSceneGraphWindow();

 private:
  SPtr<Scene> m_pScene;
  SPtr<Actor> m_pSelectedActor = nullptr;
  uint32 m_sActorIndex = 0;
};