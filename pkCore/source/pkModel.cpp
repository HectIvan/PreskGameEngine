/*********************************************/
/**
* Includes
**/
/*********************************************/
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>

#include "pkMatrix4.h"
#include "pkModel.h"
#include "pkTexture.h"
#include "stb_image.h"

namespace pkEngineSDK
{

/*********************************************/
/**
* Declarations
**/
/*********************************************/
void
processNode(Model& _model, aiNode* _node, const aiScene* _scene);

Mesh
processMesh(aiMesh* _mesh, const aiScene* _scene);

void
Model::load(String& _path)
{
  indexB = nullptr;
  vertexB = nullptr;

  String modelPath = _path;
  path = _path;
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(modelPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality |
                                                              aiProcess_RemoveRedundantMaterials |
                                                              aiProcess_FlipUVs);
  if (scene == nullptr) { return; }
  processNode(*this, scene->mRootNode, scene);
  for (uint32 i = 0; i < meshes.size(); ++i) {
    vertex.insert(vertex.end(),
                  meshes[i].vertexVector.begin(),
                  meshes[i].vertexVector.end());

    index.insert(index.end(),
                 meshes[i].indexVector.begin(),
                 meshes[i].indexVector.end());
  }
}
  
void
processNode(Model& _model, aiNode* _node, const aiScene* _scene)
{
  for (uint32 i = 0; i < _node->mNumMeshes; ++i) {
    aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
    _model.meshes.push_back(processMesh(mesh, _scene));
  }

  for (uint32 i = 0; i < _node->mNumChildren; ++i) {
    processNode(_model, _node->mChildren[i], _scene);
  }
}

Mesh
processMesh(aiMesh* _mesh, const aiScene* _scene)
{
  Mesh meshProcess;
  meshProcess.vertexCount = _mesh->mNumVertices;
  // process vertex
  for (uint32 i = 0; i < _mesh->mNumVertices; ++i) {
    SimpleVertex sv;
    sv.pos.x = _mesh->mVertices[i].x;
    sv.pos.y = _mesh->mVertices[i].y;
    sv.pos.z = _mesh->mVertices[i].z;

    if (_mesh->HasNormals()) {
      sv.normal.x = _mesh->mNormals[i].x;
      sv.normal.y = _mesh->mNormals[i].y;
      sv.normal.z = _mesh->mNormals[i].z;
    }
    else { sv.normal = Vector3(0.0f); }

    if (_mesh->mTextureCoords[0]) {
      sv.Tex.x = _mesh->mTextureCoords[0][i].x;
      sv.Tex.y = _mesh->mTextureCoords[0][i].y;
    }
    // else { sv.Tex = Vector2(0.0f); }
    meshProcess.vertexVector.push_back(sv);
  }

  // process index
  for (uint32 i = 0; i < _mesh->mNumFaces; ++i) {
    aiFace face = _mesh->mFaces[i];

    meshProcess.numIndex += face.mNumIndices;

    for (uint32 j = 0; j < face.mNumIndices; ++j) {
      meshProcess.indexVector.push_back(face.mIndices[j]);
    }
  }
  for (uint32 i = 0; i < _mesh->mNumBones; ++i) {
    // mesh->mBones[i].
  }
  if (_mesh->mMaterialIndex >= 0) {
    aiMaterial* material = _scene->mMaterials[_mesh->mMaterialIndex];
    for (uint32 i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
      aiString path;
      material->GetTexture(aiTextureType_DIFFUSE, i, &path);
      std::cout << "Texture path: " << path.C_Str() << std::endl;
    }
    // material->GetTexture(aiTextureType_DIFFUSE);
    // material->Get(AI_MATKEY_COLOR_DIFFUSE, )
    // loadMaterialTextures(meshProcess, _scene->mMaterials[_mesh->mMaterialIndex], _scene);
  }
  
  return meshProcess;
}

Matrix4
aIMatrixToMatrix(aiMatrix4x4 _node)
{
  return Matrix4(_node.a1, _node.a2, _node.a3, _node.a4,
                 _node.b1, _node.b2, _node.b3, _node.b4,
                 _node.c1, _node.c2, _node.c3, _node.c4,
                 _node.d1, _node.d2, _node.d3, _node.d4);
}

void
Model::setVertexBoneData(SimpleVertex& _vertex, int _boneId, float _weight)
{
  /**
  * This is done to prevent warnings for the moment, at least until
  * bones are implemented correctly.
  **/
  /**************************/
  SimpleVertex vertexT = _vertex;
  vertexT.pos = Vector3(0.0f);
  _boneId = 0;
  _weight = 0;
  /**************************/
  for (uint32 i = 0; i < MAX_BONE_WEIGHT; ++i) {
  }
}

void
extractBoneWeightForVertices(Model& _model,
                             // Vector<SimpleVertex>& _vertex,
                             // const aiScene* _scene,
                             aiMesh* _mesh)
{
  for (uint32 i = 0; i < _mesh->mNumBones; ++i) {
    int boneID = -1;
    String boneName = _mesh->mBones[i]->mName.C_Str();
    if (_model.boneMap.find(boneName) == _model.boneMap.end()) {
      Bone newBone;
      newBone.setBoneID(_model.boneCounter);
      Transform transform(Matrix4::IDENTITY,
                          Matrix4::IDENTITY,
                          aIMatrixToMatrix(_mesh->mBones[i]->mOffsetMatrix));
      newBone.setLocalTransform(transform);
      _model.boneMap[boneName] = newBone;
      boneID = _model.boneCounter;
      ++_model.boneCounter;
    }
    else
    {
      boneID = _model.boneMap[boneName].getBoneID();
    }
    auto weights = _mesh->mBones[i]->mWeights;
    uint32 numWeight = _mesh->mBones[i]->mNumWeights;

    for (uint32 j = 0; j < numWeight; ++j) {
      uint32 vertexID = weights[i].mVertexId;
      float weight = weights[i].mWeight;
      _model.setVertexBoneData(_model.vertex[vertexID], boneID, weight);
    }
  }
}

void
loadMaterial(const aiScene* _scene)//, String& _fileName) // PkMesh& _mesh, 
{
  // String texturePath = "textures/" + _fileName;
  // aiReturn ret;
  for (uint32 i = 0; i < _scene->mNumMaterials; ++i) {
    const aiMaterial* pMaterial = _scene->mMaterials[i];
    if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
      pMaterial->GetName();
    }
  }
}

void
Model::clean()
{
  vertex.clear();
  index.clear();

  boneMap.clear();
  meshes.clear();
  boneCounter = 0;

  vertexB = make_shared<VertexBuffer>();
  indexB = make_shared<IndexBuffer>();

  material = Material();

  path = "";
}
}