/*****************************************************************************/
/**
 * @file    pkAssetResourceManager.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    18/09/2025
 * @brief   This file will contain the Asset Resource Manager used for the engine
 *
 * @bug    No known bugs.
 *
 * @changes Created the file.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
 * Includes
 */
 /*********************************************/
#include "pkModule.h"
#include "pkBaseResource.h"

namespace pkEngineSDK
{

class Path;
class TextureResource;
class ModelResource;
class MaterialResource;

// to do: placeholder for the id.
namespace ASSET_ID
{
  enum E
  {
    kUnknown = 0
  };
}

class PK_CORE_EXPORT AssetResourceManager : public Module<AssetResourceManager>
{
 public:
  AssetResourceManager() = default;
  virtual ~AssetResourceManager() = default;
  
  /**
   * @brief Initialize the asset resource manager.
   */
  void
  init(const String _resourcesPath);

  /**
   * @brief Load a resource from a file.
   */
  SPtr<BaseResource>
  loadResource();

  /**
   * @brief Save a resource to a file.
   */
  SPtr<BaseResource>
  unloadResource();

  /**
   * @brief Load a pkmat Material resource from a given path.
   * @param _path Path to load the resource from.
   * @return Pointer to the resource.
   */
  SPtr<MaterialResource>
  loadMaterialResource(const Path _path);

  /**
   * @brief Get assets from the set resources folder.
   */
  void
  loadAssetsFromResourcesFolder();

  /**
   * @brief Check if a resource.
   * @param _path Resource path.
   * @return If the file is a PK resource.
   */
  bool
  isPKResource(const Path _path);

  /**
   * @brief Get all resources in the manager.
   * @return The map with all resources
   */
  UMap<String, SPtr<BaseResource>>
  getAllResources() {
    return m_allResources;
  }

  /**
   * @brief Insert a resource into the resources map (not the loaded resources).
   * @param _UUID resource ID.
   * @param _pResource Asset Resource to insert.
   */
  void
  insertNewResource(SPtr<BaseResource> _pResource)
  {
    m_allResources.insert({_pResource->m_id, _pResource});
  }

  /**
   * @brief Insert a resource into the loaded resources map.
   * @param _UUID resource ID.
   * @param _pResource Loaded Asset Resource to insert.
   */
  void
  insertLoadedResource(SPtr<BaseResource> _pResource)
  {
    m_loadedResources.insert({ _pResource->m_id, _pResource});
  }

  /**
   * @brief Get all resources in the manager.
   * @return The map with all loaded resources
   */
  UMap<String, SPtr<BaseResource>>
  getAllLoadedResources() {
    return m_loadedResources;
  }

  /**
   * @brief Get a resource via its ID.
   * @param _ID ID to look for.
   * @return Pointer to the resource.
   */
  SPtr<BaseResource>
  getResource(const String&_ID);

 private:
  UMap<String, SPtr<BaseResource>> m_loadedResources;
  UMap<String, SPtr<BaseResource>> m_allResources;   
  Path m_resourcesFolder;
};

PK_CORE_EXPORT AssetResourceManager&
g_AssetResourceManager();
}