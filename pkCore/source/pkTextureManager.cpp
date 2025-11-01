#include "pkAssetResourceManager.h"
#include "pkGPUResourceManager.h"
#include "pkGraphicsAPI.h"
#include "pkTextureCodec.h"
#include "pkTextureManager.h"
#include "pkLogger.h"

namespace pkEngineSDK
{

void
TextureManager::init()
{
  AssetResourceManager& assetMan = g_AssetResourceManager();
  TextureCodec& texCodec = g_TextureCodec();

  SPtr<BaseResource> diffResource = texCodec.createResourceFromFile(Path("textures/default/FlatDiff.png"));
  diffResource->softLoad(Path("resources/FlatDiff.pkt"));
  assetMan.insertNewResource(diffResource);
  SPtr<BaseResource> normalResource = texCodec.createResourceFromFile(Path("textures/default/FlatNormal.png"));
  normalResource->softLoad(Path("resources/FlatNormal.pkt"));
  assetMan.insertNewResource(normalResource);
  SPtr<BaseResource> aoResource = texCodec.createResourceFromFile(Path("textures/default/FlatAO.png"));
  aoResource->softLoad(Path("resources/FlatAO.pkt"));
  assetMan.insertNewResource(aoResource);
  SPtr<BaseResource> roughResource = texCodec.createResourceFromFile(Path("textures/default/FlatRoughness.png"));
  roughResource->softLoad(Path("resources/FlatRoughness.pkt"));
  assetMan.insertNewResource(roughResource);
  SPtr<BaseResource> metallicResource = texCodec.createResourceFromFile(Path("textures/default/FlatMetallic.png"));
  metallicResource->softLoad(Path("resources/FlatMetallic.pkt"));
  assetMan.insertNewResource(metallicResource);
  SPtr<BaseResource> emissiveResource = texCodec.createResourceFromFile(Path("textures/default/FlatEmissive.png"));
  emissiveResource->softLoad(Path("resources/FlatEmissive.pkt"));
  assetMan.insertNewResource(emissiveResource);

  m_diffID = diffResource->m_id;
  m_normalID = normalResource->m_id;
  m_AOID = aoResource->m_id;
  m_roughID = roughResource->m_id;
  m_metallicID = metallicResource->m_id;
  m_emissiveID = emissiveResource->m_id;

  loadDefaultMatTextures();
}

void
TextureManager::loadDefaultMatTextures()
{
  m_defaultDiff = loadTexture(m_diffID);
  m_defaultNormal = loadTexture(m_normalID);
  m_defaultAO = loadTexture(m_AOID);
  m_defaultMetallic = loadTexture(m_metallicID);
  m_defaultRough = loadTexture(m_roughID);
  m_defaultEmissive = loadTexture(m_emissiveID);
}

SPtr<Texture>
TextureManager::loadTexture(const String& _ID)
{
  GraphicsAPI& api = g_GraphicAPI();
  Logger& log = g_Logger();
  // check if the texture has been stored before.
  SPtr<Texture> texture = getTexture(_ID);
  if (texture) {
    return texture;
  }
  // create the texture
  // if it is an exr image / hdr
  uint32 mipLevels = 0;
  uint32 bindFlags = PK_BIND_FLAG::kPK_BIND_SHADER_RESOURCE;
  // mipLevels = 0; // to do: separate mips generation function between skybox and textures.

  // check if the resource is saved in the resource manager. otherwise, resource doesnt exist.
  SPtr<BaseResource> resource = g_AssetResourceManager().getResource(_ID);
  if (!resource) {
    const String msg = "Failed to find a resource with the ID: " + _ID + ".";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }
  resource->load();

  texture = api.createTextureFromResource(resource, bindFlags, mipLevels);

  // if the texture failed to load
  if (!texture) {
    texture = nullptr;
    return nullptr;
  }

  insertLoadedTexture(resource->m_id, texture);

  // return the final texture
  return texture;
}

SPtr<Texture>
TextureManager::getTexture(const String& _ID)
{
  // search if the texture has been stored before
  for (auto& texture : m_textures) {
    if (_ID == texture.first) {
      return texture.second;
    }
  }
  return nullptr;
}

void
TextureManager::insertLoadedTexture(const String& _ID, const SPtr<Texture>& _pTexture)
{
  m_textures.insert({ _ID, _pTexture });
}

PK_CORE_EXPORT TextureManager&
g_TextureManager()
{
  return TextureManager::instance();
}
}