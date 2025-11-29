/*****************************************************************************/
/**
 * @file    pkTextureManager.h
 * @author  Héctor Iván Muñoz Ceballos
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

class PK_CORE_EXPORT TextureManager : public Module<TextureManager>
{
 public:
  TextureManager() = default;
  virtual ~TextureManager() = default;

  /**
   * @brief initialize the texture manager by loading default textures.
   */
  void
  init();

  /**
   * @brief Load default material textures.
   */
  void
  loadDefaultMatTextures();

  /**
   * @brief Create an empty texture.
   * @return Pointer to the empty texture.
   */
  SPtr<Texture>
  createEmptyTexture();

  // /**
  //  * @brief on shut down of the module.
  //  */
  // virtual void
  // onShutDown();

  /**
   * @brief Create a flat color texture.
   * @param _name Name of the texture.
   * @param _width Width of the texture.
   * @param _height Height of the texture.
   * @param color Color to fill the texture with.
   * @return Pointer to the newly created texture.
   */
  SPtr<Texture>
  createFlatTexture(const String _name,
                    const int32 _width,
                    const int32 _height,
                    const Color color);

  /**
   * @brief Load texture from a texture id.
   * @param _ID ID of the texture to load.
   * @return Pointer to the newly created texture.
   */
  SPtr<Texture>
  loadTexture(const String& _ID);

  /**
   * @brief Get a saved texture via its ID.
   * @param _ID ID to look for.
   * @return Pointer to the saved texture.
   */
  SPtr<Texture>
  getTexture(const String& _ID);

  /**
   * @brief Get a saved texture via its resource path.
   * @param _path Path of the texture resource.
   * @return Pointer to the saved texture.
   */
  SPtr<Texture>
  getTextureFromPath(const String& _path);
  
  /**
   * @brief Store a loaded texture into the memory for later use.
   * @param _ID ID of the texture.
   * @param _path Path of the texture resource.
   * @param _pTexture Texture to store.
   */
  void
  insertLoadedTexture(const String& _ID, const Path& _path, const SPtr<Texture>& _pTexture);

 public:
  // save textures with the texture id
  UMap<String, SPtr<Texture>> m_textures;
  // save textures with the texture path
  UMap<String, SPtr<Texture>> m_texturesPath;

  // to do: allow for the texture to be generated if not found.
  // default textures
  SPtr<Texture> m_defaultAlb;
  SPtr<Texture> m_defaultNormal;
  SPtr<Texture> m_defaultAO;
  SPtr<Texture> m_defaultRough;
  SPtr<Texture> m_defaultMetallic;
  SPtr<Texture> m_defaultHeight;
  SPtr<Texture> m_defaultEmissive;

  String m_albID;
  String m_normalID;
  String m_AOID;
  String m_roughID;
  String m_metallicID;
  String m_heightID;
  String m_emissiveID;
};
PK_CORE_EXPORT TextureManager&
g_TextureManager();
}
