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
#include "pkModelResource.h"
#include "pkPath.h"
#include "pkPrerequisitesCore.h"
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
AssetResourceManager::loadResource()
{
  return SPtr<BaseResource>();
}

SPtr<BaseResource>
AssetResourceManager::unloadResource()
{
  return SPtr<BaseResource>();
}

SPtr<MaterialResource>
AssetResourceManager::loadMaterialResource(const Path _path)
{
  Logger& log = g_Logger();
  ifstream file(_path.toString(), ios::in | ios::binary);
  // if the direcory cannot be opened, return a warning and a nullptr.
  if (!file.is_open()) {
    String msg = "Failed to open material resource at directory " + _path.toString() + ".";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }

  SPtr<MaterialResource> matResource = make_shared<MaterialResource>();
  MaterialAssetHeader matHeader;

  // read diffuse data
  file.read(reinterpret_cast<char*>(&matHeader.diffusePathSize), sizeof(SIZE_T));
  matResource->m_diffusePath.resize(matHeader.diffusePathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_diffusePath[0]),
                                    matHeader.diffusePathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_diffuseColor), sizeof(Color));

  // read normal data
  file.read(reinterpret_cast<char*>(&matHeader.normalPathSize), sizeof(SIZE_T));
  matResource->m_normalPath.resize(matHeader.normalPathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_normalPath[0]), matHeader.normalPathSize);

  // read ao data
  file.read(reinterpret_cast<char*>(&matHeader.aoPathSize), sizeof(SIZE_T));
  matResource->m_aoPath.resize(matHeader.aoPathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_aoPath[0]), matHeader.aoPathSize);

  // read roughness data
  file.read(reinterpret_cast<char*>(&matHeader.roughnessPathSize), sizeof(SIZE_T));
  matResource->m_roughnessPath.resize(matHeader.roughnessPathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_roughnessPath[0]),
                                    matHeader.roughnessPathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_roughValue), sizeof(float));

  // read metallic data
  file.read(reinterpret_cast<char*>(&matHeader.metallicPathSize), sizeof(SIZE_T));
  matResource->m_metallicPath.resize(matHeader.metallicPathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_metallicPath[0]),
                                    matHeader.metallicPathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_metallicValue), sizeof(float));

  // read emissive data
  file.read(reinterpret_cast<char*>(&matHeader.emissivePathSize), sizeof(SIZE_T));
  matResource->m_emissivePath.resize(matHeader.emissivePathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_emissivePath[0]),
                                    matHeader.emissivePathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_emissiveColor), sizeof(Color));

  return matResource;
}

void
AssetResourceManager::loadAssetsFromResourcesFolder ()
{
  m_allResources.clear();
  Vector<Path> paths = FileSystem::getFilesFromFolder(m_resourcesFolder);
  uint32 pathCount = static_cast<uint32>(paths.size());

  // iterate through all found resources.
  for (uint32 i = 0; i < pathCount; ++i) {
    Path path = paths[i];
    // check if the file is a PK resource.
    if (isPKResource(path)) {
      // soft load the resource and save it in the asset manager.
      if (path.getDirectory() == "pkm") {
        SPtr<ModelResource> modelRes = make_shared<ModelResource>();
        modelRes->softLoad(path);
        insertNewResource(modelRes);
      }
      if (path.getDirectory() == "pkt") {
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
  for (auto& resource : m_allResources) {
    if (_ID == resource.first) {
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