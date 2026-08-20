/*****************************************************************************/
/**
 * @file    pkTextureResource.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    15/10/2025
 * @brief   Texture resource for engine models.
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
#include "pkBaseResource.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT TextureResource : public BaseResource
{
 public:
  TextureResource() = default;
  virtual ~TextureResource() = default;

  /**
   * @brief Load the texture resource.
   */
  void
  load() override;

  /**
   * @brief Unload the texture resource.
   */
  void
  unload() override;

  /**
   * @brief Get the resource type.
   * @return The resource type.
   */
  RESOURCE_TYPE::E
  getType() const override { return RESOURCE_TYPE::kTexture; }

  /**
   * @brief Get the resource type as a string.
   * @return The resource type string.
   */
  String
  getTypeString() const override { return "Texture"; }

  /**
   * @brief Generate an id from the texture directory.
   */
  PK_NODISCARD static UUID
  generateID(const ANSICHAR* _texDir) {
    return UUID::generateRandomUUIDFromString(String(_texDir) + "Texture");
  }

  /**
   * @brief Generate an id from the texture directory.
   */
  PK_NODISCARD static UUID
  generateID(const String& _texDir) {
    return UUID::generateRandomUUIDFromString(_texDir + "Texture");
  }

  /**
   * @brief Generate an id from the texture directory.
   */
  PK_NODISCARD static UUID
  generateID(const Path& _texDir) {
    return UUID::generateRandomUUIDFromString(_texDir.toString() + "Texture");
  }

 public:
  int32 m_width;
  int32 m_height;
  int32 m_bpp;
  PK_GRAPHICS_FORMAT::E m_format;
  uint32 m_mipMapCount;
  Vector<uint8> m_data;
};
}
