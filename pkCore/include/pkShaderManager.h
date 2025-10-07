/*****************************************************************************/
/**
 * @file    pkShaderManager.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    10/06/2026
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

// to do: temporary shader key, change for a UUID
class ShaderKey
{
 public:
  ShaderKey() = default;
  ShaderKey(const ShaderKey&) = default;
  ShaderKey(const Path _path, const char* _entryP, const char* _shaderModel) :
    shaderPath(_path),
    _szEntryPoint(_entryP),
    _szShaderModel(_shaderModel)
  {};
  ~ShaderKey() = default;
  

  Path shaderPath;
  const char* _szEntryPoint;
  const char* _szShaderModel;
};

class ShaderManager : public Module<ShaderManager>
{
 public:
  ShaderManager() = default;
  ~ShaderManager() = default;

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

  UMap<ShaderKey, SPtr<Shader>> m_shaders;
};

PK_CORE_EXPORT ShaderManager&
g_ShaderManager();
}
