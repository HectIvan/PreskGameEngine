/************************************************************************/
/**
* @pkDX11RenderTargetView pkDX11RenderTargetView.h
* @Hector Ivan Muñoz Ceballos
* @date 17/12/2024
* @Render Target file using DirectX for the Presk Game Engine.
*
* This file contains the Render Target class using DirectX of the engine
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
#include "pkRenderTargetView.h"

namespace pkEngineSDK
{

class DX11RenderTargetView : public RenderTargetView
{
 public:
  DX11RenderTargetView() = default;
  virtual ~DX11RenderTargetView()
  {
    safeRelease(pRtv);
  }

 public:
  ID3D11RenderTargetView* pRtv;
};
}