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

  // deletes the possibility of the constructor and operator in the class.
  DX11Shader(const DX11Shader&) = delete;
  DX11Shader& operator=(const DX11Shader&) = delete;

  // movement functions deleted.
  DX11Shader(DX11Shader&&) noexcept = delete;
  DX11Shader& operator=(DX11Shader&&) noexcept = delete;

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
