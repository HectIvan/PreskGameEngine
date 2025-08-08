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
  Path name;
  SPtr<Texture> texture;
};

class PK_CORE_EXPORT TextureManager : public Module<TextureManager>
{
 public:
  TextureManager() = default;
  virtual ~TextureManager() = default;

  /**
   * @brief Load default material textures.
   */
  void
  loadDefaultMatTextures();

  /**
   * @brief Loads a texture from a texture name.
   * @param _directory Where to look for the file.
   * @return Pointer to the newly created texture.
   */
  SPtr<Texture>
  loadTexture(const Path& _directory);

 public:
  Vector<SPtr<TextureMemory>> m_textures;

  // to do: allow for the texture to be generated if not found.
  // default textures
  SPtr<Texture> m_defaultDiff;
  SPtr<Texture> m_defaultNormal;
  SPtr<Texture> m_defaultAO;
  SPtr<Texture> m_defaultRough;
  SPtr<Texture> m_defaultMetallic;
  SPtr<Texture> m_defaultHeight;
};
PK_CORE_EXPORT TextureManager&
g_TextureManager();
}
