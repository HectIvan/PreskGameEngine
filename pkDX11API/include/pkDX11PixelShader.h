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

namespace pkEngineSDK {

using std::wstring;
class DX11Device;

class DX11PixelShader : public DX11Shaders
{
 public:
  DX11PixelShader() = default;
  virtual ~DX11PixelShader()
  {
    clean();
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

  /**
  * Create Pixel shader.
  * 
  * Creates the pixel shader in the device provided.
  * 
  * param _pDevice
  * Device where the shader will be created.
  * 
  * @return
  * If the creation was successful or not.
  **/
  HRESULT
  create(DX11Device* _pDevice);
  
  /**
  * Clean the pixel shader
  **/
  void
  clean();

 public:
  ID3D11PixelShader* m_pShader = nullptr;
};
}
