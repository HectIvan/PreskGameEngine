/*****************************************************************************/
/**
 * @file    pkStbiTextureCodec.h
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    24/10/2025
 * @brief   Codec for creating pkt Texures.
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
#include "pkTextureCodec.h"

namespace pkEngineSDK
{

class StbiTextureCodec : public TextureCodec
{
 public:
  StbiTextureCodec() = default;
  virtual ~StbiTextureCodec() = default;

  /**
   * @brief Create a texture resource.
   * @param _path Where to create the file.
   * @param _name Name of the texture.
   * @param _width Width of the texture.
   * @param _height Height of the texture.
   * @param _bpp Bits per pixel of the texture.
   * @param _format Format of the texture.
   * @param _mipCount Mip map count of the texture.
   * @param _data Data of the texture.
   */
  SPtr<TextureResource>
  createResource(const Path& _path,
                 const String _name,
                 const int32 _width,
                 const int32 _height,
                 const int32 _bpp,
                 const uint32 _format,
                 const uint32 _mipCount,
                 Vector<uint8>& _data) override;

  /**
   * @brief Create a texture resource from a file using STBI.
   * @param _path Where to look for the file.
   * @return texture resource.
   */
  SPtr<TextureResource>
  createResourceFromFile(const Path _path) override;
}; 
}
