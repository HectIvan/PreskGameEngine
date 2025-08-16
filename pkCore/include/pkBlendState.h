/*****************************************************************************/
/**
 * @file    pkBlendState.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    04/03/2025
 * @brief   Blend State file for the Presk Game Engine.
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

namespace pkEngineSDK
{

class PK_CORE_EXPORT BlendState
{
 public:
  BlendState() = default;
  virtual ~BlendState() = default;
};
}