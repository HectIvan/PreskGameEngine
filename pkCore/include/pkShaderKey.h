/*****************************************************************************/
/**
 * @file    pkshaderKey.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    04/03/2025
 * @brief   Shader Key for locating specific shaders.
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
#include "pkPrerequisitesCore.h"
#include "pkPath.h"

namespace pkEngineSDK
{

class ShaderKey
{
 public:
  ShaderKey() = default;
  ShaderKey(const String& _path, const ANSICHAR* _entryP, const ANSICHAR* _shaderModel) :
    shaderPath(_path),
    _szEntryPoint(_entryP),
    _szShaderModel(_shaderModel)
  {};
  ShaderKey(const Path& _path, const ANSICHAR* _entryP, const ANSICHAR* _shaderModel) :
    shaderPath(_path.toString()),
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

  /**
   * @brief Check if the shader key is empty, meaning that it doesn't have a valid path,
   *  entry point or shader model.
   */
  bool
  isEmpty() const {
    return shaderPath.empty() || !_szEntryPoint || !_szShaderModel;
  }

 public:
  String shaderPath = "";
  const ANSICHAR* _szEntryPoint = nullptr;
  const ANSICHAR* _szShaderModel = nullptr;
};
}