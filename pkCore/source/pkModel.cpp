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

#include "pkGraphicsAPI.h"
#include "pkLogger.h"
#include "pkMatrix4.h"
#include "pkModel.h"
#include "pkTexture.h"
#include "pkTextureManager.h"
#include "pkResourceManager.h"

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

SPtr<Mesh>
processMesh(aiMesh* _mesh, const aiScene* _scene);

bool
Model::load(Path& _path)
{
  m_indexB = nullptr;
  m_vertexB = nullptr;

  String modelPath = _path.toString();
  path = _path;
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(modelPath.c_str(),
                                           aiProcessPreset_TargetRealtime_MaxQuality |
                                           aiProcess_RemoveRedundantMaterials |
                                           aiProcess_FlipUVs);
  if (!scene) {
    g_Logger().registerMessage("Failed to load model at directory " + modelPath + ".",
                               LOG_MSG_TYPE::kWarning);
    return false;
  }
  setName(scene->mName.C_Str());
  processNode(*this, scene->mRootNode, scene);
  for (uint32 i = 0; i < meshes.size(); ++i) {
    vertex.insert(vertex.end(),
                  meshes[i]->vertexVector.begin(),
                  meshes[i]->vertexVector.end());

    index.insert(index.end(),
                 meshes[i]->indexVector.begin(),
                 meshes[i]->indexVector.end());
  }
  return true;
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

SPtr<Mesh>
processMesh(aiMesh* _mesh, const aiScene* _scene)
{
  // modules
  ResourceManager& rm = g_ResourceManager().instance();
  TextureManager& tm = g_TextureManager().instance();
  Logger& log = g_Logger().instance();

  // check if the mesh is already in storage
  String meshName(_mesh->mName.C_Str());
  SPtr<Mesh> meshProcess = rm.searchMesh(meshName);
  if (meshProcess) {
    log.registerMessage("Found pre-loaded mesh of name " + meshName + ".");
    return meshProcess;
  }

  // create the mesh
  meshProcess = make_shared<Mesh>();
  meshProcess->setName(meshName);
  meshProcess->vertexCount = _mesh->mNumVertices;
  meshProcess->vertexVector.resize(_mesh->mNumVertices);

  // exist checks
  const aiVector3D* meshTexCoords = _mesh->mTextureCoords[0];
  const bool hasTexCoords = (meshTexCoords != nullptr);
  const bool tbExist = _mesh->HasTangentsAndBitangents();
  const bool hasNormals = _mesh->HasNormals();
  // default vectors for when no data is found
  const Vector3 zero3 = Vector3::ZERO;
  const Vector2 zero2 = Vector2::ZERO;

  // process vertex
  for (uint32 i = 0; i < _mesh->mNumVertices; ++i) {
    // set positions
    Vector3 pos = Vector3(_mesh->mVertices[i].x, _mesh->mVertices[i].y, _mesh->mVertices[i].z);
    
    // get normal directions
    Vector3 normal = (hasNormals) ?
                      Vector3(_mesh->mNormals[i].x, _mesh->mNormals[i].y, _mesh->mNormals[i].z)
                      : zero3;
    
    // if there are texture coordinates, store them
    Vector2 tex = (hasTexCoords) ? Vector2(meshTexCoords[i].x, meshTexCoords[i].y) : zero2;
    
    // check if the model has tantents and bitangents
    Vector3 tangent = (tbExist) ? Vector3(_mesh->mTangents[i].x,
                                          _mesh->mTangents[i].y,
                                          _mesh->mTangents[i].z) : zero3;
    
    Vector3 bitangent = (tbExist) ? Vector3(_mesh->mBitangents[i].x,
                                            _mesh->mBitangents[i].y,
                                            _mesh->mBitangents[i].z) : zero3;
    // create and add a new vertex
    SimpleVertex sv = SimpleVertex(pos, normal, tex, tangent, bitangent);
    meshProcess->vertexVector[i] = sv;
  }

  // process index
  for (uint32 i = 0; i < _mesh->mNumFaces; ++i) {
    aiFace face = _mesh->mFaces[i];

    meshProcess->numIndex += face.mNumIndices;

    for (uint32 j = 0; j < face.mNumIndices; ++j) {
      meshProcess->indexVector.push_back(face.mIndices[j]);
    }
  }
  // for (uint32 i = 0; i < _mesh->mNumBones; ++i) {
  //   mesh->mBones[i].
  // }
  if (_mesh->mMaterialIndex >= 0) {
    aiMaterial* materialA = _scene->mMaterials[_mesh->mMaterialIndex];
    meshProcess->material = make_shared<Material>();
    String meshName = _mesh->mName.C_Str();
    // load default textures.
    meshProcess->material->setDiffuse(tm.m_defaultDiff);
    meshProcess->material->setNormal(tm.m_defaultNormal);
    meshProcess->material->setOcclusion(tm.m_defaultAO);
    meshProcess->material->setHeight(tm.m_defaultHeight);
    meshProcess->material->setMetallic(tm.m_defaultMetallic);
    meshProcess->material->setRoughness(tm.m_defaultRough);
    meshProcess->material->setEmissive(tm.m_defaultEmissive);

    String matName = materialA->GetName().C_Str();
    meshProcess->material->setName(matName);
    Path filePath;
    // if no diffuse texture is found.
    uint32 diffCount = materialA->GetTextureCount(aiTextureType_DIFFUSE);
    if (diffCount < 1) {
      log.registerMessage("Could not find diffuse texture of material " + matName + ".",
                          LOG_MSG_TYPE::kWarning);
    }
    // if there are diffuse textures.
    for (uint32 i = 0; i < diffCount; ++i) {
      aiString path;
      // diffuse texture loading.
      if (materialA->GetTexture(aiTextureType_DIFFUSE, i, &path) == AI_SUCCESS) {
        // load the texture.
        Path newPath(path.C_Str());
        SPtr<Texture> texture = tm.loadTexture(newPath);
        // if a texture was loaded.
        if (texture) {
          // log registry.
          log.registerMessage("Loaded diffuse texture " + newPath.getFileName() +
                              " in material " + matName + ".");
          meshProcess->material->setDiffuse(texture);
        }
      }
      else { // register that a diffuse texture was not found.
        filePath = Path(path.C_Str()).getFileName();
        log.registerMessage("Failed to load diffuse texture" + filePath.toString() +
                            " in material " + matName + ".", LOG_MSG_TYPE::kWarning);
      }
    }

    // get all normal maps of the material.
    uint32 normCount = materialA->GetTextureCount(aiTextureType_HEIGHT);
    for (uint32 i = 0; i < normCount; ++i) {
      aiString path;
      // normal texture loading.
      if (materialA->GetTexture(aiTextureType_HEIGHT, i, &path) == AI_SUCCESS) {
        // load the texture.
        Path newPath(path.C_Str());
        SPtr<Texture> texture = tm.loadTexture(newPath);
        // if a texture was loaded.
        if (texture) {
          // log registry.
          log.registerMessage("Loaded normal texture " + newPath.getFileName() +
                              " in material " + matName + ".");
          meshProcess->material->setNormal(texture);
        }
      }
      else { // register that a normal texture was not found.
        filePath = Path(path.C_Str()).getFileName();
        log.registerMessage("Failed to load normal texture" + filePath.toString() +
                            " in material " + matName + ".", LOG_MSG_TYPE::kWarning);
      }
    }

    // get all ambient occlusion maps of the material.
    uint32 aoCount = materialA->GetTextureCount(aiTextureType_AMBIENT);
    for (uint32 i = 0; i < aoCount; ++i) {
      aiString path;
      // ambient occlusion texture loading.
      if (materialA->GetTexture(aiTextureType_AMBIENT, i, &path) == AI_SUCCESS) {
        // load the texture.
        Path newPath(path.C_Str());
        SPtr<Texture> texture = tm.loadTexture(newPath);
        // if a texture was loaded.
        if (texture) {
          // log registry.
          log.registerMessage("Loaded ambient occlussion texture " + newPath.getFileName() +
                              " in material " + matName + ".");
          meshProcess->material->setOcclusion(texture);
        }
      }
      else { // register that an ambient occlussion texture was not found.
        filePath = Path(path.C_Str()).getFileName();
        log.registerMessage("Failed to load ambient occlussion texture" + filePath.toString() +
                            " in material " + matName + ".",
                            LOG_MSG_TYPE::kWarning);
      }
    }

    // get all metallic maps of the material
    uint32 metallicCount = materialA->GetTextureCount(aiTextureType_METALNESS);
    for (uint32 i = 0; i < metallicCount; ++i) {
      aiString path;
      // metallic texture loading.
      if (materialA->GetTexture(aiTextureType_METALNESS, i, &path) == AI_SUCCESS) {
        // load the texture.
        Path newPath(path.C_Str());
        SPtr<Texture> texture = tm.loadTexture(newPath);
        // if a texture was loaded.
        if (texture) {
          // log registry.
          log.registerMessage("Loaded metallic texture " + newPath.getFileName() +
                              " in material " + matName + ".");
          meshProcess->material->setMetallic(texture);
        }
      }
      else { // register that a metallic texture was not found.
        filePath = Path(path.C_Str()).getFileName();
        log.registerMessage("Failed to load metallic texture" + filePath.toString() +
                            " in material " + matName + ".", LOG_MSG_TYPE::kWarning);
      }
    }

    // get all roughness maps of the material
    uint32 roughnessCount = materialA->GetTextureCount(aiTextureType_REFLECTION);
    for (uint32 i = 0; i < roughnessCount; ++i) {
      aiString path;
      // roughness texture loading.
      if (materialA->GetTexture(aiTextureType_REFLECTION, i, &path) == AI_SUCCESS) {
        // load the texture.
        Path newPath(path.C_Str());
        SPtr<Texture> texture = tm.loadTexture(newPath);
        // if a texture was loaded.
        if (texture) {
          // log registry.
          log.registerMessage("Loaded roughness texture " + newPath.getFileName() +
                              " in material " + matName + ".", LOG_MSG_TYPE::kWarning);
          meshProcess->material->setRoughness(texture);
        }
      }
      else { // register that a roughness texture was not found.
        filePath = Path(path.C_Str()).getFileName();
        log.registerMessage("Failed to load roughness texture" + filePath.toString() +
                            " in material " + matName + ".", LOG_MSG_TYPE::kWarning);
      }
    }

    // get all emissive maps of the material
    uint32 emissiveCount = materialA->GetTextureCount(aiTextureType_EMISSIVE);
    for (uint32 i = 0; i < emissiveCount; ++i) {
      aiString path;
      // emissive texture loading.
      if (materialA->GetTexture(aiTextureType_EMISSIVE, i, &path) == AI_SUCCESS) {
        // load the texture.
        Path newPath(path.C_Str());
        SPtr<Texture> texture = tm.loadTexture(newPath);
        // if a texture was loaded.
        if (texture) {
          // log registry.
          log.registerMessage("Loaded roughness texture " + newPath.getFileName() +
                              " in material " + matName + ".", LOG_MSG_TYPE::kWarning);
          meshProcess->material->setEmissive(texture);
        }
      }
      else { // register that an emissive texture was not found.
        filePath = Path(path.C_Str()).getFileName();
        log.registerMessage("Failed to load emissive texture" + filePath.toString() +
                            " in material " + matName + ".", LOG_MSG_TYPE::kWarning);
      }
    }
    // materialA->GetTexture(aiTextureType_DIFFUSE);
    // materialA->Get(AI_MATKEY_COLOR_DIFFUSE, )
    // loadMaterialTextures(meshProcess, _scene->mMaterials[_mesh->mMaterialIndex], _scene);
  }
  log.registerMessage("Loaded mesh of name " + String(_mesh->mName.C_Str()) + ".");
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
  // for (uint32 i = 0; i < MAX_BONE_WEIGHT; ++i) {
  // }
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

  m_vertexB = make_shared<VertexBuffer>();
  m_indexB = make_shared<IndexBuffer>();
}
}