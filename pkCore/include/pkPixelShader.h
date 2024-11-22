/************************************************************************/
/**
* @pkPixelShader pkPixelShader.h
* @Hector Ivan Muñoz Ceballos
* @date 11/11/2024
* @Pixel Shader file for the Presk Game Engine.
*
* This file contains the Pixel Shader class of the engine
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
#include "pkPrerequisitesCore.h"
#include "pkShader.h"

namespace pkEngineSDK {

  using std::wstring;

class PixelShader : public Shader
{
public:
  PixelShader() = default;
  virtual ~PixelShader()
  {
    safeRelease(m_pPixelShader);
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
  create(Device* _pDevice);

  /**
  * Clean the pixel shader
  **/
  void
    clean();

 public:
  ID3D11PixelShader* m_pPixelShader = nullptr;
};
}
