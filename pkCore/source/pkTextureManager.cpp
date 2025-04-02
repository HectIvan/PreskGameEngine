/*****************************************************************************/
/**
 * @file    pkTextureManager.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    01/04/2025
 * @brief   Texture manager used for handling texture management and creation.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkTextureManager.h"
#include "pkGraphicsAPI.h"

namespace pkEngineSDK
{

SPtr<Texture>
TextureManager::createTexture(String _name, String _directory)
{
  // search if the texture has been stored before
  for (uint32 i = 0; i < m_textures.size(); ++i) {
    if (m_textures[i]->name == _name) {
      return m_textures[i]->texture;
    }
  }

  // create the texture
  SPtr<Texture> texture = createTextureFromFile(_name, _directory);

  // store the new texture in the memory
  SPtr<TextureMemory> newTexture = make_shared<TextureMemory>();
  newTexture->name = _name;
  newTexture->texture = texture;
  m_textures.push_back(newTexture);

  // return the final texture
  return texture;
}

SPtr<Texture>
TextureManager::createTextureFromFile(String _name, String _directory)
{
  // create the texture adress.
  String textureName = _directory + _name;
  return g_GraphicAPI().createTextureFromFile(textureName, 8, false, 28);
}

PK_CORE_EXPORT TextureManager&
g_TextureManager()
{
  return TextureManager::instance();
}
}