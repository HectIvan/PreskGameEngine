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
   * @brief load a pkt Model resource from a given path.
   * @param _path Path to load the resource from.
   * @return Pointer to the resource.
   */
  SPtr<ModelResource>
  loadModelResource(const Path _path);

  /**
   * @brief load a pkt Texture resource from a given path.
   * @param _path Path to load the resource from.
   * @return Pointer to the resource.
   */
  SPtr<TextureResource>
  loadTextureResource(const Path _path);

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

  UMap<uint32, Path>
  getAllResources() {
    return m_allResources;
  }

 private:
  UMap<uint32, Path> m_loadedResources; // SPtr<BaseResource>
  UMap<uint32, Path> m_allResources;    // SPtr<BaseResource>
  Path m_resourcesFolder;
};

PK_CORE_EXPORT AssetResourceManager&
g_AssetResourceManager();
}