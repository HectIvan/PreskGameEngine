/*****************************************************************************/
/**
 * @file    pkShaderManager.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    10/06/2025
 * @brief   Shader Manager for the Engine.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
 * Includes
 */
/*********************************************/
#include "pkModule.h"
#include "pkPath.h"
#include "pkShader.h"
#include "pkUUID.h"
#include "pkShaderKey.h"

namespace std {
template<>
struct hash<pkEngineSDK::ShaderKey>
{
  SIZE_T
    operator()(const pkEngineSDK::ShaderKey& _key) const {
    return hash<pkEngineSDK::String>()(_key.shaderPath) ^
           hash<const pkEngineSDK::ANSICHAR*>()(_key._szEntryPoint) ^
           hash<const pkEngineSDK::ANSICHAR*>()(_key._szShaderModel);
  }
};
}

namespace pkEngineSDK
{
class PK_CORE_EXPORT ShaderManager : public Module<ShaderManager>
{
 public:
  ShaderManager() = default;
  ~ShaderManager() = default;

  /**
   * @brief Create the shader resources.
   */
  void
  createShaderResources();

  /**
   * @brief create the shader resource by type.
   * @param _type Type of shader to create.
   */
  void
  createShaderResource(const ShaderKey& _shaderData, const PK_SHADER_TYPE::E _type);

  /**
   * @brief Insert a shader into the map.
   * @param _key Shader specific key.
   * @param _pShader Shader to store.
   */
  void
  insertShader(const UUID& _id, const SPtr<Shader>& _pShader);

  /**
   * @brief Create the shaders from the shader resources.
   */
  void
  createShaders();

  /**
   * @brief Insert a shader into the map.
   * @param _key Shader specific key.
   * @param _pShader Shader to store.
   */
  void
  insertShader(const ShaderKey& _key, const SPtr<Shader>& _pShader);

  /**
   * @brief Get shader from the shader map.
   * @param _id ID of the shader to look for.
   * @return Shader pointer.
   */
  SPtr<Shader>
  getShader(const UUID& _id);

  /**
   * @brief Get shader from the shader map.
   * @param _key Shader to look for.
   * @return Shader pointer.
   */
  SPtr<Shader>
  getShader(const ShaderKey& _key);

  /**
   * @brief Get all shaders from the manager.
   * @return All shaders.
   */
  Vector<SPtr<Shader>>
  getShaders();

  /**
   * @brief Get the names from all compiled shaders.
   * @param _getEntry If the function should get the entry point of the shader.
   * @param _getModel If the function should get the model of the shader.
   * @return The name of the shader with or without the entry point and/or model.
   */
  Vector<String> 
  getShaderNames(const bool _getEntry = false, const bool _getModel = false);

 private:
  UMap<UUID, SPtr<Shader>> m_shaders;
  UMap<ShaderKey, SPtr<Shader>> m_keyShaders;
};

PK_CORE_EXPORT ShaderManager&
g_ShaderManager();
}
