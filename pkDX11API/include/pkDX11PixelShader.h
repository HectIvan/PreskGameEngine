/************************************************************************/
/**
* @pkDX11PixelShader pkDX11PixelShader.h
* @Hector Ivan Muñoz Ceballos
* @date 11/11/2024
* @DirectX Pixel Shader file for the Presk Game Engine.
*
* This file contains the Pixel Shader class using DirectX11 of the engine
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
  compile();

 public:
  ID3D11PixelShader* pShader = nullptr;
};
}
