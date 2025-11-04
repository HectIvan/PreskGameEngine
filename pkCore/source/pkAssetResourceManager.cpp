/*****************************************************************************/
/**
 * @file    pkAssetResourceManager.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    18/09/2025
 * @brief   Definitions for the functions of the Asset Resource Manager used for the engine.
 *
 * @bug    No known bugs.
 *
 * @changes Created the file.
 */
 /*****************************************************************************/
#include "pkAssetResourceManager.h"
#include "pkGPUResourceManager.h"
#include "pkLogger.h"
#include "pkMaterialResource.h"
#include "pkModelCodec.h"
#include "pkModelResource.h"
#include "pkPath.h"
#include "pkPrerequisitesCore.h"
#include "pkTextureCodec.h"
#include "pkTextureResource.h"
#include "pkFileSystem.h"
#include "pkUUID.h"

namespace pkEngineSDK
{
void
AssetResourceManager::init(const String _resourcesPath)
{
  m_resourcesFolder = Path(_resourcesPath);
}

SPtr<BaseResource>
AssetResourceManager::loadResource(const String& _ID)
{
  // check if the file is stored in the map
  SPtr<BaseResource> resource = getResource(_ID);
  if (!resource) {
    return nullptr;
  }
  resource->load();
  return resource;
}

SPtr<BaseResource>
AssetResourceManager::unloadResource()
{
  return SPtr<BaseResource>();
}

void
AssetResourceManager::loadAssetsFromResourcesFolder ()
{
  m_allResources.clear();
  Vector<Path> paths = FileSystem::getFilesFromFolder(m_resourcesFolder);
  uint32 pathCount = static_cast<uint32>(paths.size());

  // iterate through all found resources.
  for (uint32 i = 0; i < pathCount; ++i) {
    const Path path = paths[i];
    // check if the file is a PK resource.
    if (isPKResource(path)) {
      // soft load the resource and save it in the asset manager.
      const String extension = path.getExtension();
      if (extension == "pkm") {
        SPtr<ModelResource> modelRes = make_shared<ModelResource>();
        modelRes->softLoad(path);
        insertNewResource(modelRes);
      }
      if (extension == "pkt") {
        SPtr<TextureResource> textureRes = make_shared<TextureResource>();
        textureRes->softLoad(path);
        insertNewResource(textureRes);
      }
    }
  }
}

bool
AssetResourceManager::isPKResource(const Path _path)
{
  String extension = _path.getExtension();
  if (extension == "pkm" || extension == "pkt" || extension == "pkmat") {
    return true;
  }
  return false;
}

SPtr<BaseResource>
AssetResourceManager::getResource(const String& _ID)
{
  auto it = m_allResources.find(_ID);
  if (it != m_allResources.end()) {
    return it->second;
  }
  return nullptr;
}

PK_CORE_EXPORT AssetResourceManager&
g_AssetResourceManager()
{
  return AssetResourceManager::instance();
}
}