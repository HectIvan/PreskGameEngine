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
   * @param _imgTexSize Image texture size.
   * @param _pMaterialInspect Material to set on the inspector.
   */
  void
  createComponentWindow(SPtr<Component>& _pComponent,
                        Window& _window,
                        String& _searchMesh,
                        float& _imgTexSize,
                        SPtr<Material>& _pMaterialInspect);

 public:
  SPtr<Actor> m_actor;
};
