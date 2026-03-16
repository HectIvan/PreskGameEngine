/*****************************************************************************/
/**
 * @file    ActiveActorInspector.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    11/03/2026
 * @brief   Inspector for the currently selected actor.
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"
#include "pkActor.h"
#include "pkWindow.h"

using pkEngineSDK::Actor;
using pkEngineSDK::SPtr;
using pkEngineSDK::Window;

class ActiveActorInspector
{
public:
  ActiveActorInspector() = default;
  virtual ~ActiveActorInspector() = default;

  static void
  init(Window& _window, SPtr<Actor>& _pActor);
};
