#include "pkTextureManager.h"
#include "pkGPUResourceManager.h"
#include "pkGraphicsAPI.h"
#include "pkTextureCodec.h"

namespace pkEngineSDK
{

void
TextureManager::init()
{
  TextureCodec& texCodec = g_TextureCodec();
  texCodec.createResourceFromFile(Path("textures/default/FlatDiff.png"));
  texCodec.createResourceFromFile(Path("textures/default/FlatNormal.png"));
  texCodec.createResourceFromFile(Path("textures/default/FlatAO.png"));
  texCodec.createResourceFromFile(Path("textures/default/FlatRoughness.png"));
  texCodec.createResourceFromFile(Path("textures/default/FlatMetallic.png"));
  texCodec.createResourceFromFile(Path("textures/default/FlatEmissive.png"));
  loadDefaultMatTextures();
}

void
TextureManager::loadDefaultMatTextures()
{
  m_defaultDiff = loadTexture(Path("resources/FlatDiff.pkt"));
  m_defaultNormal = loadTexture(Path("resources/FlatNormal.pkt"));
  m_defaultAO = loadTexture(Path("resources/FlatAO.pkt"));
  m_defaultMetallic = loadTexture(Path("resources/FlatMetallic.pkt"));
  m_defaultRough = loadTexture(Path("resources/FlatRoughness.pkt"));
  m_defaultEmissive = loadTexture(Path("resources/FlatEmissive.pkt"));
}

SPtr<Texture>
TextureManager::loadTexture(const Path& _directory)
{
  GraphicsAPI& api = g_GraphicAPI();
  // search if the texture has been stored before
  for (uint32 i = 0; i < m_textures.size(); ++i) {
    if (m_textures[i]->name.toString() == _directory.toString()) {
      return m_textures[i]->texture;
    }
  }

  SPtr<Texture> texture = nullptr;
  // create the texture
  // if it is an exr image / hdr
  uint32 mipLevels = 1;
  if (_directory.getExtension() == "exr" || _directory.getExtension() == "hdr") {
    mipLevels = 0;
    texture = api.createTextureFromFileF(_directory,
                                         PK_BIND_FLAG::kPK_BIND_UNORDERED_ACCESS |
                                         PK_BIND_FLAG::kPK_BIND_SHADER_RESOURCE,
                                         mipLevels);// PK_RESOURCE_MISC_FLAG::kPK_RESOURCE_MISC_GENERATE_MIPS);
  }
  else {
    texture = api.createTextureFromFile(_directory, 
                                        PK_BIND_FLAG::kPK_BIND_SHADER_RESOURCE,
                                        mipLevels,
                                        PK_TEXTURE_FORMAT::kPK_FORMAT_R8G8B8A8_UNORM);
  }

  // if the texture failed to load
  if (!texture) {
    texture = nullptr;
    return nullptr;
  }
  texture->setName(_directory.getFileNameWithoutExtension());

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