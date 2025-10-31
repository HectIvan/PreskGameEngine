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
    kTexture
  };
}

struct BaseHeader
{
  String ID;
  SIZE_T IDSize;
  String name;
  SIZE_T nameSize;
  String path;
  SIZE_T pathSize;
};

class PK_CORE_EXPORT BaseResource
{
 public:
  BaseResource() = default;
  virtual ~BaseResource() = default;

  /**
   * @brief Soft load the resource (used by the asset manager to know what resource is what).
   */
  void
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
   * @param _path file path.
   */
  void
  writeBaseHeader(ofstream& _file, const Path& _path);

  /**
   * @brief Load the resource.
   */
  virtual void
  load() = 0;

  /**
   * @brief Get the resource type.
   * @return The resource type.
   */
  virtual RESOURCE_TYPE::E
  getType() const = 0;

 public:
  String m_id;
  String m_name;
  Path m_originalPath;
  Path m_resourcePath;
};
}