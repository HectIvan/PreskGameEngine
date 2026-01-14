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

struct TextureAssetHeader
{
  int32 width;
  int32 height;
  int32 bpp;
  uint32 format;
  uint32 mipMapCount;
};

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

 public:
  int32 m_width;
  int32 m_height;
  int32 m_bpp;
  uint32 m_format;
  uint32 m_mipMapCount;
  Vector<uint8> m_data;
};
}
