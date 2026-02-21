/*****************************************************************************/
/**
 * @file    TransformInspector.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    21/02/2025
 * @brief   Inspector for a transform.
 */
 /*****************************************************************************/
#pragma once

#include "pkActor.h"
#include "pkPrerequisitesUtilities.h"

using pkEngineSDK::SPtr;
using pkEngineSDK::Actor;

class TransformInspector
{
 public:
  TransformInspector() = default;
  virtual ~TransformInspector() = default;

  /**
   * @brief Transform Inspector.
   * @param _actor Actor to inspect their transform.
   */
  static void
  inspect(SPtr<Actor>& _actor);
};