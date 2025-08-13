/*****************************************************************************/
/**
 * @file    ActorInspector.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    01/07/2025
 * @brief   Inspector for the Game Actor.
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
#include "pkActor.h"
#include "pkWindow.h"

using pkEngineSDK::Actor;
using pkEngineSDK::Component;
using pkEngineSDK::SPtr;
using pkEngineSDK::Matrix4;
using pkEngineSDK::String;
using pkEngineSDK::Window;

class ActorInspector
{
 public:
  ActorInspector() = default;
  ActorInspector(SPtr<Actor> _pActor);
  virtual ~ActorInspector() = default;

  /**
   * @brief Inspect the components of the actor.
   */
  void
  Inspect();

  /**
   * @brief Create a window for the ui based on the component of the actor.
   * @param _pComponent Component to use.
   * @param _window Main window.
   * @param _searchMesh Search string for a mesh in the model.
   */
  void
  createComponentWindow(SPtr<Component>& _pComponent,
                        Window& _window,
                        String& _searchMesh,
                        float& _imgTexSize);

 public:
  SPtr<Actor> m_actor;
};
