/*****************************************************************************/
/**
 * @file    pkRendererManager.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    04/02/2025
 * @brief   This file will contain the Render Manager used for the engine
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

class PK_CORE_EXPORT DepthStencilView
{
 public:
  DepthStencilView() = default;
  virtual ~DepthStencilView() = default;
};
}