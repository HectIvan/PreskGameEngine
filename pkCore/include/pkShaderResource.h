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

  /**
   * @brief Generate a unique ID for the shader resource based on a ShaderKey.
   * @param _key The ShaderKey containing the shader properties.
   * @return A unique UUID for the shader resource.
   */
  static UUID
  generateID(const ShaderKey& _key) {
    return generateID(_key.shaderPath.c_str(), _key._szEntryPoint, _key._szShaderModel);
  }

  /**
   * @brief Generate a unique ID for the shader resource based on its properties.
   * @param _shaderDirectory The directory of the shader file.
   * @param _sEntryPoint The entry point of the shader.
   * @param _sModel The shader model.
   * @return A unique UUID for the shader resource.
   */
  static UUID
  generateID(const ANSICHAR* _shaderDirectory,
             const ANSICHAR* _sEntryPoint,
             const ANSICHAR* _sModel);

 public:
  PK_SHADER_TYPE::E m_type;

  ANSICHAR m_shaderDirectory[PK_RESOURCE_PATH_SIZE];
  ANSICHAR m_sEntryPoint[PK_RESOURCE_NAME_SIZE];
  ANSICHAR m_sModel[PK_RESOURCE_NAME_SIZE];

  Vector<ANSICHAR> m_data;
};
}
