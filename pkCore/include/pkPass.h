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
  Pass() = default;
  ~Pass() = default;

  /**
   * @brief Get the name of the pass.
   */
  PK_NODISCARD PKFORCEINLINE const String&
  getName() const { return m_name; }

  /**
   * @brief Set the name of the pass.
   */
  PKFORCEINLINE void
  setName(const String& _name) { m_name = _name; }

  /**
   * @brief Clear the pass of all shaders, states and textures.
   */
  virtual void
  clear() = 0;

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
   * @brief Begin the pass.
   * @param _color Clear color.
   */
  virtual void
  beginPass(const FColor& _color = FColor::CYAN) = 0;

  /**
   * @brief Set all parameters to null;
   */
  virtual void
  endPass() = 0;

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

 protected:

  /**
   * @brief Create the basic members of all passes.
   * @param _desc Base pass descriptor.
   */
  void
  createBasics(const PassDesc& _desc);

 public:
  String m_name;

 protected:
  Vector2 m_viewPortSize;

  SPtr<InputLayout> m_pInputLayout = nullptr;
  SPtr<SamplerState> m_pSamplerState = nullptr;
  SPtr<RasterizerState> m_pRasterizerState = nullptr;

  PASS_MODEL::E m_passModel;
};
}
