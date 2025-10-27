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
   * @brief Create a texture resource from a file using STBI.
   * @param _path Where to look for the file.
   * @return texture resource.
   */
  virtual TextureResource*
  createResourceFromFile(const Path _path) = 0;
};

PK_CORE_EXPORT TextureCodec&
g_TextureCodec();
}
