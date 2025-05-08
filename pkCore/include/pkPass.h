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

// class Shader;

class Pass
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
   * @brief Gets the input layout to the pass.
   * @return Pointer of the Input Layout.
   */
  SPtr<InputLayout>
  getInputLayout() { return m_pInputLayout; }

  /**
   * @brief Create the sampler state.
   */
  void
  createSamplerState(uint32 _mode, uint32 _filter);

  /**
   * @brief Get the sampler state of this pass.
   * @return Pointer to the sampler state.
   */
  SPtr<SamplerState>
  getSamplerState() { return m_pSamplerState; }

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
   * @brief Create the shaders from their blobs
   */
  void
  createShaders();

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

  /**
   * @brief Get the vertex shader.
   * @return The pointer to the vertex shader.
   */
  SPtr<Shader>
  getVShader() { return m_pVShader; }

  /**
   * @brief Get the pixel shader.
   * @return The pointer to the pixel shader.
   */
  SPtr<Shader>
  getPShader() { return m_pPShader; }

 private:
  /**
   * Shader pointers
   */
  SPtr<Shader> m_pVShader = nullptr;
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