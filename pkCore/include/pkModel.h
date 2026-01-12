/*****************************************************************************/
/**
 * @file    pkModel.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    20/11/2024
 * @brief   Model file for the Presk Game Engine.
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
#include "pkBone.h"
#include "pkComponent.h"
#include "pkIndexBuffer.h"
#include "pkMesh.h"
#include "pkPath.h"
#include "pkVertexBuffer.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT Model : public Component
{
 public:
  Model();
  virtual ~Model() {
    clean();
  }

  /**
   * @brief Update the model component.
   * @param _owner Actor that owns the component.
   */
  void
  update(Actor& _owner) override;

  /**
   * @brief Get the map of bones assigned to the model.
   * @return The bone map.
   */
  auto&
  getBoneInfoMap() { return boneMap; }

  /**
   * @brief Get the ammount of bones in the model.
   * @return The bone count.
   */
  uint32
  getBoneCount() { return boneCounter; }

  /**
   * @brief Set the data of the vertex to a default value
   * @param _vertex Vertex to set to default.
   */
  // void
  // setVertexBoneDataToDefault(SimpleVertex& _vertex)
  // {
  //   for (int i = 0; i < MAX_BONE_WEIGHT; i++) {
  //     _vertex.boneIDs[i] = -1;
  //     _vertex.weights[i] = 0.0f;
  //   }
  // }

  /**
   * @brief Assign bone data to a vertex.
   * @param _vertex Vertex to modify.
   * @param _boneID What bone id it will have.
   * @param _weight How much influence will the bone have.
   */
  // void
  // setVertexBoneData(SimpleVertex& _vertex, int _boneId, float _weight);

  /**
   * @brief Get the component type of this component.
   * @return The component type.
   */
  COMPONENT_TYPE::E
  getType() override { return COMPONENT_TYPE::kModel; }

  /**
   * @brief Set the name of the model.
   * @param _name Name of the model.
   */
  void
  setName(const ANSICHAR* _name) { m_name = String(_name); }

  /**
   * @brief Get the name of the component.
   */
  const ANSICHAR*
  getName() override { return m_name.c_str(); }

  /**
   * @brief Get the component type of this component.
   * @return The component type.
   */
  static COMPONENT_TYPE::E
  getObjType() { return COMPONENT_TYPE::kModel; }

  /**
   * @brief Clean all data from the model.
   */
  void
  clean();

  /**
   * @brief Gets all the meshes of the model.
   * @return A vector with all the meshes.
   */
  Vector<SPtr<Mesh>>
  getMeshes() { return meshes; }

  /**
   * @brief Get the vertex buffer.
   * @return The vertex buffer.
   */
  SPtr<VertexBuffer>
  getVertexBuffer() { return m_vertexB; }

  /**
   * @brief Get the index buffer.
   * @return The index buffer.
   */
  SPtr<IndexBuffer>
  getIndexBuffer() { return m_indexB; }

  /**
   * @brief Set the vertex and index data to the model.
   */
  void
  setVerticesIndices();

 public:
  // vertex and index data
  Vector<SimpleVertex> vertex;
  Vector<uint32> index;


  // skeleton data
  Map<String, Bone> boneMap;
  Vector<SPtr<Mesh>> meshes;
  uint32 boneCounter = 0;

  // buffers for both vectors and index
  SPtr<VertexBuffer> m_vertexB;
  SPtr<IndexBuffer> m_indexB;

  // location data
  Path path;
 private:
  // name
  String m_name;
};
}
