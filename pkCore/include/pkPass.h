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
#include "pkPassDesc.h"
#include "pkShader.h"
#include "pkConstantBuffer.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT Pass
{
 public:
  Pass();
  Pass(PassDesc& _desc);
  virtual ~Pass();

  /**
   * @brief Gets the input layout to the pass.
   * @return Pointer of the Input Layout.
   */
  SPtr<InputLayout>&
  getInputLayout() { return m_pInputLayout; }

  /**
   * @brief Set the sampler state of the pass.
   * @param _samState Reference to the sampler state.
   */
  void
  setSamplerState(SPtr<SamplerState> _samState) { m_pSamplerState = _samState; }

  /**
   * @brief Get the sampler state of this pass.
   * @return Pointer to the sampler state.
   */
  SPtr<SamplerState>&
  getSamplerState() { return m_pSamplerState; }

  /**
   * @brief Create the vertex shader.
   * @param _directory Directory of the shader.
   * @param _entry Name of function to execute.
   * @param _sModel Shader model.
   */
  void
  createVShader(const Path _directory, const ANSICHAR* _entry, const ANSICHAR* _sModel);

  /**
   * @brief Create the pixel shader.
   * @param _directory Directory of the shader.
   * @param _entry Name of function to execute.
   * @param _sModel Shader model.
   */
  void
  createPShader(const Path _directory, const ANSICHAR* _entry, const ANSICHAR* _sModel);

  /**
   * @brief Create the compute shader.
   * @param _directory Directory of the shader.
   * @param _entry Name of function to execute.
   * @param _sModel Shader model.
   */
  void
  createCShader(const Path _directory, const ANSICHAR* _entry, const ANSICHAR* _sModel);

  /**
   * @brief Compile both pixel and vertex shaders;
   */
  void
  compileShaders();

  /**
   * @brief Get the vertex shader.
   * @return The pointer to the vertex shader.
   */
  SPtr<Shader>&
  getVShader() { return m_pVShader; }

  /**
   * @brief Get the pixel shader.
   * @return The pointer to the pixel shader.
   */
  SPtr<Shader>&
  getPShader() { return m_pPShader; }

  /**
   * @brief Get the compute shader.
   * @return The pointer to the compute shader.
   */
  SPtr<Shader>&
  getCShader() { return m_pCShader; }

  /**
   * @brief Get the constant buffer vector.
   * @return The vector of cBuffers.
   */
  Vector<SPtr<ConstantBuffer>>
  getCBuffers() { return m_cBuffers; }

  /**
   * @brief Update the constant buffers of a pass.
   * @param _data Array of data to use in the update.
   * @param _sizes Size of each data blob.
   */
  void
  updateCBuffers(const Vector<const void*>& _data,
                 const Vector<SIZE_T>& _sizes);

  /**
   * @brief Get the constant buffer in the index position.
   * @param _index Where to look for the cbuffer.
   * @return Pointer to the cbuffer.
   */
  SPtr<ConstantBuffer>&
  getCBuffer(const uint32 _index) { return m_cBuffers[_index]; }

  /**
   * @brief Start pass based parameter setting.
   * @param _color Clear color.
   */
  void
  beginPass(const Color _color = Color(0, 1, 1, 0));

  /**
   * @brief Set all parameters to null;
   */
  void
  endPass();

  /**
   * @brief get the input textures of the pass.
   * @return A list of Textures.
   */
  Vector<SPtr<Texture>>&
  getInputTextures() { return m_inputTex; }

  /**
   * @brief get the output textures of the pass.
   * @return A list of Textures.
   */
  Vector<SPtr<Texture>>&
  getOutputTextures() { return m_outputTex; }

  /**
   * @brief get the unordered access view textures of the pass.
   * @return A list of Textures.
   */
  Vector<SPtr<Texture>>&
  getUAVTextures() { return m_uavTex; }

  /**
   * @brief Get the viewport size.
   * @return viewport size.
   */
  Vector2
  getViewportSize() { return m_viewPortSize; }

 private:
  /**
   * Shader pointers
   */
  SPtr<Shader> m_pVShader = nullptr;
  SPtr<Shader> m_pPShader = nullptr;
  SPtr<Shader> m_pCShader = nullptr;

  SPtr<InputLayout> m_pInputLayout = nullptr;
  // states
  SPtr<SamplerState> m_pSamplerState = nullptr;
  SPtr<RasterizerState> m_pRasterizerState = nullptr;

  // constant buffers for the shaders.
  Vector<SPtr<ConstantBuffer>> m_cBuffers;

  Vector2 m_viewPortSize;

  // input and output textures of the pass
  Vector<SPtr<Texture>> m_inputTex;
  Vector<SPtr<Texture>> m_outputTex;
  Vector<SPtr<Texture>> m_uavTex;
  SPtr<Texture> m_depthTex;
};
}
