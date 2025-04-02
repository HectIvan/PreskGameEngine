/*****************************************************************************/
/**
 * @file    pkTextureManager.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    01/04/2025
 * @brief   Texture manager used for handling texture management and creation.
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
#include "pkModule.h"
#include "pkTexture.h"

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
   * @brief Create a texture from a file name.
   * @param _name Name of the texture (extension type required).
   * @param _directory Where is the texture located (textures/ set as default).
   * @return Pointer to the texture.
   */
  SPtr<Texture>
  createTexture(String _name, String _directory = "textures/");

 private:
  /**
   * @brief Create a texture from file.
   * @param _name Name of the texture (extension type required).
   * @param _directory Where is the texture located (textures/ set as default).
   * @return Pointer to the texture.
   */
  SPtr<Texture>
  createTextureFromFile(String _name, String _directory = "textures/");

 public:
  Vector<SPtr<TextureMemory>> m_textures;
};
PK_CORE_EXPORT TextureManager&
g_TextureManager();
}