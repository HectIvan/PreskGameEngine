#include "pkTextureManager.h"
#include "pkResourceManager.h"
#include "pkGraphicsAPI.h"

namespace pkEngineSDK
{

SPtr<Texture>
TextureManager::loadTexture(const Path& _directory)
{
  GraphicsAPI& api = g_GraphicAPI().instance();
  // search if the texture has been stored before
  for (uint32 i = 0; i < m_textures.size(); ++i) {
    if (m_textures[i]->name.toString() == _directory.toString()) {
      return m_textures[i]->texture;
    }
  }

  SPtr<Texture> texture = nullptr;
  // create the texture
  // if it is an exr image / hdr
  if (_directory.getExtension() == "exr" || _directory.getExtension() == "hdr") {
    texture = api.createTextureFromFileF(_directory, 8, false);
  }
  else {
    texture = api.createTextureFromFile(_directory, 8, false, 28);
  }

  // if the texture failed to load
  if (!texture) {
    texture = nullptr;
    return nullptr;
  }

  // store the new texture in the memory
  SPtr<TextureMemory> newTexture = make_shared<TextureMemory>();
  newTexture->name = _directory;
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