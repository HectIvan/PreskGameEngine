#include "pkModel.h"

namespace pkEngineSDK
{

void
Model::load(String _file)
{
  String modelPath = "models/" + _file;
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(modelPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
  if (scene == nullptr) { return; }
  processNode(scene->mRootNode, scene);
  for (uint32 i = 0; i < m_meshes.size(); ++i)
  {
    m_vertex.insert(m_vertex.end(),
                    m_meshes[i].m_vertexVector.begin(),
                    m_meshes[i].m_vertexVector.end());

    m_index.insert(m_index.end(),
                   m_meshes[i].m_indexVector.begin(),
                   m_meshes[i].m_indexVector.end());
  }
}

void
Model::clean()
{
  for (uint32 i = 0; i < m_meshes.size(); ++i)
  {
    m_meshes[i].Clean();
  }
}
  
void
Model::processNode(aiNode* _node, const aiScene* _scene)
{
  for (uint32 i = 0; i < _node->mNumMeshes; ++i)
  {
    aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
    m_meshes.push_back(&processMesh(mesh, _scene));
  }

  for (uint32 i = 0; i < _node->mNumChildren; ++i)
  {
    processNode(_node->mChildren[i], _scene);
  }
}

PkMesh
Model::processMesh(aiMesh* _mesh, const aiScene* _scene)
{
  PkMesh meshProcess = new PkMesh();
  meshProcess.m_vertexCount = _mesh->mNumVertices;
  // process vertex
  for (uint32 i = 0; i < _mesh->mNumVertices; ++i)
  {
    SimpleVertex sv;
    sv.pos.x = _mesh->mVertices[i].x;
    sv.pos.y = _mesh->mVertices[i].y;
    sv.pos.z = _mesh->mVertices[i].z;

    if (_mesh->HasNormals())
    {
      sv.normal.x = _mesh->mNormals[i].x;
      sv.normal.y = _mesh->mNormals[i].y;
      sv.normal.z = _mesh->mNormals[i].z;
    }
    else { sv.normal = Vector3(0.0f); }

    if (_mesh->mTextureCoords[0] != nullptr)
    {
      sv.Tex.x = _mesh->mTextureCoords[0][i].x;
      sv.Tex.y = _mesh->mTextureCoords[0][i].y;
    }
    else { sv.Tex = Vector2(0.0f); }
    meshProcess.m_vertexVector.push_back(sv);
  }

  // process index
  for (uint32 i = 0; i < _mesh->mNumFaces; ++i)
  {
    aiFace face = _mesh->mFaces[i];

    meshProcess.m_numIndex += face.mNumIndices;

    for (uint32 j = 0; j < face.mNumIndices; ++j)
    {
      meshProcess.m_indexVector.push_back(face.mIndices[j]);
    }
  }
  for (uint32 i = 0; i < _mesh->mNumBones; ++i)
  {
    // mesh->mBones[i].
  }
  // for (uint32 i = 0; i < scene->mNumMaterials; ++i)
  // {
  //   LoadMaterialTextures(meshProcess, scene->mMaterials[mesh->mMaterialIndex], scene);
  // }
  return meshProcess;
}

Matrix4
Model::aIMatrixToMatrix(aiMatrix4x4 _node)
{
  return Matrix4(_node.a1, _node.a2, _node.a3, _node.a4,
                 _node.b1, _node.b2, _node.b3, _node.b4,
                 _node.c1, _node.c2, _node.c3, _node.c4,
                 _node.d1, _node.d2, _node.d3, _node.d4);
}

void
Model::setVertexBoneData(SimpleVertex& _vertex, int _boneId, float _weight)
{
  for (uint32 i = 0; i < MAX_BONE_WEIGHT; ++i)
  {
  }
}

void
Model::extractBoneWeightForVertices(Vector<SimpleVertex>& _vertex,
                                    aiMesh* _mesh,
                                    const aiScene* _scene)
{
  for (uint32 i = 0; i < _mesh->mNumBones; ++i)
  {
    int boneID = -1;
    String boneName = _mesh->mBones[i]->mName.C_Str();
    if (mBoneMap.find(boneName) == mBoneMap.end())
    {
      Bone newBone;
      newBone.m_ID = mBoneCounter;
      Transform transform(Matrix4::IDENTITY,
                          Matrix4::IDENTITY,
                          aIMatrixToMatrix(_mesh->mBones[i]->mOffsetMatrix));
      newBone.m_transform = transform;
      mBoneMap[boneName] = newBone;
      boneID = mBoneCounter;
      ++mBoneCounter;
    }
    else
    {
      boneID = mBoneMap[boneName].m_ID;
    }
    auto weights = _mesh->mBones[i]->mWeights;
    uint32 numWeight = _mesh->mBones[i]->mNumWeights;

    for (uint32 j = 0; j < numWeight; ++j)
    {
      uint32 vertexID = weights[i].mVertexId;
      float weight = weights[i].mWeight;
      setVertexBoneData(m_vertex[vertexID], boneID, weight);
    }
  }
}

void
Model::loadMaterial(PkMesh& _mesh, const aiScene* _scene, String& _fileName)
{
  String texturePath = "textures/" + _fileName;
  aiReturn ret;
  for (uint32 i = 0; i < _scene->mNumMaterials; ++i)
  {
    const aiMaterial* pMaterial = _scene->mMaterials[i];
    if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {

    }
  }
}
}