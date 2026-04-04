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
#include "pkBlob.h"
#include "pkPrerequisitesCore.h"
#include "pkPath.h"
#include "pkShaderKey.h"

namespace pkEngineSDK
{

namespace PK_SHADER_TYPE
{
enum E
{
  kVertex,
  kPixel,
  kCompute,
  kGeometry,
};
}

class BaseResource;

class PK_CORE_EXPORT Shader
{
 public:
  Shader() {};
  virtual ~Shader() = default;

  // deletes the possibility of the constructor and operator in the class.
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
   * @brief Set the shader data.
   * @param _key Shader key containing the data to set.
   */
  void
  setData(const ShaderKey& _key)
  {
    m_shaderDirectory = _key.shaderPath;
    m_sEntryPoint = _key._szEntryPoint;
    m_sModel = _key._szShaderModel;
  }

  /**
   * @brief Get the shader type.
   * @return The shader type.
   */
  const PK_SHADER_TYPE::E&
  getType() const { return m_shaderType; }

  /**
   * @brief Set the shader type.
   * @param _type Type of the shader.
   */
  void
  setType(const PK_SHADER_TYPE::E& _type) { m_shaderType = _type; }

  /**
   * @brief Set the shader directory.
   * @param _directory Directory of the shader.
   */
  void
  setShaderDirectory(const Path _directory) { m_shaderDirectory = _directory; }

  /**
   * @brief Set the entry point of the shader.
   * @param _entry Entry point of the shader.
   */
  void
  setEntryPoint(const ANSICHAR* _entry) { m_sEntryPoint = _entry; }

  /**
   * @brief Set the model of the shader.
   * @param _sModel Shader model.
   */
  void
  setShaderModel(const ANSICHAR* _sModel) { m_sModel = _sModel; }

  /**
   * @brief get the current shader directory.
   * @return the Shader directory.
   */
  const Path&
  getShaderDirectory() const { return m_shaderDirectory; }

  /**
   * @brief Get the current shader entry point.
   * @return The entry point.
   */
  const ANSICHAR*
  getEntryPoint() const { return m_sEntryPoint; }

  /**
   * @brief Get the current shader model.
   * @return The shader model.
   */
  const ANSICHAR*
  getShaderModel() const { return m_sModel; }

  /**
   * @brief Set the shader blob.
   * @param _pBlob Info blob.
   */
  void
  setBlob(PKBlob _pBlob) { m_pSBlob = _pBlob; }

  /**
   * @brief Get the shader blob.
   * @return the shader blob.
   */
  PKBlob
  getBlob() const { return m_pSBlob; }

 public:
  PKBlob m_pSBlob;

 protected:
  /**
   * Data used to compile the shader
   */
  Path m_shaderDirectory = Path();
  const ANSICHAR* m_sEntryPoint = nullptr;
  const ANSICHAR* m_sModel = nullptr;

  PK_SHADER_TYPE::E m_shaderType;
};
}
