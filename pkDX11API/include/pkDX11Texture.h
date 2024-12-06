/************************************************************************/
/**
* @pkTexture pkTexture.h
* @Hector Ivan Muñoz Ceballos
* @date 22/11/2024
* @Texture file for the Presk Game Engine.
*
* Texture class for the engine
*
* @bug No bug known.
*
* @HectIvan 25/11/2024
* Created the file
*/
/************************************************************************/
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
  virtual ~DX11Texture()
  {
    clear();
  }

  /**
  * Get a unique pointer to the current texture.
  * 
  * @return
  * The new unique pointer as the child class.
  **/
  UPtr<Texture>
  getTexture() override;

  /**
  * Create the texture.
  * 
  * Creates the texture based on DirectX.
  * 
  * @param _pDevice
  * Device that will create the texture.
  * 
  * @param _width
  * Width of the texture.
  * 
  * @param _height
  * Height of the texture.
  * 
  * @param _colors
  * Array of colors that the texture will have.
  **/
  void
  create(DX11Device* _pDevice,
         uint32 _width,
         uint32 _height,
         Vector<Color> _colors);

  /**
  * Gets a pointer to the DX11 texture.
  * 
  * @return
  * Pointer to the resource.
  **/
  ID3D11Texture2D*
  getTexture2D() { return m_t2d; }

  /**
  * Set the DirectX texture.
  * 
  * @param _t2d
  * Pointer to the new texture.
  **/
  void
  setTexture2D(ID3D11Texture2D* _t2d) { m_t2d = _t2d; }

  /**
  * Gets a pointer to the shader resource view.
  * 
  * @return
  * Pointer to the resource.
  **/
  ID3D11ShaderResourceView*
  getSRV() const { return m_srv; }

  /**
  * Sets the shader resource view of the texture.
  * 
  * @param _srv
  * New shader resource view.
  **/
  void
  setSRV(ID3D11ShaderResourceView* _srv) { m_srv = _srv; }

  /**
  * Gets the type of the current texture.
  * 
  * @return
  * Type as an uint32.
  **/
  const uint32
  getType() { return m_type; }

  /**
  * Set the type of the texture.
  * 
  * @param _type
  * What type of texture it will be
  **/
  void
  setType(uint32 _type) { m_type = _type; }

  /**
  * Clears all the members of the texture.
  **/
  void
  clear();

 private:
  ID3D11Texture2D* m_t2d = nullptr;
  ID3D11ShaderResourceView* m_srv;
  uint32 m_type;
};
}
