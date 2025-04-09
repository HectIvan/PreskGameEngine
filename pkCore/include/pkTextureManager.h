/*****************************************************************************/
/**
 * @file    pkTextureManager.cpp
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    19/11/2024
 * @brief   Manager used for creating and storing textures.
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
#include "pkPrerequisitesCore.h"
#include "pkTexture.h"
#include "pkModule.h"

namespace pkEngineSDK
{

struct TextureMemory
{
  String name;
  SPtr<Texture> texture;
};

class PK_CORE_EXPORT TextureManager : public Module<TextureManager>
{
 public:
  TextureManager() = default;
  virtual ~TextureManager() = default;

  /**
   * @brief Creates a texture from a texture name.
   * @param _name Name of the texture to load (extenison type is required).
   * @param _directory Where to look for (textures/ set as default).
   * @return Pointer to the newly created texture.
   */
  SPtr<Texture>
  createTexture(String _name, String _directory = "textures/");

 public:
  Vector<SPtr<TextureMemory>> m_textures;
};
PK_CORE_EXPORT TextureManager&
g_TextureManager();
}
