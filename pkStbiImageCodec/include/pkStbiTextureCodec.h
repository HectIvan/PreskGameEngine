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
   * @brief Create a texture resource from a file using STBI.
   * @param _path Where to look for the file.
   * @return texture resource.
   */
  TextureResource*
    createResourceFromFile(const Path _path) override;
}; 
}
