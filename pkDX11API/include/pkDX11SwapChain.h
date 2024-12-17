/************************************************************************/
/**
* @pkDX11SwapChain pkDX11SwapChain.h
* @Hector Ivan Muñoz Ceballos
* @date 17/12/2024
* @Swap Chain file using DirectX for the Presk Game Engine.
*
* This file contains the Swap Chain class using DirectX of the engine
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
#include "pkSwapChain.h"

namespace pkEngineSDK
{

class DX11SwapChain : public SwapChain
{
 public:
  DX11SwapChain() = default;
  virtual ~DX11SwapChain()
  {
    safeRelease(pSch);
  }

 public:
  IDXGISwapChain* pSch;
};
}