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
  virtual ~SceneInspector() = default;

  /**
   * @brief Get the selected actor.
   * @return Pointer to the selected actor.
   */
  PKFORCEINLINE SPtr<Actor>
  getActor() const { return m_pSelectedActor; }

  /**
   * @brief Create the scene graph inspector window.
   * @param _pSelectedActor Pointer to the selected actor.
   */
  void
  createSceneGraphWindow(const SPtr<Scene>& _pScene);

 private:
  SPtr<Actor> m_pSelectedActor = nullptr;
  uint32 m_sActorIndex = 0;
};