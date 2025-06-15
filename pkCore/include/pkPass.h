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
#include "pkPrerequisitesCore.h"
#include "pkShader.h"

namespace pkEngineSDK
{

// class Shader;

class Pass
{
public:
  Pass();
  Pass(PassDesc& _desc);
  virtual ~Pass() = default;

  /**
   * @brief Gets the input layout to the pass.
   * @return Pointer of the Input Layout.
   */
  SPtr<InputLayout>&
  getInputLayout() { return m_pInputLayout; }

  /**
   * @brief Get the sampler state of this pass.
   * @return Pointer to the sampler state.
   */
  SPtr<SamplerState>&
  getSamplerState() { return m_pSamplerState; }

  /**
   * @brief Create the vertex shader.
   */
  void
  createVShader(const WString _directory, const char* _entry, const char* _sModel);

  /**
   * @brief Create the pixel shader.
   */
  void
  createPShader(const WString _directory, const char* _entry, const char* _sModel);

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
   * @brief Get the constant buffer vector.
   * @return The vector of cBuffers.
   */
  Vector<SPtr<ConstantBuffer>>
  getCBuffers() { return m_cBuffers; }

  /**
   * @brief Get the constant buffer in the index position.
   * @param _index Where to look for the cbuffer.
   * @return Pointer to the cbuffer.
   */
  SPtr<ConstantBuffer>&
  getCBuffer(uint32 _index) { return m_cBuffers[_index]; }

  /**
   * @brief Start pass based parameter setting.
   * @param _color Clear color.
   */
  void
  beginPass(Color _color = Color(0, 30, 76, 255));

  /**
   * @brief Set all parameters to null;
   */
  void
  endPass();

 private:
  /**
   * Shader pointers
   */
  SPtr<Shader> m_pVShader;
  SPtr<Shader> m_pPShader;

  SPtr<InputLayout> m_pInputLayout;
  SPtr<SamplerState> m_pSamplerState;

  // constant buffers for the shaders.
  Vector<SPtr<ConstantBuffer>> m_cBuffers;

  // input and output textures of the pass
  Vector<SPtr<Texture>> m_inputTex;
  SPtr<Texture> m_depthTex;
  Vector<SPtr<Texture>> m_outputTex;
};
}
