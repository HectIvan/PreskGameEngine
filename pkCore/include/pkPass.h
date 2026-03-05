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
  Pass(const PixelDesc& _desc);
  Pass(const ComputeDesc& _desc);
  ~Pass() {
    clear();
  }

  /**
   * @brief Clear the pass of all shaders, states and textures.
   */
  void
  clear();

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
   * @brief Compile both pixel and vertex shaders;
   */
  void
  compileShaders();

  /**
   * @brief Get the vertex shader.
   * @return The pointer to the vertex shader.
   */
  WPtr<Shader>&
  getVShader() { return m_pVShader; }

  /**
   * @brief Get the pixel shader.
   * @return The pointer to the pixel shader.
   */
  WPtr<Shader>&
  getPShader() { return m_pPShader; }

  /**
   * @brief Get the compute shader.
   * @return The pointer to the compute shader.
   */
  WPtr<Shader>&
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
   * @brief Update a specific constant buffer.
   * @param _index Index of the cbuffer to update.
   * @param _data Data to use in the update.
   * @param _size Size of the data.
   */
  void
  updateCBuffer(const uint32 _index, const void* _data, const SIZE_T _size);

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
  beginPass(const Color& _color = Color(0, 1, 1, 0));

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
   * @brief Create the basic members of all passes.
   * @param _desc Base pass descriptor.
   */
  void
  createBasics(const PassDesc& _desc);

 private:
  Vector2 m_viewPortSize;

  /**
   * Shader pointers
   */
  WPtr<Shader> m_pVShader;
  WPtr<Shader> m_pPShader;
  WPtr<Shader> m_pCShader;

  SPtr<InputLayout> m_pInputLayout = nullptr;
  SPtr<SamplerState> m_pSamplerState = nullptr;
  SPtr<RasterizerState> m_pRasterizerState = nullptr;

  /**
   * Shader CBuffers.
   */
  Vector<SPtr<ConstantBuffer>> m_cBuffers;

  /**
   * Textures.
   */
  Vector<SPtr<Texture>> m_inputTex;
  Vector<SPtr<Texture>> m_outputTex;
  Vector<SPtr<Texture>> m_uavTex;
  SPtr<Texture> m_depthTex;
};
}
