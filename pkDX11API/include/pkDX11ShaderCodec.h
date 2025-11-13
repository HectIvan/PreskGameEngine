/*****************************************************************************/
/**
 * @file    pkDX11ShaderCodec.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    07/11/2025
 * @brief   Codec for DirectX compiled shaders.
 *
 * @bug    No known bugs.
 *
 * @changes Created the file.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
 * Includes
 */
 /*********************************************/
#include "pkShaderCodec.h"

namespace pkEngineSDK
{

class DX11ShaderCodec : public ShaderCodec
{
 public:
  DX11ShaderCodec() = default;
  virtual ~DX11ShaderCodec() = default;

  /**
   * @brief Create a shader resource from a pointer to a DirectX shader.
   * @param _pShader Pointer to the shader.
   */
  SPtr<BaseResource>
  createResourceFromShader(const SPtr<Shader>& _pShader) override;
};
}
