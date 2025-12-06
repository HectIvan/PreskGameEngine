/*****************************************************************************/
/**
 * @file    pkActorCodec.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    05/12/2025
 * @brief   Actor codec for the game engine.
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
#include "pkPrerequisitesCore.h"
#include "pkModule.h"

namespace pkEngineSDK
{

class Actor;
class BaseResource;

class PK_CORE_EXPORT ActorCodec : public Module<ActorCodec>
{
 public:
  ActorCodec() = default;
  virtual ~ActorCodec() = default;

  /**
   * @brief Encode an actor to a file.
   */
  SPtr<BaseResource>
  createResource(const SPtr<Actor>& _pActor);
};

PK_CORE_EXPORT ActorCodec&
g_ActorCodec();
}