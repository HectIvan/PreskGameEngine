/*****************************************************************************/
/**
 * @file    pkModelResource.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    04/02/2025
 * @brief   Resource for engine models.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
 * Includes
 */
/*********************************************/
#include "pkBaseResource.h"
#include "pkMesh.h"

namespace pkEngineSDK
{

struct ModelAssetHeader
{
  uint32 meshCount;
};

struct MeshAssetHeader
{
  char name[PK_RESOURCE_NAME_SIZE];
  uint32 vertexCount;
  uint32 indexCount;
};

class PK_CORE_EXPORT ModelResource : public BaseResource
{
 public:
  ModelResource() = default;
  virtual ~ModelResource() = default;

  /**
   * @brief Load the model resource.
   */
  void
  load() override;

  /**
   * @brief Unload the model resource.
   */
  void
  unload() override;

  /**
   * @brief Get the resource type.
   * @return The resource type.
   */
  RESOURCE_TYPE::E
  getType() const override { return RESOURCE_TYPE::kModel; }

  /**
   * @brief Get the resource type as a string.
   * @return The resource type string.
   */
  String
  getTypeString() const override { return "Model"; }

  /**
   * @brief Generate an id from the model directory.
   */
  PK_NODISCARD static UUID
  generateID(const ANSICHAR* _modelDir) {
    return UUID::generateRandomUUIDFromString(String(_modelDir) + "Model");
  }

  /**
   * @brief Generate an id from the model directory.
   */
  PK_NODISCARD static UUID
  generateID(const String& _modelDir) {
    return UUID::generateRandomUUIDFromString(_modelDir + "Model");
  }

  /**
   * @brief Generate an id from the model directory.
   */
  PK_NODISCARD static UUID
  generateID(const Path& _modelDir) {
    return UUID::generateRandomUUIDFromString(_modelDir.toString() + "Model");
  }

 public:
  Vector<SimpleVertex> m_vertex;
  Vector<uint32> m_index;
  Vector<SPtr<Mesh>> m_meshes;
};
}
