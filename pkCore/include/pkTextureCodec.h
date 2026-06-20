/*****************************************************************************/
/**
 * @file    pkStbiTextureCodec.h
 * @author  Héctor Iván Muñoz Ceballos
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
 */
/*********************************************/
#include "pkModule.h"
#include "pkTextureResource.h"
#include "pkPath.h"
#include "pkPrerequisitesCore.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT TextureCodec : public Module<TextureCodec>
{
 public:
  TextureCodec() = default;
  virtual ~TextureCodec() = default;

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
  virtual SPtr<TextureResource>
  createResource(const String& _name,
                 const int32& _width,
                 const int32& _height,
                 const int32& _bpp,
                 const uint32& _format,
                 const uint32& _mipCount,
                 Vector<uint8>& _data) = 0;

  /**
   * @brief Create a texture resource from a file using STBI.
   * @param _path Where to look for the file.
   * @return texture resource.
   */
  virtual SPtr<TextureResource>
  createResource(const Path& _path) = 0;

  /**
   * @brief Get the possible extensions for texture search in folders.
   * @return possible extensions for texture search in folders.
   */
  const String
  getPossibleExtensions() const {
    return "*.png;*.jpeg;*.jpg;*.tga;*.hdr;*.exr";
  }
};

PK_CORE_EXPORT TextureCodec&
g_TextureCodec();
}
