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

class DX11PixelShader : public DX11Shaders
{
 public:
  DX11PixelShader() = default;
  virtual ~DX11PixelShader()
  {
    safeRelease(pShader);
  }

  /**
  * Compile pixel shader.
  * 
  * Compiles the pixel shader.
  * 
  * @return
  * If the compilation was successful or not.
  **/
  HRESULT
  compile(wstring _fileName, const char* _entryPoint, const char* _model);

 public:
  ID3D11PixelShader* pShader = nullptr;
};
}
