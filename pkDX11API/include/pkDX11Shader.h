/************************************************************************/
/**
* @pkDX11Shader pkDX11Shader.h
* @Hector Ivan Muñoz Ceballos
* @date 11/11/2024
* @DirectX Shader file for the Presk Game Engine.
*
* This file contains the Shader class using DirectX11 of the engine
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
#include "pkDX11Prerequisites.h"
#include "pkShader.h"

namespace pkEngineSDK
{

using std::wstring;

class DX11Shader : public Shader
{
 public:
  DX11Shader() = default;
  virtual ~DX11Shader() {
    safeRelease(m_pSBlob);
  }

  /**
   * @brief Compile the shader.
   */
  void
  compile() override;

  /**
   * @brief Set the shader directory.
   * @param _directory Directory of the shader.
   */
  void
  setShaderDirectory(const Path _directory) override { m_shaderDirectory = _directory; }

  /**
   * @brief Set the entry point of the shader.
   * @param _entry Entry point of the shader.
   */
  void
  setEntryPoint(const char* _entry) override { m_sEntryPoint = _entry; }

  /**
   * @brief Set the model of the shader.
   * @param _sModel Shader model.
   */
  void
  setShaderModel(const char* _sModel) override { m_sModel = _sModel; }

  /**
   * @brief Set the shader data.
   * @param _directory Directory of the shader.
   * @param _entry Entry point of the shader.
   * @param _sModel Shader model.
   */
  void
  setData(const Path _directory, const char* _entry, const char* _sModel) override;

  /**
   * @brief get the current shader directory.
   * @return the Shader directory.
   */
  const Path&
  getShaderDirectory() override { return m_shaderDirectory; }

  /**
   * @brief Get the current shader entry point.
   * @return The entry point.
   */
  const char*
  getEntryPoint() override { return m_sEntryPoint; }

  /**
   * @brief Get the current shader model.
   * @return The shader model.
   */
  const char*
  getShaderModel() override { return m_sModel; }

  void*
  getBlob() { return m_pSBlob; }

 public:
  ID3DBlob* m_pSBlob = nullptr;

 protected:
  /**
   * Data used to compile the shader
   */
  Path m_shaderDirectory;

  const char* m_sEntryPoint;

  const char* m_sModel;
};
}
