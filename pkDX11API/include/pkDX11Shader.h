/************************************************************************/
/**
* @pkDX11Shader pkDX11Shader.h
* @Hector Ivan Muñoz Ceballos
* @date 11/11/2024
* @DirectX Shader file for the Presk Game Engine.
*
* This file contains the Shader class using DirectX11 of the engine
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
#include "pkShader.h"

namespace pkEngineSDK
{

using std::wstring;

class DX11Shader : public Shader
{
 public:
  DX11Shader() = default;
  virtual ~DX11Shader() = default;

  /**
   * @brief Compile the shader from a file.
   */
  void
  compileFromFile() override;

  /**
   * @brief Compile the shader blob from a shader resource.
   */
  void
  compileFromResource(const SPtr<BaseResource>& _pBaseResource) override;
};
}
