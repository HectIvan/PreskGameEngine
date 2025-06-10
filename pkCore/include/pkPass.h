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
  Pass();
  virtual ~Pass() = default;

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
  SPtr<Shader>
  getVShader() { return m_pVShader; }

  /**
   * @brief Get the pixel shader.
   * @return The pointer to the pixel shader.
   */
  SPtr<Shader>
  getPShader() { return m_pPShader; }

  /**
   * @brief Create a constant buffer for this pass and stores it in the buffer vector.
   * @param _size Size of the CBuffer.
   * @param _pData Data that the buffer will store.
   * @param _usage What usage will be given to the CBuffer.
   * @return Pointer to the buffer
   */
  SPtr<ConstantBuffer>
  createCBuffer(SIZE_T _size, const void* _data = nullptr, uint32 _usage = 0);

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
  SPtr<ConstantBuffer>
  getCBuffer(uint32 _index) { return m_cBuffers[_index]; }

  /**
   * @brief Adds a new buffer to the CBuffer vector.
   * @param _pCBuffer Buffer to add.
   */
  void
  addToCBuffers(SPtr<ConstantBuffer> _pCBuffer);

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

};
}