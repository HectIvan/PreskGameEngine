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
  TextureManager& texMan = g_TextureManager();

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

  auto flatTex = createTexture("FlatRed", 1, 1, 4, Color::RED);
  texMan.insertTexture(flatTex->getID(), flatTex);
}

void
TextureManager::onShutDown()
{
  LOG_REGISTER("------------------Shutting down Texture Manager.", __FILE__, __LINE__);
  String msg = "Unloading " + to_string(m_textures.size()) + " textures.";
  LOG_REGISTER(msg, __FILE__, __LINE__);
  m_defaultAlb.reset();
  m_defaultNormal.reset();
  m_defaultAO.reset();
  m_defaultRough.reset();
  m_defaultMetallic.reset();
  m_defaultEmissive.reset();
  m_defaultHeight.reset();

  const uint32 textureCount = toUint32(m_textures.size());
  for (uint32 i = 0; i < textureCount; ++i) {
    auto it = m_textures.begin();
    const int32 count = it->second.use_count();
    const String name = it->second->getName();
    it->second.reset();

    if (count - 1 > 0) {
      const String msg = "Texture " + name + " has " + to_string(count - 1) + " references.";
      LOG_REGISTER(msg, __FILE__, __LINE__);
    }

    m_textures.erase(it);
  }
  m_textures.clear();
  msg = "------------------Texture Manager shut down. Remaining textures: " +
        to_string(m_textures.size());
  LOG_REGISTER(msg, __FILE__, __LINE__);
}

void
TextureManager::loadDefaultMatTextures()
{
  m_defaultAlb = createTexture(m_albID);
  m_defaultNormal = createTexture(m_normalID);
  m_defaultAO = createTexture(m_AOID);
  m_defaultMetallic = createTexture(m_metallicID);
  m_defaultRough = createTexture(m_roughID);
  m_defaultEmissive = createTexture(m_emissiveID);
}

SPtr<Texture>
TextureManager::createEmptyTexture()
{
  return g_GraphicAPI().createEmptyTexture();
}

SPtr<Texture>
TextureManager::createTexture(const String& _name,
                              const int32& _width,
                              const int32& _height,
                              const uint32& _bpp,
                              const Color& _color)
{
  GraphicsAPI& api = g_GraphicAPI();
  TextureCodec& texCodec = g_TextureCodec();
  
  Vector<byte> data;
  PK_TEXTURE_FORMAT::E format = PK_TEXTURE_FORMAT::kPK_FORMAT_UNKNOWN;

  // check for bpp and fill the data vector accordingly.
  if (_bpp == 3) {
    data = { _color._color.R, _color._color.G, _color._color.B};
    format = PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32_FLOAT;
  }
  if (_bpp == 4) {
    data = { _color._color.R, _color._color.G, _color._color.B, 255 };
    format = PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_FLOAT;
  }

  // create and store the resource in the resource manager.
  auto resource = texCodec.createResource(_name, _width, _height, _bpp, format, 1, data);
  g_AssetResourceManager().insertNewResource(resource);

  // create the final texture and return it.
  SPtr<Texture> texture = api.createTexture(resource, PK_BIND_FLAG::kPK_BIND_SHADER_RESOURCE);
  
  return texture;
}

SPtr<Texture>
TextureManager::createTexture(const Path& _path)
{
  const String pathStr = PK_RESOURCE_FOLDER + _path.getFileNameWithoutExtension() + ".pkt";
  const UUID id = UUID::generateRandomUUIDFromString(pathStr);
  return createTexture(id);
}

SPtr<Texture>
TextureManager::createTexture(const UUID& _ID)
{
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

  texture = g_GraphicAPI().createTexture(resource, bindFlags);

  // if the texture failed to load
  if (!texture) {
    texture = nullptr;
    return nullptr;
  }
  texture->setID(resource->m_id);

  const Path resPath = Path(resource->m_resourcePath);
  insertTexture(resource->m_id, texture);

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
  const UUID id = UUID::generateRandomUUIDFromString(_path);
  auto it = m_textures.find(id);
  if (it != m_textures.end()) {
    return it->second;
  }
  return nullptr;
}

void
TextureManager::deleteTexture(const UUID& _ID)
{
  SPtr<Texture> texture = getTexture(_ID);
  if (texture) {
    m_textures.erase(_ID);
  }
}

void
TextureManager::insertTexture(const UUID& _ID, const SPtr<Texture>& _pTexture)
{
  m_textures.insert({ _ID, _pTexture });
}

PK_CORE_EXPORT TextureManager&
g_TextureManager()
{
  return TextureManager::instance();
}
}