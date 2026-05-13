/*****************************************************************************/
/**
 * @file    pkDX11PixelShader.h
 * @author  Héctor Iván Muñoz Ceballos
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

namespace pkEngineSDK {

class DX11PixelShader : public DX11Shader
{
 public:
  DX11PixelShader() {
    m_shaderType = PK_SHADER_TYPE::kPixel;
  }
  ~DX11PixelShader() override {
    safeRelease(m_pShader);
  }

  // deletes the possibility of the constructor and operator in the class.
  DX11PixelShader(const DX11PixelShader&) = delete;
  DX11PixelShader& operator=(const DX11PixelShader&) = delete;

  // movement functions deleted.
  DX11PixelShader(DX11PixelShader&&) noexcept = delete;
  DX11PixelShader& operator=(DX11PixelShader&&) noexcept = delete;

 public:
  ID3D11PixelShader* m_pShader = nullptr;
};
}
