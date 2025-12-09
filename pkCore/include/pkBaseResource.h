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

#define PK_RESOURCE_ID_SIZE 36
#define PK_RESOURCE_NAME_SIZE 64
#define PK_RESOURCE_ORIGINAL_PATH_SIZE 256
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
  ANSICHAR originalPath[PK_RESOURCE_ORIGINAL_PATH_SIZE];
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
   * @brief Soft load the resource (used by the asset manager to know what resource is what).
   * @param _path Path to soft load.
   * @return If the soft load was successful.
   */
  bool
  softLoad(const Path& _path);

  /**
   * @brief load the base resource header from a read stream.
   * @param _file File ifstream.
   * @return The base resource header.
   */
  BaseHeader
  loadBaseHeader(ifstream& _file);

  /**
   * @brief Write the base resource header in a write stream.
   * @param _file File ofstream.
   * @param _ID Resource ID.
   * @param _fileName Name of the file.
   * @param _resourcePath resource path.
   */
  void
  writeBaseHeader(ofstream& _file,
                  const String& _ID,
                  const String& _fileName,
                  const String& _resourcePath);

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
  bool m_isLoaded = false;
  ANSICHAR m_originalPath[PK_RESOURCE_ORIGINAL_PATH_SIZE];
  ANSICHAR m_resourcePath[PK_RESOURCE_PATH_SIZE];
};
}