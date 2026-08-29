/*****************************************************************************/
/**
 * @file    pkPixelPass.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    29/08/2026
 * @brief   Header file for a pixel pass.
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
#include "pkPass.h"
#include "pkPassDesc.h"

namespace pkEngineSDK
{

class PixelPass : public Pass
{
 public:
  PixelPass() = default;
  PixelPass(const PixelPassDesc& _desc);
  ~PixelPass() = default;

  /**
   * @brief Clear the pixel pass.
   */
  void
  clear() override;

  /**
   * @brief Update the constant buffers of the vertex shader.
   * @param _data New data to update the buffers.
   * @param _sizes Sizes of the data.
   */
  void
  updateVCBuffers(const Vector<const void*>& _data, const Vector<SIZE_T>& _sizes) const;

  /**
   * @brief Update a specific constant buffer of the vertex shader.
   * @param _index CBuffer index.
   * @param _data New data to update the buffer.
   * @param _size Size of the data.
   */
  void
  updateVCBuffer(const uint32 _index, const void* _data, const SIZE_T _size) const;

  /**
   * @brief Update the constant buffers of the pixel shader.
   * @param _data New data to update the buffers.
   * @param _sizes Sizes of the data.
   */
  void
  updatePCBuffers(const Vector<const void*>& _data, const Vector<SIZE_T>& _sizes) const;

  /**
   * @brief Update a specific constant buffer of the pixel shader.
   * @param _index CBuffer index.
   * @param _data New data to update the buffer.
   * @param _size Size of the data.
   */
  void
  updatePCBuffer(const uint32 _index, const void* _data, const SIZE_T _size) const;

  /*********************************************/
  /**
  * Vertex shader stuff
  **/
  /*********************************************/

  /**
   * @brief Get the Vertex shader.
   */
  PK_NODISCARD PKFORCEINLINE const WPtr<Shader>&
  getVShader() const { return m_pVShader; }

  /**
   * @brief Set the Vertex shader.
   */
  PKFORCEINLINE void
  setVShader(const WPtr<Shader>& _vShader) { m_pVShader = _vShader; }

  /**
   * @brief Get the Vertex shader constant buffers.
   */
  PK_NODISCARD PKFORCEINLINE const Vector<SPtr<ConstantBuffer>>&
  getVShaderCBuffers() const { return m_vCBuffers; }

  /**
   * @brief Set the Vertex shader constant buffers.
   */
  PKFORCEINLINE void
  setVShaderCBuffers(const Vector<SPtr<ConstantBuffer>>& _cbuffers) { m_vCBuffers = _cbuffers; }

  /**
   * @brief Get the vertex shader Texure resources.
   */
  PK_NODISCARD PKFORCEINLINE const Vector<WPtr<Texture>>&
  getVShaderTextureResources() const { return m_VTex; }

  /**
   * @brief Set the vertex shader Texure resources.
   */
  PKFORCEINLINE void
  setVShaderTextureResources(const Vector<WPtr<Texture>>& _textures) { m_VTex = _textures; }

  /*********************************************/
  /**
  * Pixel shader stuff
  **/
  /*********************************************/

  /**
   * @brief Get the Pixel shader.
   */
  PK_NODISCARD PKFORCEINLINE const WPtr<Shader>&
  getPShader() const { return m_pPShader; }

  /**
   * @brief Set the Pixel shader.
   */
  PKFORCEINLINE void
  setPShader(const WPtr<Shader>& _pShader) { m_pPShader = _pShader; }

  /**
   * @brief Get the Pixel shader constant buffers.
   */
  PK_NODISCARD PKFORCEINLINE const Vector<SPtr<ConstantBuffer>>&
  getPShaderCBuffers() const { return m_pCBuffers; }

  /**
   * @brief Set the Pixel shader constant buffers.
   */
  PKFORCEINLINE void
  setPShaderCBuffers(const Vector<SPtr<ConstantBuffer>>& _cbuffers) { m_pCBuffers = _cbuffers; }

  /**
   * @brief Get the pixel shader Texure resources.
   */
  PK_NODISCARD PKFORCEINLINE const Vector<WPtr<Texture>>&
  getPShaderTextureResources() const { return m_PTex; }

  /**
   * @brief Set the pixel shader Texure resources.
   */
  PKFORCEINLINE void
  setPShaderTextureResources(const Vector<WPtr<Texture>>& _textures) { m_PTex = _textures; }



  /**
   * @brief Get the output textures of the pass.
   */
  PK_NODISCARD PKFORCEINLINE const Vector<WPtr<Texture>>&
  getOutputTextures() const { return m_outputTex; }

  /**
   * @brief Set the output textures of the pass.
   */
  PKFORCEINLINE void
  setOutputTextures(const Vector<WPtr<Texture>>& _textures) { m_outputTex = _textures; }

  /**
   * @brief Get the depth texture of the pass.
   */
  PK_NODISCARD PKFORCEINLINE const SPtr<Texture>&
  getDepthTexture() const { return m_depthTex; }

  /**
   * @brief Set the depth texture of the pass.
   */
  PKFORCEINLINE void
  setDepthTexture(const SPtr<Texture>& _depthTex) { m_depthTex = _depthTex; }

  /**
   * Behaviour functions for the pass.
   */

  /**
   * @brief Begin the pass.
   * @param _color Clear color.
   */
  void
  beginPass(const FColor& _color = FColor::CYAN) override;

  /**
   * @brief Set all parameters to null;
   */
  void
  endPass() override;

 private:

  // shaders
  WPtr<Shader> m_pVShader;
  WPtr<Shader> m_pPShader;

  // buffers for each shader stage.
  Vector<SPtr<ConstantBuffer>> m_vCBuffers;
  Vector<SPtr<ConstantBuffer>> m_pCBuffers;
  
  // textures for each shader stage.
  Vector<WPtr<Texture>> m_VTex;
  Vector<WPtr<Texture>> m_PTex;

  Vector<WPtr<Texture>> m_outputTex;

  SPtr<Texture> m_depthTex;
};
}