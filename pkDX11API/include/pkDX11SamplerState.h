/*****************************************************************************/
/**
 * @file    pkDX11SamplerState.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    11/11/2024
 * @brief   DirectX Sampler State file for the Presk Game Engine.
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
#include "pkSamplerState.h"
#include "pkDX11Prerequisites.h"

namespace pkEngineSDK
{

class DX11SamplerState : public SamplerState
{
 public:
  DX11SamplerState() = default;
  virtual ~DX11SamplerState() {
    safeRelease(m_pSampler);
  }

 public:
  ID3D11SamplerState* m_pSampler = nullptr;
};
}