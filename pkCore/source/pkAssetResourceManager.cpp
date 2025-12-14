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
#include "pkShaderResource.h"
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
AssetResourceManager::loadResource(const UUID& _ID)
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
      SPtr<BaseResource> resource;
      if (extension == "pkm") {
        resource = make_shared<ModelResource>();
      }
      if (extension == "pkt") {
        resource = make_shared<TextureResource>();
      }
      if (extension == "pkmat") {
        resource = make_shared<MaterialResource>();
      }
      if (extension == "pks") {
        resource = make_shared<ShaderResource>();
      }
      resource->softLoad(path);
      insertNewResource(resource);
      // register the action in the log.
      const String msg = "Stored resource " + String(resource->m_resourcePath) + ".";
      g_Logger().registerMessage(msg, __FILE__, __LINE__);
    }
  }
}

bool
AssetResourceManager::isPKResource(const Path _path)
{
  String extension = _path.getExtension();
  if (extension == "pkm" || extension == "pkt" || extension == "pkmat" || extension == "pks") {
    return true;
  }
  return false;
}

SPtr<BaseResource>
AssetResourceManager::getResource(const UUID& _ID)
{
  auto it = m_allResources.find(_ID);
  if (it != m_allResources.end()) {
    return it->second;
  }
  return nullptr;
}

SPtr<BaseResource>
AssetResourceManager::getResourceBydirectory(const String& _directory)
{
  for (auto resource : m_allResources) {
    if (resource.second->m_resourcePath == _directory) {
      return resource.second;
    }
  }
  return nullptr;
}

PK_CORE_EXPORT AssetResourceManager&
g_AssetResourceManager()
{
  return AssetResourceManager::instance();
}
}