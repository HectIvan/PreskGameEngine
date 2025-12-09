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
#include "pkPrerequisitesCore.h"
#include "pkShader.h"

namespace pkEngineSDK
{

struct ShaderKey
{
  ShaderKey(const String _path, const ANSICHAR* _entryP, const ANSICHAR* _shaderModel) :
    shaderPath(_path),
    _szEntryPoint(_entryP),
    _szShaderModel(_shaderModel)
  {};
  ~ShaderKey() = default;

  bool
  operator==(const ShaderKey& _other) const {
    return (shaderPath == _other.shaderPath &&
            _szEntryPoint == _other._szEntryPoint && 
            _szShaderModel == _other._szShaderModel);
  }

  String shaderPath;
  const ANSICHAR* _szEntryPoint;
  const ANSICHAR* _szShaderModel;
};
}

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

namespace PK_SHADER_TYPE
{
  enum E
  {
    kVertex,
    kPixel,
    kCompute
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
   * @brief Initialize the shader resource by type.
   * @param _type Type of shader to create.
   * @return Shader pointer.
   */
  SPtr<Shader>
  initShaderResource(const ShaderKey& _shaderData, const PK_SHADER_TYPE::E _type);

  /**
   * @brief Insert a shader into the map.
   * @param _key Shader specific key.
   * @param _pShader Shader to store.
   */
  void
  insertShader(const ShaderKey& _key, SPtr<Shader> _pShader);

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
  UMap<ShaderKey, SPtr<Shader>> m_shaders;
  UMap<String, SPtr<Shader>> m_shadersByPath;
};

PK_CORE_EXPORT ShaderManager&
g_ShaderManager();
}
