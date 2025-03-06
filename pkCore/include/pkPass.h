/*****************************************************************************/
/**
 * @file    pkPass.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    07/02/2025
 * @brief   Header file of the pass, a pass is used for rendering a scene and calculating
 *          post-process effects
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkInputLayout.h"
#include "pkPrerequisitesCore.h"
#include "pkSamplerState.h"
#include "pkShader.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT Pass
{
public:
  Pass() = default;
  virtual ~Pass() = default;

  /**
   * @brief Create the pass, creating all shaders, layouts and samplers.
   */
  void
  create();

  /**
   * @brief Create the vertex shader input layout.
   */
  void
  createInputLayout();

  /**
   * @brief Set the input layout to the device.
   */
  void
  setInputLayout();

  /**
   * @brief Create the sampler state.
   */
  void
  createSamplerState(uint32 _mode, uint32 _filter);

  /**
   * @brief Set the data for the compilation of the vertex shader.
   *        used for when we need to recompile, so that the parameters dont
   *        need to be sent every time.
   */
  void
  setVSData(WString _fileName, const char* _entryPoint, const char* _model);

  /**
   * @brief Set the data for the compilation of the pixel shader.
   *        used for when we need to recompile, so that the parameters dont
   *        need to be sent every time.
   */
  void
  setPSData(WString _fileName, const char* _entryPoint, const char* _model);

  /**
   * @brief Compile both pixel and vertex shaders;
   */
  void
  compileShaders();

  /**
   * @brief Compile the Vertex Shader.
   */
  void
  compileVShader();

  /**
   * @brief Compile the Pixel Shader.
   */
  void
  compilePShader();

 private:
  /**
   * Shader pointers
   */
  SPtr<Shader> m_pVShader;
  SPtr<Shader> m_pPShader;

  SPtr<InputLayout> m_pInputLayout;

  SPtr<SamplerState> m_pSamplerState;

  /**
   * Data used to compile shaders
   */
  WString m_PShaderDirectory;
  WString m_VShaderDirectory;

  const char* m_PSEntryPoint;
  const char* m_VSEntryPoint;

  const char* m_PSModel;
  const char* m_VSModel;
};
}