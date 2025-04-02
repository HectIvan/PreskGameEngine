/************************************************************************/
/**
* @pkModel pkModel.h
* @Hector Ivan Muñoz Ceballos
* @date 20/11/2024
* @Model file for the Presk Game Engine.
*
* This file will contain the Model class used for the engine
*
* @bug No bug known.
*
* @HectIvan 21/11/2024
* added a way to get the model current component type.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkBone.h"
#include "pkComponent.h"
#include "pkIndexBuffer.h"
#include "pkMaterial.h"
#include "pkMesh.h"
#include "pkVertexBuffer.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT Model : public Component
{
 public:
  Model() = default;
  virtual ~Model()
  {
    clean();
  }

  /**
  * Load the model from a path.
  * 
  * @param _path
  * File path.
  **/
  void
  load(String& _path);

  /**
  * Get the map of bones assigned to the model.
  * 
  * @return
  * The bone map.
  **/
  auto&
  getBoneInfoMap() { return boneMap; }

  /**
  * Get the ammount of bones in the model.
  * 
  * @return
  * The bone count.
  **/
  uint32
  getBoneCount() { return boneCounter; }

  /**
  * Set the data of the vertex to a default
  * value
  * 
  * @param _vertex
  * Vertex to set to default.
  **/
  void
  setVertexBoneDataToDefault(SimpleVertex& _vertex)
  {
    _vertex = _vertex;
    for (int i = 0; i < MAX_BONE_WEIGHT; i++) {
      // _vertex.boneIDs[i] = -1;
      // _vertex.weights[i] = 0.0f;
    }
  }

  /**
  * Assign bone data to a vertex.
  * 
  * @param _vertex
  * Vertex to modify.
  * 
  * @param _boneID
  * What bone id it will have.
  * 
  * @param _weight
  * How much influence will the bone have.
  **/
  void
  setVertexBoneData(SimpleVertex& _vertex, int _boneId, float _weight);

  /**
  * Get the component type of this component.
  * 
  * @return
  * The component type.
  **/
  COMPONENT_TYPE::E
  getType() override { return COMPONENT_TYPE::kModel; }

  /**
  * Get the component type of this component.
  * 
  * @return
  * The component type.
  **/
  static COMPONENT_TYPE::E
  getObjType() { return COMPONENT_TYPE::kModel; }

  /**
  * Clean all data from the model.
  **/
  void
  clean();

 public:
  // vertex and index data
  Vector<SimpleVertex> vertex;
  Vector<uint32> index;

  // skeleton data
  Map<String, Bone> boneMap;
  Vector<SPtr<Mesh>> meshes;
  uint32 boneCounter = 0;

  // buffers for both vectors and index
  SPtr<VertexBuffer> vertexB;
  SPtr<IndexBuffer> indexB;

  // material
  Material material;

  // location data
  String path;
};
}