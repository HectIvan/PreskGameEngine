#include "pkAssetResourceManager.h"
#include "pkModelManager.h"
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
                                 "resources/FlatWhite.pkt",
                                 "resources/FlatBlack.pkt" };

  Vector<UUID> ids;
  ids.resize(paths.size(), UUID::PK_DEFAULT_UUID);

  for (uint32 i = 0; i < paths.size(); ++i) {
    SPtr<BaseResource> resource = make_shared<TextureResource>();
    bool status = resource->softLoad(Path(paths[i]));

    if (status) {
      assetMan.insertNewResource(resource);
      ids[i] = resource->m_id;
    }
  }

  m_albID = ids[0];
  m_normalID = ids[1];
  m_AOID = ids[2];
  m_roughID = ids[2];
  m_metallicID = ids[2];
  m_emissiveID = ids[3];

  loadDefaultMatTextures();

  // createFlatTexture("FlatRed", 1, 1, Color::RED);
}

void
TextureManager::loadDefaultMatTextures()
{
  m_defaultAlb = loadTexture(m_albID);
  m_defaultNormal = loadTexture(m_normalID);
  m_defaultAO = loadTexture(m_AOID);
  m_defaultMetallic = loadTexture(m_metallicID);
  m_defaultRough = loadTexture(m_roughID);
  m_defaultEmissive = loadTexture(m_emissiveID);
}

SPtr<Texture>
TextureManager::createEmptyTexture()
{
  return g_GraphicAPI().createEmptyTexture();
}

SPtr<Texture>
TextureManager::createFlatTexture(const String _name,
                                  const int32 _width,
                                  const int32 _height,
                                  const Color color)
{
  GraphicsAPI& api = g_GraphicAPI();
  TextureCodec& texCodec = g_TextureCodec();
  AssetResourceManager& assetMan = g_AssetResourceManager();
  Vector<uint8> data = { color.getR(), color.getG(), color.getB(), color.getA() };
  SPtr<TextureResource> resource = texCodec.createResource(_name,
                                                           _width,
                                                           _height,
                                                           4,
                                                           PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_FLOAT,
                                                           1,
                                                           data);
  assetMan.insertNewResource(resource);

  SPtr<Texture> texture = api.createTextureFromResource(resource,
                                                        PK_BIND_FLAG::kPK_BIND_SHADER_RESOURCE);

  return texture;
}

SPtr<Texture>
TextureManager::loadTexture(const UUID& _ID)
{
  GraphicsAPI& api = g_GraphicAPI();
  // check if the texture has been stored before.
  SPtr<Texture> texture = getTexture(_ID);
  if (texture) {
    return texture;
  }
  // create the texture
  uint32 bindFlags = PK_BIND_FLAG::kPK_BIND_SHADER_RESOURCE;

  // check if the resource is saved in the resource manager. otherwise, resource doesnt exist.
  SPtr<BaseResource> resource = g_AssetResourceManager().getResource(_ID);
  if (!resource) {
    const String msg = "Failed to find a resource with the ID: " + _ID.toString() + ".";
    LOG_WARNING(msg, __FILE__, __LINE__);
    return nullptr;
  }
  
  resource->load();

  texture = api.createTextureFromResource(resource, bindFlags);

  // if the texture failed to load
  if (!texture) {
    texture = nullptr;
    return nullptr;
  }
  texture->setID(resource->m_id);

  const Path resPath = Path(resource->m_resourcePath);
  insertTexture(resource->m_id, resPath, texture);

  // return the final texture
  return texture;
}

SPtr<Texture>
TextureManager::getTexture(const UUID& _ID)
{
  // search if the texture has been stored before
  auto it = m_textures.find(_ID);
  if (it != m_textures.end()) {
    return it->second;
  }
  return nullptr;
}

SPtr<Texture>
TextureManager::getTexture(const String& _path)
{
  // search if the texture has been stored before
  auto it = m_texturesPath.find(_path);
  if (it != m_texturesPath.end()) {
    return it->second;
  }
  return nullptr;
}

PKFORCEINLINE void
TextureManager::deleteTexture(const UUID& _ID)
{
  SPtr<Texture> texture = getTexture(_ID);
  if (texture) {
    m_textures.erase(_ID);
    // also erase from path map
    for (auto it = m_texturesPath.begin(); it != m_texturesPath.end(); ++it) {
      if (it->second == texture) {
        m_texturesPath.erase(it);
        break;
      }
    }
  }
}

void
TextureManager::insertTexture(const UUID& _ID, const Path& _path, const SPtr<Texture>& _pTexture)
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