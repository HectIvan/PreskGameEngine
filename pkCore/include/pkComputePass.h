/*****************************************************************************/
/**
 * @file    pkComputePass.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    29/08/2026
 * @brief   Header file for a compute pass.
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

class ComputePass : public Pass
{
 public:
  ComputePass() = default;
  ComputePass(const ComputePassDesc& _desc);
  ~ComputePass() = default;

  /**
   * @brief Clear the compute pass.
   */
  void
  clear() override;

  /**
   * @brief Update the constant buffers of the compute shader.
   * @param _data New data to update the buffers.
   * @param _sizes Sizes of the data.
   */
  void
  updateCBuffers(const Vector<const void*>& _data, const Vector<SIZE_T>& _sizes) const;

  /**
   * @brief Update a specific constant buffer of the compute shader.
   * @param _index CBuffer index.
   * @param _data New data to update the buffer.
   * @param _size Size of the data.
   */
  void
  updateCBuffer(const uint32 _index, const void* _data, const SIZE_T _size) const;

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

  /**
   * @brief Get the compute shader.
   */
  PK_NODISCARD PKFORCEINLINE const WPtr<Shader>&
  getCShader() const { return m_pCShader; }

  /**
   * @brief Set the compute shader.
   * @param _cShader Compute shader to set.
   */
  PKFORCEINLINE void
  setCShader(const WPtr<Shader>& _cShader) { m_pCShader = _cShader; }

  /**
   * @brief Get the constant buffers of the compute shader.
   */
  PK_NODISCARD PKFORCEINLINE const Vector<SPtr<ConstantBuffer>>&
  getCBuffers() const { return m_cCBuffers; }

  /**
   * @brief Set the constant buffers for the compute shader.
   * @param _cBuffers Constant buffers to set.
   */
  PKFORCEINLINE void
  setCBuffers(const Vector<SPtr<ConstantBuffer>>& _cBuffers) { m_cCBuffers = _cBuffers; }

  /**
   * @brief Get the unordered access view textures of the compute shader.
   */
  PK_NODISCARD PKFORCEINLINE const Vector<SPtr<Texture>>&
  getUAVTextures() const { return m_uavTex; }

  /**
   * @brief Set the unordered access view textures for the compute shader.
   * @param _uavTex Unordered access view textures to set.
   */
  PKFORCEINLINE void
  setUAVTextures(const Vector<SPtr<Texture>>& _uavTex) { m_uavTex = _uavTex; }

  /**
   * @brief Get the shader resources of the pass.
   */
  PK_NODISCARD PKFORCEINLINE const Vector<WPtr<Texture>>&
  getCShaderTextures() const { return m_cTex; }

  /**
   * @brief Set the shader resources for the compute shader.
   * @param _cTex Shader resources to set.
   */
  PKFORCEINLINE void
  setCShaderTextures(const Vector<WPtr<Texture>>& _cTex) { m_cTex = _cTex; }
  
 private:
  // shader.
  WPtr<Shader> m_pCShader;

  // buffers for the shader.
  Vector<SPtr<ConstantBuffer>> m_cCBuffers;
  // unordered access views.
  Vector<SPtr<Texture>> m_uavTex;
  // shader resources.
  Vector<WPtr<Texture>> m_cTex;
};
}