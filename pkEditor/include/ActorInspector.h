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

using pkEngineSDK::Actor;
using pkEngineSDK::Component;
using pkEngineSDK::SPtr;

class ActorInspector
{
 public:
  ActorInspector() = default;
  ActorInspector(SPtr<Actor> _pActor);
  virtual ~ActorInspector() = default;

  /**
   * @brief Inspect the components of the actor.
   * @param _pActor Actor to inspect.
   */
  void
  Inspect(SPtr<Actor>& _pActor);

  /**
   * @brief Create a window for the ui based on the component of the actor.
   * @param _pComponent Component to use.
   */
  void
  createComponentWindow(SPtr<Component>& _pComponent);
};
