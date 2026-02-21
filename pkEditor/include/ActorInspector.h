/*****************************************************************************/
/**
 * @file    ActorInspector.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    01/07/2025
 * @brief   Inspector for the Game Actor.
 *
 * @bug    .- user is able to drag & drop from the resources window, but is not able to drag
              materials inside of the material window.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkActor.h"
#include "pkWindow.h"
#include "pkMaterial.h"

using pkEngineSDK::Actor;
using pkEngineSDK::Component;
using pkEngineSDK::SPtr;
using pkEngineSDK::Matrix4;
using pkEngineSDK::String;
using pkEngineSDK::Window;
using pkEngineSDK::Material;

class ActorInspector
{
 public:
  ActorInspector() = default;
  ActorInspector(const SPtr<Actor>& _pActor);
  virtual ~ActorInspector() = default;
  /**
   * @brief Set the actor to inspect.
   * @param _pActor Pointer to the actor.
   */
  void
  setActor(SPtr<Actor>& _pActor) { m_actor = _pActor; }

  /**
   * @brief Inspect the currently selected actor.
   * @param _pMaterialInspect Material to set on the inspector.
   */
  void
  inspectComponents(SPtr<Material>& _pMaterialInspect);

 private:
  SPtr<Actor> m_actor;
  String m_searchMesh;
};
