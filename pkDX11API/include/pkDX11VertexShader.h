/************************************************************************/
/**
* @pkDX11VertexShader pkDX11VertexShader.h
* @Hector Ivan Muñoz Ceballos
* @date 11/11/2024
* @DirectX Vertex Shader file for the Presk Game Engine.
*
* This file contains the Vertex Shader class using DirectX11 of the engine
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

class DX11Device;

class DX11VertexShader : public DX11Shaders
{
public:
  DX11VertexShader() = default;
  virtual ~DX11VertexShader()
  {
    safeRelease(pSBlob);
  }
  /**
  * Compile.
  * 
  * Compiles the vertex shader.
  * 
  * @return
  * If the compilation was successful or not.
  **/
  HRESULT
  compile();

 public:
  ID3D11VertexShader* pShader = nullptr;
};
}
