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
**/
/*********************************************/
#include "pkPrerequisitesCore.h"
#include "pkPath.h"
#include "pkUUID.h"

#define PK_RESOURCE_NAME_SIZE 64
#define PK_RESOURCE_PATH_SIZE 256

namespace pkEngineSDK
{

namespace RESOURCE_TYPE
{
  enum E {
    kUnknown = 0,
    kModel,
    kMesh,
    kMaterial,
    kShader,
    kTexture,
    kActor
  };
}

struct BaseHeader
{
  UUID ID;
  ANSICHAR name[PK_RESOURCE_NAME_SIZE];
  ANSICHAR originalPath[PK_RESOURCE_PATH_SIZE];
  ANSICHAR path[PK_RESOURCE_PATH_SIZE];
};

class PK_CORE_EXPORT BaseResource
{
 public:
  BaseResource() = default;
  virtual ~BaseResource() = default;

  /**
   * @brief Load the resource.
   */
  virtual void
  load() = 0;

  /**
   * @brief Unload the resource.
   */
  virtual void
  unload() = 0;

  /**
   * @brief Soft load the resource (used by the asset manager to know what resource is what).
   * @param _path Path to soft load.
   * @return If the soft load was successful.
   */
  bool
  softLoad(const Path& _path);

  /**
   * @brief Fill the base resource header with the necessary data.
   * @param _uuidString String from where the UUID will b generated.
   * @param _name Name of the resource.
   * @param _originalPath Path of the original resource (if one exists).
   * @param _resourcePath Path of the current resource.
   */
  void
  fillBaseHeader(const String& _uuidString,
                 const String& _name,
                 const String& _originalPath,
                 const String& _resourcePath);

  /**
   * @brief load the base resource header from a read stream.
   * @param _file File ifstream.
   */
  void
  loadBaseHeader(ifstream& _file);

  /**
   * @brief Write the base header of the resource.
   * @param _file File in which to write the data.
   */
  void
  writeBaseHeader(ofstream& _file);

  /**
   * @brief Write the base resource header in a write stream.
   * @param _file File ofstream.
   * @param _ID Resource ID.
   * @param _fileName Name of the file.
   * @param _originalPath OriginalPath of the file (if there's one).
   * @param _resourcePath resource path.
   */
  // void
  // writeBaseHeader(ofstream& _file,
  //                 const UUID& _ID,
  //                 const ANSICHAR* _fileName,
  //                 const ANSICHAR* _originalPath,
  //                 const ANSICHAR* _resourcePath);

  /**
   * @brief Get the resource type.
   * @return The resource type.
   */
  virtual RESOURCE_TYPE::E
  getType() const = 0;

  /**
   * @brief Get the resource type as a string.
   * @return The resource type in string.
   */
  virtual String
  getTypeString() const = 0;

 public:
  UUID m_id;
  ANSICHAR m_name[PK_RESOURCE_NAME_SIZE];
  ANSICHAR m_originalPath[PK_RESOURCE_PATH_SIZE];
  ANSICHAR m_resourcePath[PK_RESOURCE_PATH_SIZE];
  bool m_isLoaded = false;
};
}