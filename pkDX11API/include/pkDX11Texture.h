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
  DX11Texture(ID3D11Texture2D* _t2d) :
    m_t2d(_t2d),
    m_owner(false)
  {}
  virtual ~DX11Texture()
  {
    safeRelease(m_srv);
    safeRelease(m_dSV);
    safeRelease(m_rTV);
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
  getSRV() const { return m_srv; }

  /**
   * @brief Set the shader resource view.
   * @param _srv New shader resource view.
   */
  void
  setSRV(ID3D11ShaderResourceView* _srv) { m_srv = _srv; }

 public:
  ID3D11Texture2D* m_t2d = nullptr;

  ID3D11RenderTargetView* m_rTV;

  ID3D11DepthStencilView* m_dSV;

  ID3D11ShaderResourceView* m_srv;

  bool m_owner = true;
};
}
