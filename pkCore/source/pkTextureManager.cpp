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

  SPtr<BaseResource> diffResource = make_shared<TextureResource>();
  diffResource->softLoad(Path("resources/FlatDiff.pkt"));
  assetMan.insertNewResource(diffResource);
  SPtr<BaseResource> normalResource = make_shared<TextureResource>();
  normalResource->softLoad(Path("resources/FlatNormal.pkt"));
  assetMan.insertNewResource(normalResource);
  SPtr<BaseResource> aoResource = make_shared<TextureResource>();
  aoResource->softLoad(Path("resources/FlatAO.pkt"));
  assetMan.insertNewResource(aoResource);
  SPtr<BaseResource> roughResource = make_shared<TextureResource>();
  roughResource->softLoad(Path("resources/FlatRoughness.pkt"));
  assetMan.insertNewResource(roughResource);
  SPtr<BaseResource> metallicResource = make_shared<TextureResource>();
  metallicResource->softLoad(Path("resources/FlatMetallic.pkt"));
  assetMan.insertNewResource(metallicResource);
  SPtr<BaseResource> emissiveResource = make_shared<TextureResource>();
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
  if (!resource->m_isLoaded) {
    resource->load();
  }

  texture = api.createTextureFromResource(resource, bindFlags, mipLevels);

  // if the texture failed to load
  if (!texture) {
    texture = nullptr;
    return nullptr;
  }
  texture->setID(resource->m_id);

  insertLoadedTexture(resource->m_id, resource->m_resourcePath, texture);

  // return the final texture
  return texture;
}

SPtr<Texture>
TextureManager::getTexture(const String& _ID)
{
  // search if the texture has been stored before
  auto it = m_textures.find(_ID);
  if (it != m_textures.end()) {
    return it->second;
  }
  return nullptr;
}

SPtr<Texture>
TextureManager::getTextureFromPath(const String& _path)
{
  // search if the texture has been stored before
  auto it = m_texturesPath.find(_path);
  if (it != m_texturesPath.end()) {
    return it->second;
  }
  return nullptr;
}

void
TextureManager::insertLoadedTexture(const String& _ID, const Path& _path, const SPtr<Texture>& _pTexture)
{
  m_textures.insert({ _ID, _pTexture });
  m_texturesPath.insert({ _path.toString(), _pTexture });
}

PK_CORE_EXPORT TextureManager&
g_TextureManager()
{
  return TextureManager::instance();
}
}