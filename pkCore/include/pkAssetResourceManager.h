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
#include "pkModule.h"
#include "pkBaseResource.h"

#pragma once

namespace pkEngineSDK
{

class Path;
class TextureResource;
class ModelResource;

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
  init();

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
  ModelResource*
  loadModelResource(const Path _path);

  /**
   * @brief load a pkt Texture resource from a given path.
   * @param _path Path to load the resource from.
   * @return Pointer to the resource.
   */
  TextureResource*
  loadTextureResource(const Path _path);

 private:
  UMap<ASSET_ID::E, SPtr<BaseResource>> m_loadedResources;
  UMap<ASSET_ID::E, SPtr<BaseResource>> m_allResources;
};

PK_CORE_EXPORT AssetResourceManager&
g_AssetResourceManager();
}