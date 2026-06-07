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
   * @brief on shut down of the module.
   */
  void
  onShutDown() override;

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
   * @brief create texture from a texture/resource path.
   * @param _path Path of the texture/resource.
   * @return Pointer to the newly created texture.
   */
  SPtr<Texture>
  createTexture(const Path& _path);

  /**
   * @brief create texture from a resource id.
   * @param _ID ID of the texture to load.
   * @return Pointer to the newly created texture.
   */
  SPtr<Texture>
  createTexture(const UUID& _ID);

  /**
   * @brief Get a saved texture via its ID.
   * @param _ID ID to look for.
   * @return Pointer to the saved texture.
   */
  SPtr<Texture>
  getTexture(const UUID& _ID);

  /**
   * @brief Get a saved texture via its resource path.
   * @param _path Path of the texture resource.
   * @return Pointer to the saved texture.
   */
  SPtr<Texture>
  getTexture(const String& _path);

  /**
   * @brief Delete a texture from the texture manager.
   * @param _ID ID of the texture.
   */
  PKFORCEINLINE void
  deleteTexture(const UUID& _ID);
  
  /**
   * @brief Store a loaded texture into the memory for later use.
   * @param _ID ID of the texture.
   * @param _pTexture Texture to store.
   */
  void
  insertTexture(const UUID& _ID, const SPtr<Texture>& _pTexture);

 public:
  // save textures with the texture id
  UMap<UUID, SPtr<Texture>> m_textures;

  // to do: allow for the texture to be generated if not found.
  // default textures
  SPtr<Texture> m_defaultAlb;
  SPtr<Texture> m_defaultNormal;
  SPtr<Texture> m_defaultAO;
  SPtr<Texture> m_defaultRough;
  SPtr<Texture> m_defaultMetallic;
  SPtr<Texture> m_defaultHeight;
  SPtr<Texture> m_defaultEmissive;

  UUID m_albID;
  UUID m_normalID;
  UUID m_AOID;
  UUID m_roughID;
  UUID m_metallicID;
  UUID m_heightID;
  UUID m_emissiveID;
};
PK_CORE_EXPORT TextureManager&
g_TextureManager();
}
