/*****************************************************************************/
/**
 * @file    pkDX11Rasterizerstate.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    10/07/2025
 * @brief   DirectX Rasterizer state for the engine.
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
#include "pkRasterizerState.h"
#include "pkDX11Prerequisites.h"

namespace pkEngineSDK
{

class DX11RasterizerState : public RasterizerState
{
 public:
  DX11RasterizerState() = default;
  virtual ~DX11RasterizerState()
  {
    safeRelease(m_pRasterizer);
  }

public:
  ID3D11RasterizerState* m_pRasterizer = nullptr;
};
}
