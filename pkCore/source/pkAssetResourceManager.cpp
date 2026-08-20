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
#include "pkModelManager.h"
#include "pkLogger.h"
#include "pkMaterialManager.h"
#include "pkMaterialResource.h"
#include "pkModelCodec.h"
#include "pkModelResource.h"
#include "pkPath.h"
#include "pkPrerequisitesCore.h"
#include "pkShaderResource.h"
#include "pkTextureCodec.h"
#include "pkTextureManager.h"
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
    LOG_WARNING("No resource of ID " + _ID.toString() + " found.", __FILE__, __LINE__);
    return nullptr;
  }
  resource->load();
  return resource;
}

SPtr<BaseResource>
AssetResourceManager::unloadResource(const UUID& _ID)
{
  SPtr<BaseResource> resource = getResource(_ID);
  if (!resource) {
    LOG_WARNING("No resource of ID " + _ID.toString() + " found.", __FILE__, __LINE__);
    return nullptr;
  }
  resource->unload();
  return resource;
}

void
AssetResourceManager::loadAssetsFromResourcesFolder()
{
  // clear all previously stored resources.
  m_allResources.clear();

  const Vector<Path> paths = FileSystem::getFilesFromFolder(m_resourcesFolder);
  const uint32 pathCount = toUint32(paths.size());

  // iterate through all found resources.
  for (uint32 i = 0; i < pathCount; ++i) {
    const Path path = paths[i];
    // check if the file is a PK resource.
    if (isPKResource(path)) {
      // soft load the resource and save it in the asset manager.
      const String extension = path.getExtension();
      SPtr<BaseResource> resource;
      if (extension == "pkm") {
        resource = pk_shared_ptr_new<ModelResource>();
      }
      if (extension == "pkt") {
        resource = pk_shared_ptr_new<TextureResource>();
      }
      if (extension == "pkmat") {
        resource = pk_shared_ptr_new<MaterialResource>();
      }
      if (extension == "pks") {
        resource = pk_shared_ptr_new<ShaderResource>();
      }
      resource->softLoad(path);
      insertNewResource(resource);
      // register the action in the log.
      const String msg = "Stored resource " + String(resource->m_resourcePath) + ".";
      LOG_REGISTER(msg, __FILE__, __LINE__);
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

void
AssetResourceManager::removeResourceFromManagers(const UUID& _ID)
{
  SPtr<BaseResource> resource = getResource(_ID);

  if (resource) {
    const RESOURCE_TYPE::E resType = resource->getType();
    if (RESOURCE_TYPE::kModel == resType) {
      g_ModelManager().deleteModel(_ID);
    }
    if (RESOURCE_TYPE::kMaterial == resType) {
      g_MaterialManager().deleteMaterial(_ID);
    }
    if (RESOURCE_TYPE::kTexture == resType) {
      g_TextureManager().deleteTexture(_ID);
    }
  }
}

void
AssetResourceManager::deleteResource(const UUID& _ID)
{
  removeResourceFromManagers(_ID);
  m_loadedResources.erase(_ID);
  m_allResources.erase(_ID);
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
AssetResourceManager::getResourceByDirectory(const String& _directory)
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