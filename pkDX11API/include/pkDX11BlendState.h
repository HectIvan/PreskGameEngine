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
#include "pkBlendState.h"
#include "pkDX11Prerequisites.h"

namespace pkEngineSDK
{

class DX11BlendState : public BlendState
{
 public:
  DX11BlendState() = default;
  virtual ~DX11BlendState() {
    safeRelease(m_pBlendState);
  }

 public:
  ID3D11BlendState* m_pBlendState;
};
}