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

namespace pkEngineSDK
{

class DX11Device;
class Color;

class DX11Texture
{
 public:
  DX11Texture() = default;
  virtual ~DX11Texture()
  {
    clear();
  }

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
  * Gets a pointer to the shader resource view.
  * 
  * @return
  * Pointer to the resource.
  **/
  ID3D11ShaderResourceView*
  getSRV() { return m_srv; }

  /**
  * Gets the type of the current texture.
  * 
  * @return
  * Type as an uint32.
  **/
  uint32
  getType() { return m_type; }

  /**
  * Clears all the members of the texture.
  **/
  void
  clear();

 private:
  ID3D11Texture2D* m_t2d = nullptr;
  ID3D11ShaderResourceView* m_srv = nullptr;
  uint32 m_type;
};
}
