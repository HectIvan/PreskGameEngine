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

class TextureResource : public BaseResource
{
 public:
  TextureResource() = default;
  virtual ~TextureResource() = default;

  /**
   * @brief Get the resource type.
   * @return The resource type.
   */
  RESOURCE_TYPE::E
  getType() const { return RESOURCE_TYPE::kTexture; }

 public:
  uint32 m_width;
  uint32 m_height;
  uint32 m_format;
  Vector<uint8> m_data;
};
}
