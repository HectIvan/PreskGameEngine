/*****************************************************************************/
/**
 * @file    pkShaderResource.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    07/11/2025
 * @brief   Resource for compiled shaders.
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
#include "pkBaseResource.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT ShaderResource : public BaseResource
{
 public:
  ShaderResource() = default;
  virtual ~ShaderResource() = default;

  /**
   * @brief Load the shader resource.
   */
  void
  load() override;

  RESOURCE_TYPE::E
  getType() const override { return RESOURCE_TYPE::kShader; }

 public:
  Vector<ANSICHAR> m_data;
};
}
