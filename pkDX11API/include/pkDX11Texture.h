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
#include "pkLogger.h"

namespace pkEngineSDK
{

class DX11Texture : public Texture
{
 public:
  DX11Texture() = default;
  DX11Texture(const Vector2& _size) {
    setSize(_size);
  }
  DX11Texture(const uint32& _width, const uint32& _height) {
    setWidth(_width);
    setHeight(_height);
  }
  DX11Texture(D3DTexture2D* _t2d,
              const Vector<PKRenderTargetView*>& _rTVs = { nullptr },
              PKDepthStencilView* _dSV = nullptr,
              PKShaderResourceView* _Srv = nullptr,
              const Vector<PKUnorderedAccessView*>& _uAVs = { nullptr }) :
    m_t2d(_t2d),
    m_rTVs(_rTVs),
    m_dSV(_dSV),
    m_sRV(_Srv),
    m_uAVs(_uAVs)
  {}
  ~DX11Texture() override {
    const String msg = "Releasing texture: " + getName();
    LOG_REGISTER(msg, __FILE__, __LINE__);

    m_rTVs.clear();
    m_uAVs.clear();

    safeRelease(m_sRV);
    safeRelease(m_dSV);
    safeRelease(m_t2d);
  }

  /**
   * @brief Get the DirectX texture.
   * @return Pointer to the resource.
   */
  PK_NODISCARD PKFORCEINLINE D3DTexture2D*
  getTexture2D() const { return m_t2d; }

  /**
   * @brief Set the DirectX texture.
   * @param _t2d Pointer to the new texture.
   */
  PKFORCEINLINE void
  setTexture2D(D3DTexture2D* _t2d) { m_t2d = _t2d; }

  /**
   * @brief Set the name of the texture.
   * @param _name New name of the texture.
   */
  void
  setName(const String& _name) override;

  /**
   * @brief Get the name of the texture.
   * @return Name of the texture.
   */
  const String
  getName() const override;

  /**
   * @brief Get the render target view.
   * @param _index Index of the RTV to get.
   * @return Pointer to the target.
   */
  PK_NODISCARD PKFORCEINLINE const PKRenderTargetView*
  getRTV(const int32& _index = 0) const { return m_rTVs[_index]; }

  /**
   * @brief Set the render target view.
   * @param _rTV New render target view.
   * @param _index Index of the RTV to set.
   */
  PKFORCEINLINE void
  setRTV(PKRenderTargetView* _rTV, const int32& _index) { m_rTVs[_index] = _rTV; }

  /**
   * @brief Get the render target view.
   * @return Vect of render target views.
   */
  PK_NODISCARD PKFORCEINLINE const Vector<PKRenderTargetView*>&
  getRTVs() const { return m_rTVs; }

  /**
   * @brief Set the render target views.
   * @param _rTV New render target views.
   */
  PKFORCEINLINE void
  setRTVs(const Vector<PKRenderTargetView*>& _rTVs) { m_rTVs = _rTVs; }

  /**
   * @brief Get the shader resource view.
   * @return Pointer to the resource.
   */
  PK_NODISCARD PKFORCEINLINE PKShaderResourceView*
  getSRV() const { return m_sRV; }

  /**
   * @brief Set the shader resource view.
   * @param _sRV New shader resource view.
   */
  PKFORCEINLINE void
  setSRV(PKShaderResourceView* _sRV) { m_sRV = _sRV; }

  /**
   * @brief Get the unordered access view of an index.
   * @return Pointer to the view.
   */
  PK_NODISCARD PKFORCEINLINE const PKUnorderedAccessView*
  getUAV(const int32& _index) const { return m_uAVs[_index]; }

  /**
   * @brief Get the unordered access views.
   * @return Vector of UAVs.
   */
  PK_NODISCARD PKFORCEINLINE const Vector<PKUnorderedAccessView*>&
  getUAVs() const { return m_uAVs; }

  /**
   * @brief Set the unordered access view.
   * @param _sRV New unordered access view.
   * @param _index Index of the UAV to set.
   */
  PKFORCEINLINE void
  setUAV(PKUnorderedAccessView* _uAV, const int32& _index) { m_uAVs[_index] = _uAV; }

  /**
   * @brief Set the unordered access views.
   * @param _sRV New unordered access views.
   */
  PKFORCEINLINE void
  setUAVs(const Vector<PKUnorderedAccessView*>& _uAVs) { m_uAVs = _uAVs; }

  /**
   * @brief Get the raw texture data.
   * @return The raw texture data.
   */
  PKFORCEINLINE void*
  getRawData() override { return reinterpret_cast<void*>(m_sRV); }

 public:
  D3DTexture2D* m_t2d = nullptr;

  Vector<PKRenderTargetView*> m_rTVs;

  PKDepthStencilView* m_dSV = nullptr;

  PKShaderResourceView* m_sRV = nullptr;

  Vector<PKUnorderedAccessView*> m_uAVs;
};
}
