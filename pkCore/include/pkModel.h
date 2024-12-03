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
* @HectIvan 20/11/2024
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>

#include "pkBone.h"
#include "pkIndexBuffer.h"
#include "pkMatrix4.h"
#include "pkMesh.h"
#include "pkPrerequisitesCore.h"
#include "pkVertexBuffer.h"


namespace pkEngineSDK
{

class Device;

class Model
{
 public:
  Model() = default;
  virtual ~Model() = default;

  void
  load(String _file);

  void
  clean();

  void
  processNode(aiNode* _node, const aiScene* _scene);

  PkMesh
  processMesh(aiMesh* _mesh, const aiScene* _scene);

  Matrix4
  aIMatrixToMatrix(aiMatrix4x4 _node);

  auto&
  getBoneInfoMap() { return mBoneMap; }

  uint32
  getBoneCount() { return mBoneCounter; }

  void
  setVertexBoneDataToDefault(SimpleVertex& _vertex)
  {
    for (int i = 0; i < MAX_BONE_WEIGHT; i++)
    {
      _vertex.boneIDs[i] = -1;
      _vertex.weights[i] = 0.0f;
    }
  }

  void
  setVertexBoneData(SimpleVertex& _vertex, int _boneId, float _weight);

  void
  extractBoneWeightForVertices(Vector<SimpleVertex>& _vertex,
                               aiMesh* _mesh,
                               const aiScene* _scene);

  void
  loadMaterial(PkMesh& _mesh, const aiScene* _scene, String& _fileName);

  Vector<SimpleVertex> m_vertex;
  Vector<uint32> m_index;

  Map<String, Bone> mBoneMap;
  Vector<Mesh> m_meshes;
  uint32 mBoneCounter = 0;
  String m_directory;

  // buffers for both vectors and index
  SPtr<VertexBuffer> m_vertexB = nullptr;
  SPtr<IndexBuffer> m_indexB = nullptr;
};
}