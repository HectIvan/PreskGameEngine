/**
 * @file    pkDX11Texture.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    22/11/2024
 * @brief   DX Texture file for the Presk Game Engine.
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
#include "pkDX11Prerequisites.h"
#include "pkDX11Device.h"
#include "pkTexture.h"

namespace pkEngineSDK
{

class DX11Device;
struct Color;

class DX11Texture : public Texture
{
 public:
  DX11Texture() = default;
  DX11Texture(ID3D11Texture2D* _t2d,
              ID3D11RenderTargetView* _rTV = nullptr,
              ID3D11DepthStencilView* _dSV = nullptr,
              ID3D11ShaderResourceView* _Srv = nullptr,
              ID3D11UnorderedAccessView* _uAV = nullptr) :
    m_t2d(_t2d),
    m_rTV(_rTV),
    m_dSV(_dSV),
    m_sRV(_Srv),
    m_uAV(_uAV),
    m_owner(false)
  {}
  virtual ~DX11Texture()
  {
    safeRelease(m_sRV);
    safeRelease(m_dSV);
    safeRelease(m_rTV);
    safeRelease(m_uAV);
    if (m_owner) { safeRelease(m_t2d); }
  }

  /**
   * @brief Get the DirectX texture.
   * @return Pointer to the resource.
   */
  ID3D11Texture2D*
  getTexture2D() { return m_t2d; }

  /**
   * @brief Set the DirectX texture.
   * @param _t2d Pointer to the new texture.
   */
  void
  setTexture2D(ID3D11Texture2D* _t2d) { m_t2d = _t2d; }

  /**
   * @brief Get the shader resource view.
   * @return Pointer to the resource.
   */
  ID3D11ShaderResourceView*
  getSRV() const { return m_sRV; }

  /**
   * @brief Set the shader resource view.
   * @param _sRV New shader resource view.
   */
  void
  setSRV(ID3D11ShaderResourceView* _sRV) { m_sRV = _sRV; }

  /**
   * @brief Get the unordered access view.
   * @return Pointer to the view.
   */
  ID3D11UnorderedAccessView*
  getUAV() const { return m_uAV; }

  /**
   * @brief Set the unordered access view.
   * @param _sRV New unordered access view.
   */
  void
  setUAV(ID3D11UnorderedAccessView* _uAV) { m_uAV = _uAV; }

  /**
   * @brief Get the raw texture data.
   * @return The raw texture data.
   */
  void*
  getRawData() override { return static_cast<void*>(m_sRV); }

 public:
  ID3D11Texture2D* m_t2d = nullptr;

  ID3D11RenderTargetView* m_rTV = nullptr;

  ID3D11DepthStencilView* m_dSV = nullptr;

  ID3D11ShaderResourceView* m_sRV = nullptr;

  ID3D11UnorderedAccessView* m_uAV = nullptr;

  bool m_owner = true;
};
}
