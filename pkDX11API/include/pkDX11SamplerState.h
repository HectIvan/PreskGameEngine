/************************************************************************/
/**
* @pkDX11SamplerState pkDX11SamplerState.h
* @Hector Ivan Muñoz Ceballos
* @date 11/11/2024
* @DirectX Sampler State file for the Presk Game Engine.
*
* This file contains the Sampler State class using DirectX11 of the engine
*
* @bug No bug known.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11Prerequisites.h"

namespace pkEngineSDK {

class DX11SamplerState
{
public:
  DX11SamplerState() = default;
  virtual ~DX11SamplerState()
  {
    safeRelease(m_pSampler);
  }

  /**
  * Clean the sampler state.
  **/
  void
  clean();

 public:
  ID3D11SamplerState* m_pSampler = nullptr;
};
}