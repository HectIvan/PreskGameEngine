/************************************************************************/
/**
* @pkShader pkShader.h
* @Hector Ivan Muñoz Ceballos
* @date 17/12/2024
* @Shader file for the Presk Game Engine.
*
* This file contains the base Shader class of the engine
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
#include "pkPrerequisitesCore.h"
#include "pkPath.h"

namespace pkEngineSDK
{

class BaseResource;

class PK_CORE_EXPORT Shader
{
 public:
  Shader() {};
  virtual ~Shader() = default;

  // deletes the possibility of using said constructor and operator in the class.
  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  // movement functions deleted.
  Shader(Shader&&) noexcept = delete;
  Shader& operator=(Shader&&) noexcept = delete;

  /**
   * @brief Compile the shader from a file.
   */
  virtual void
  compileFromFile() = 0;

  /**
   * @brief Compile the shader blob from a shader resource.
   */
  virtual void
  compileFromResource(const SPtr<BaseResource>& _pBaseResource) = 0;

  /**
   * @brief Set the shader directory.
   * @param _directory Directory of the shader.
   */
  virtual void
  setShaderDirectory(const Path _directory) = 0;

  /**
   * @brief Set the entry point of the shader.
   * @param _entry Entry point of the shader.
   */
  virtual void
  setEntryPoint(const char* _entry) = 0;

  /**
   * @brief Set the model of the shader.
   * @param _sModel Shader model.
   */
  virtual void
  setShaderModel(const char* _sModel) = 0;

  /**
   * @brief Set the shader data.
   * @param _directory Directory of the shader.
   * @param _entry Entry point of the shader.
   * @param _sModel Shader model.
   */
  virtual void
  setData(const Path _directory, const ANSICHAR* _entry, const ANSICHAR* _sModel) = 0;

  /**
   * @brief get the current shader directory.
   * @return the Shader directory.
   */
  virtual const Path&
  getShaderDirectory() = 0;

  /**
   * @brief Get the current shader entry point.
   * @return The entry point.
   */
  virtual const ANSICHAR*
  getEntryPoint() = 0;

  /**
   * @brief Get the current shader model.
   * @return The shader model.
   */
  virtual const ANSICHAR*
  getShaderModel() = 0;
};
}