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

class DX11Device;
struct Color;

class DX11Texture : public Texture
{
 public:
  DX11Texture() = default;
  DX11Texture(ID3D11Texture2D* _t2d,
              Vector<ID3D11RenderTargetView*> _rTVs = { nullptr },
              ID3D11DepthStencilView* _dSV = nullptr,
              ID3D11ShaderResourceView* _Srv = nullptr,
              Vector<ID3D11UnorderedAccessView*> _uAVs = { nullptr }) :
    m_t2d(_t2d),
    m_rTVs(_rTVs),
    m_dSV(_dSV),
    m_sRV(_Srv),
    m_uAVs(_uAVs)
  {}
  ~DX11Texture() override {
    const String msg = "Releasing texture: " + getName();
    LOG_REGISTER(msg, __FILE__, __LINE__);
    LOG_PRINT(msg);

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
  ID3D11Texture2D*
  getTexture2D() { return m_t2d; }

  /**
   * @brief Set the DirectX texture.
   * @param _t2d Pointer to the new texture.
   */
  void
  setTexture2D(ID3D11Texture2D* _t2d) { m_t2d = _t2d; }

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
  String
  getName() override;

  /**
   * @brief Get the render target view.
   * @param _index Index of the RTV to get.
   * @return Pointer to the target.
   */
  ID3D11RenderTargetView*
  getRTV(int32 _index = 0) const { return m_rTVs[_index]; }

  /**
   * @brief Get the render target view.
   * @return Vect of render target views.
   */
  Vector<ID3D11RenderTargetView*>
  getRTVs() const { return m_rTVs; }

  /**
   * @brief Set the render target view.
   * @param _rTV New render target view.
   * @param _index Index of the RTV to set.
   */
  void
  setRTV(ID3D11RenderTargetView* _rTV, int32 _index) { m_rTVs[_index] = _rTV; }

  /**
   * @brief Set the render target views.
   * @param _rTV New render target views.
   */
  void
  setRTVs(Vector<ID3D11RenderTargetView*> _rTVs) { m_rTVs = _rTVs; }

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
   * @brief Get the unordered access view of an index.
   * @return Pointer to the view.
   */
  ID3D11UnorderedAccessView*
  getUAV(int32 _index) const { return m_uAVs[_index]; }

  /**
   * @brief Get the unordered access views.
   * @return Vector of UAVs.
   */
  Vector<ID3D11UnorderedAccessView*>
  getUAVs() const { return m_uAVs; }

  /**
   * @brief Set the unordered access view.
   * @param _sRV New unordered access view.
   * @param _index Index of the UAV to set.
   */
  void
  setUAV(ID3D11UnorderedAccessView* _uAV, int32 _index) { m_uAVs[_index] = _uAV; }

  /**
   * @brief Set the unordered access views.
   * @param _sRV New unordered access views.
   */
  void
  setUAVs(Vector<ID3D11UnorderedAccessView*> _uAVs) { m_uAVs = _uAVs; }

  /**
   * @brief Get the raw texture data.
   * @return The raw texture data.
   */
  void*
  getRawData() override { return reinterpret_cast<void*>(m_sRV); }

 public:
  ID3D11Texture2D* m_t2d = nullptr;

  Vector<ID3D11RenderTargetView*> m_rTVs;

  ID3D11DepthStencilView* m_dSV = nullptr;

  ID3D11ShaderResourceView* m_sRV = nullptr;

  Vector<ID3D11UnorderedAccessView*> m_uAVs;
};
}
