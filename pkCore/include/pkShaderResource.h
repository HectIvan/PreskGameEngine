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
#include "pkShader.h"

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

  /**
   * @brief Unload the shader resource.
   */
  void
  unload() override;

  /**
   * @brief Get the resource type.
   */
  RESOURCE_TYPE::E
  getType() const override { return RESOURCE_TYPE::kShader; }

  /**
   * @brief Get the resource type as a string.
   * @return The resource type string.
   */
  String
  getTypeString() const override { return "Shader"; }

 public:
  PK_SHADER_TYPE::E m_type;

  ANSICHAR m_shaderDirectory[PK_RESOURCE_PATH_SIZE];
  ANSICHAR m_sEntryPoint[PK_RESOURCE_NAME_SIZE];
  ANSICHAR m_sModel[PK_RESOURCE_NAME_SIZE];

  Vector<ANSICHAR> m_data;
};
}
