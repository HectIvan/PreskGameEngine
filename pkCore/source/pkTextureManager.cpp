#include "pkAssetResourceManager.h"
#include "pkGPUResourceManager.h"
#include "pkGraphicsAPI.h"
#include "pkTextureCodec.h"
#include "pkTextureManager.h"
#include "pkLogger.h"
#include "pkUUID.h"

namespace pkEngineSDK
{

void
TextureManager::init()
{
  AssetResourceManager& assetMan = g_AssetResourceManager();

  const Vector<String> paths = { "resources/FlatDiff.pkt",
                                 "resources/FlatNormal.pkt",
                                 "resources/FlatAO.pkt",
                                 "resources/FlatRoughness.pkt",
                                 "resources/FlatMetallic.pkt",
                                 "resources/FlatEmissive.pkt" };

  Vector<String> ids;
  ids.resize(paths.size(), UUID::PK_DEFAULT_UUID);

  for (uint32 i = 0; i < paths.size(); ++i) {
    SPtr<BaseResource> resource = make_shared<TextureResource>();
    bool status = resource->softLoad(Path(paths[i]));

    if (status) {
      assetMan.insertNewResource(resource);
      ids[i] = resource->m_id;
    }
  }

  m_diffID = ids[0];
  m_normalID = ids[1];
  m_AOID = ids[2];
  m_roughID = ids[3];
  m_metallicID = ids[4];
  m_emissiveID = ids[5];

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
TextureManager::createEmptyTexture()
{
  GraphicsAPI& api = g_GraphicAPI();
  return api.createEmptyTexture();
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