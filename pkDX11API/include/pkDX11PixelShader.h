/*****************************************************************************/
/**
 * @file    pkDX11PixelShader.h
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    11/11/2024
 * @brief   This file contains the Pixel Shader class using DirectX11 of the engine
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
#include "pkDX11Prerequisites.h"
#include "pkDX11Shader.h"

namespace pkEngineSDK
{

using std::wstring;
class DX11Device;

class DX11PixelShader : public DX11Shader
{
 public:
  DX11PixelShader() = default;
  virtual ~DX11PixelShader() {
    safeRelease(m_pShader);
  }

 public:
  ID3D11PixelShader* m_pShader = nullptr;
};
}
