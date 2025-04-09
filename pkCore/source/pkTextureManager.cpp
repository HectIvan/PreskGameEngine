#include "pkTextureManager.h"
#include "pkResourceManager.h"

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
  SPtr<Texture> texture = g_ResourceManager().newTexture(_name, _directory);

  // store the new texture in the memory
  SPtr<TextureMemory> newTexture = make_shared<TextureMemory>();
  newTexture->name = _name;
  newTexture->texture = texture;
  m_textures.push_back(newTexture);

  // return the final texture
  return texture;
}
PK_CORE_EXPORT TextureManager&
g_TextureManager()
{
  return TextureManager::instance();
}
}