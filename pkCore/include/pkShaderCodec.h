/*****************************************************************************/
/**
 * @file    pkShaderCodec.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    07/11/2025
 * @brief   Codec for compiled shaders.
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
#include "pkModule.h"
#include "pkPrerequisitesCore.h"

namespace pkEngineSDK
{

class BaseResource;
class Shader;

class PK_CORE_EXPORT ShaderCodec : public Module<ShaderCodec>
{
 public:
  ShaderCodec() = default;
  virtual ~ShaderCodec() = default;

  /**
   * @brief Create a shader resource from a pointer to a shader.
   * @param _pShader Pointer to the shader.
   */
  virtual SPtr<BaseResource>
  createResourceFromShader(const SPtr<Shader> _pShader) = 0;
};
}
