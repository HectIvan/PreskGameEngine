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
**/
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
  SIZE_T nameSize;
  String name;
  uint32 vertexCount;
  uint32 indexCount;
};

class PK_CORE_EXPORT ModelResource : public BaseResource
{
 public:
  ModelResource() = default;
  virtual ~ModelResource() = default;

  /**
   * @brief Get the resource type.
   * @return The resource type.
   */
  RESOURCE_TYPE::E
  getType() const override { return RESOURCE_TYPE::kModel; }

 public:
  Vector<SimpleVertex> m_vertex;
  Vector<uint32> m_index;
  Vector<SPtr<Mesh>> m_meshes;
};
}
