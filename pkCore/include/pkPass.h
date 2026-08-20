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
#include "pkFColor.h"

namespace pkEngineSDK
{

namespace PASS_MODEL
{
enum E
{
  kPass_Pixel = 0,
  kPass_Compute,
  kPass_Geometry,
};
}

namespace PK_PASS_CLEAR_FLAGS
{
enum E
{
  kPass_Unknown = 0,
  kPass_RT = 1,
  kPass_Depth = 2,
  kPass_UAV = 3,
};
}

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
   * @brief Sets the input layout to the pass.
   * @param _pInput Pointer to the new layout.
   */
  PKFORCEINLINE void
  setInputLayout(const SPtr<InputLayout>& _pILayout) { m_pInputLayout = _pILayout; }

  /**
   * @brief Gets the input layout to the pass.
   * @return Pointer of the Input Layout.
   */
  PK_NODISCARD PKFORCEINLINE const SPtr<InputLayout>&
  getInputLayout() const { return m_pInputLayout; }

  /**
   * @brief Set the sampler state of the pass.
   * @param _samState Reference to the sampler state.
   */
  PKFORCEINLINE void
  setSamplerState(const SPtr<SamplerState>& _samState) { m_pSamplerState = _samState; }

  /**
   * @brief Get the sampler state of this pass.
   * @return Pointer to the sampler state.
   */
  PK_NODISCARD PKFORCEINLINE const SPtr<SamplerState>&
  getSamplerState() const { return m_pSamplerState; }

  /**
   * @brief Set the Vertex shader of the pass.
   */
  PKFORCEINLINE void
  setVShader(const SPtr<Shader>& _vShader) { m_pVShader = _vShader; }

  /**
   * @brief Set the Pixel shader of the pass.
   */
  PKFORCEINLINE void
  setPShader(const SPtr<Shader>& _pShader) { m_pPShader = _pShader; }

  /**
   * @brief Set the Compute shader of the pass.
   */
  PKFORCEINLINE void
  setCShader(const SPtr<Shader>& _cShader) { m_pCShader = _cShader; }

  /**
   * @brief Get the vertex shader.
   * @return The pointer to the vertex shader.
   */
  PK_NODISCARD PKFORCEINLINE const WPtr<Shader>&
  getVShader() const { return m_pVShader; }

  /**
   * @brief Get the pixel shader.
   * @return The pointer to the pixel shader.
   */
  PK_NODISCARD PKFORCEINLINE const WPtr<Shader>&
  getPShader() const { return m_pPShader; }

  /**
   * @brief Get the compute shader.
   * @return The pointer to the compute shader.
   */
  PK_NODISCARD PKFORCEINLINE const WPtr<Shader>&
  getCShader() const { return m_pCShader; }

  /**
   * @brief Update the constant buffers of a pass.
   * @param _data Array of data to use in the update.
   * @param _sizes Size of each data blob.
   */
  void
  updateCBuffers(const Vector<const void*>& _data,
                 const Vector<SIZE_T>& _sizes) const;

  /**
   * @brief Update a specific constant buffer.
   * @param _index Index of the cbuffer to update.
   * @param _data Data to use in the update.
   * @param _size Size of the data.
   */
  void
  updateCBuffer(const uint32 _index, const void* _data, const SIZE_T _size) const;

  /**
   * @brief Get the constant buffer vector.
   * @return The vector of cBuffers.
   */
  PK_NODISCARD PKFORCEINLINE const Vector<SPtr<ConstantBuffer>>&
  getCBuffers() const { return m_cBuffers; }

  /**
   * @brief Get the constant buffer in the index position.
   * @param _index Where to look for the cbuffer.
   * @return Pointer to the cbuffer.
   */
  PK_NODISCARD PKFORCEINLINE SPtr<ConstantBuffer>
  getCBuffer(const uint32& _index) const { return m_cBuffers[_index]; }

  /**
   * @brief Start pass based parameter setting.
   * @param _color Clear color.
   */
  void
  beginPass(const FColor& _color = FColor::CYAN,
            const PK_PASS_CLEAR_FLAGS::E& _clearFlags = PK_PASS_CLEAR_FLAGS::kPass_RT) const;

  /**
   * @brief Set all parameters to null;
   */
  void
  endPass() const;

  /**
   * @brief get the input textures of the pass.
   * @return A list of Textures.
   */
  PK_NODISCARD PKFORCEINLINE const Vector<WPtr<Texture>>&
  getInputTextures() const { return m_inputTex; }

  /**
   * @brief get the output textures of the pass.
   * @return A list of Textures.
   */
  PK_NODISCARD PKFORCEINLINE const Vector<WPtr<Texture>>&
  getOutputTextures() const { return m_outputTex; }

  /**
   * @brief get the unordered access view textures of the pass.
   * @return A list of Textures.
   */
  PK_NODISCARD PKFORCEINLINE const Vector<SPtr<Texture>>&
  getUAVTextures() const { return m_uavTex; }

  /**
   * @brief Get the rasterizer state of the pass.
   */
  PKFORCEINLINE void
  setRasterizerState(const SPtr<RasterizerState>& _pRState) { m_pRasterizerState = _pRState; }

  /**
   * @brief Get the rasterizer state of the pass.
   */
  PK_NODISCARD PKFORCEINLINE const SPtr<RasterizerState>&
  getRasterizerState() const { return m_pRasterizerState; }

  /**
   * @brief Set the viewport size.
   */
  PKFORCEINLINE void
  setViewport(const Vector2& _viewport) { m_viewPortSize = _viewport; }

  /**
   * @brief Get the viewport size.
   */
  PK_NODISCARD PKFORCEINLINE const Vector2
  getViewport() const { return m_viewPortSize; }

  /**
   * @brief Compile both pixel and vertex shaders;
   */
  void
  compileShaders();

 private:
  /**
   * @brief Start pass based parameter setting for pixel shader passes.
   * @param _color Clear color.
   */
  void
  beginPixel(const FColor& _color,
             const PK_PASS_CLEAR_FLAGS::E& _clearFlags = PK_PASS_CLEAR_FLAGS::kPass_RT) const;

  /**
   * @brief End pass based parameter setting for pixel shader passes.
   */
  void
  endPixel() const;

  /**
   * @brief Start pass based parameter setting for compute shader passes.
   * @param _color Clear color.
   */
  void
  beginCompute(const FColor& _color,
               const PK_PASS_CLEAR_FLAGS::E& _clearFlags = PK_PASS_CLEAR_FLAGS::kPass_UAV) const;

  /**
   * @brief End pass based parameter setting for compute shader passes.
   */
  void
  endCompute() const;

  /**
   * @brief (TO_IMPLEMENT) Start pass based parameter setting for geometry shader passes.
   * @param _color Clear color.
   */
  void
  beginGeometry(const FColor& _color) const;

  /**
   * @brief (TO IMPLEMENT) End pass based parameter setting for geometry shader passes.
   */
  void
  endGeometry() const;

  /**
   * @brief Create the basic members of all passes.
   * @param _desc Base pass descriptor.
   */
  void
  createBasics(const PassDesc& _desc);

 public:
  String m_name;

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
  Vector<WPtr<Texture>> m_inputTex;
  Vector<WPtr<Texture>> m_outputTex;
  Vector<SPtr<Texture>> m_uavTex;
  SPtr<Texture> m_depthTex;

  PASS_MODEL::E m_passModel;
};
}
