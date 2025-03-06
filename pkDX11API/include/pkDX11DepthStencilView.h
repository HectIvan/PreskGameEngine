/************************************************************************/
/**
* @pkDX11DepthStencilView pkDX11DepthStencilView.h
* @Hector Ivan Muñoz Ceballos
* @date 21/10/2024
* @DirectX Depth stencil View file for the Presk Game Engine.
*
* This file contains the Depth stencil view class using DirectX11 of the engine
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
#include "pkDepthStencilView.h"

namespace pkEngineSDK
{

class DX11DepthStencilView : public DepthStencilView
{
 public:
  DX11DepthStencilView() = default;
  virtual ~DX11DepthStencilView()
  {
    safeRelease(pDepthSV);
  }

 public:
  ID3D11DepthStencilView* pDepthSV = nullptr;
};
}
