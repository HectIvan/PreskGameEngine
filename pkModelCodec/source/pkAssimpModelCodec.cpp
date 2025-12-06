/*****************************************************************************/
/**
 * @file    pkAssimpModelCodec.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    08/10/2025
 * @brief   Codec for loading models using Assimp.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "pkAssetResourceManager.h"
#include "pkAssimpModelCodec.h"
#include "pkGPUResourceManager.h"
#include "pkLogger.h"
#include "pkFileSystem.h"
#include "pkMaterialResource.h"
#include "pkModelResource.h"
#include "pkTexture.h"
#include "pkTextureCodec.h"
#include "pkTextureManager.h"
#include "pkWindow.h"

namespace pkEngineSDK
{

extern "C" __declspec(dllexport) void
loadPlugin()
{
  AssimpModelCodec::startUp<AssimpModelCodec>();
}

/**
 * @brief Processes an aiNode and its children recursively.
 * @param _model Model to which the processed meshes will be added.
 * @param _node aiNode to process.
 * @param _scene aiScene containing the node.
 */
void
processNode(Model& _model, const aiNode* _node, const aiScene* _scene);

/**
 * @brief Processes an aiMesh and converts it to a Mesh.
 * @param _mesh aiMesh to process.
 * @param _scene aiScene containing the mesh.
 * @param _transform Transformation matrix to apply to the mesh.
 * @return Pointer to the processed Mesh.
 */
SPtr<Mesh>
processMesh(const aiMesh* _mesh, const aiScene* _scene, const Matrix4 _transform);

/**
 * @brief Converts an aiMatrix4x4 to a Matrix4.
 * @param _transform aiMatrix4x4 to convert.
 * @return Converted Matrix4.
 */
Matrix4
aiTransformToMatrix4(aiMatrix4x4 _transform);

SPtr<BaseResource>
AssimpModelCodec::createResourceFromFile(const Path _path)
{
  Logger& log = g_Logger();
  SPtr<Model> model = make_shared<Model>();

  const String modelPath = FileSystem::getAbsolutePath(_path).string();
  model->path = _path;
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(modelPath.c_str(),
                                           aiProcessPreset_TargetRealtime_MaxQuality |
                                           aiProcess_RemoveRedundantMaterials |
                                           aiProcess_FlipUVs);
  if (!scene) {
    const String msg = "Assimp failed to load model at directory " + modelPath + ".";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }

  model->setName(scene->mName.C_Str());
  processNode(*model, scene->mRootNode, scene);
  model->setVerticesIndices();

  return createResourceFromModel(model, _path);
}

SPtr<Texture>
getTextureFromPath(const Path& _path)
{
  TextureCodec& textureCodec = g_TextureCodec();
  TextureManager& tm = g_TextureManager();
  AssetResourceManager& assetMan = g_AssetResourceManager();

  // if a texture with this path has already been loaded.
  SPtr<Texture> texture = tm.getTextureFromPath(_path.toString());
  if (texture) {
    return texture;
  }

  SPtr<BaseResource> resource = textureCodec.createResourceFromFile(_path);
  if (!resource) {
    return nullptr;
  }
  assetMan.insertNewResource(resource);
  texture = tm.loadTexture(resource->m_id);
  return texture;
}

Bone
AssimpModelCodec::aiboneToBone(const String& _name, int32 _ID, const aiNodeAnim* _channel)
{
  Bone newBone;
  newBone.setBoneName(_name);
  newBone.setBoneID(_ID);
  newBone.setLocalTransform(Matrix4(1.0f));

  // get all the positions
  newBone.numPositions = _channel->mNumPositionKeys;
  for (uint32 i = 0; i < newBone.numPositions; ++i) {
    Vector3 position = Vector3(_channel->mPositionKeys[i].mValue.x,
                               _channel->mPositionKeys[i].mValue.y,
                               _channel->mPositionKeys[i].mValue.z);
    float timeStamp = static_cast<float>(_channel->mPositionKeys[i].mTime);
    KeyPosition data;
    data.position = position;
    data.timeStamp = timeStamp;
    newBone.positions.push_back(data);
  }

  // get all the rotations
  newBone.numRotations = _channel->mNumRotationKeys;
  for (uint32 i = 0; i < newBone.numRotations; ++i) {
    Vector4 rotation = Vector4(_channel->mRotationKeys[i].mValue.x,
      _channel->mRotationKeys[i].mValue.y,
      _channel->mRotationKeys[i].mValue.z,
      _channel->mRotationKeys[i].mValue.w);
    float timeStamp = static_cast<float>(_channel->mRotationKeys[i].mTime);
    KeyRotation data;
    data.rotation = rotation;
    data.timeStamp = timeStamp;
    newBone.rotations.push_back(data);
  }

  // get all the scales
  newBone.numScales = _channel->mNumScalingKeys;
  for (uint32 i = 0; i < newBone.numScales; ++i) {
    Vector3 scale = Vector3(_channel->mScalingKeys[i].mValue.x,
      _channel->mScalingKeys[i].mValue.y,
      _channel->mScalingKeys[i].mValue.z);
    float timeStamp = static_cast<float>(_channel->mRotationKeys[i].mTime);
    KeyScale data;
    data.scale = scale;
    data.timeStamp = timeStamp;
    newBone.scales.push_back(data);
  }

  return newBone;
}

void
processNode(Model& _model, const aiNode* _node, const aiScene* _scene)
{
  for (uint32 i = 0; i < _node->mNumMeshes; ++i) {
    aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
    Matrix4 transform = aiTransformToMatrix4(_node->mTransformation);
    _model.meshes.push_back(processMesh(mesh, _scene, transform)); // , _node->mTransformation
  }

  for (uint32 i = 0; i < _node->mNumChildren; ++i) {
    processNode(_model, _node->mChildren[i], _scene);
  }
}

Matrix4
aiTransformToMatrix4(aiMatrix4x4 _transform)
{
  Matrix4 M(_transform.a1, _transform.a2, _transform.a3, _transform.a4,
            _transform.b1, _transform.b2, _transform.b3, _transform.b4,
            _transform.c1, _transform.c2, _transform.c3, _transform.c4,
            _transform.d1, _transform.d2, _transform.d3, _transform.d4);

  return M;
}

SPtr<Mesh>
processMesh(const aiMesh* _mesh, const aiScene* _scene, const Matrix4 _transform)
{
  // modules
  GPUResourceManager& rm = g_GPUResourceManager();
  Logger& log = g_Logger();

  // check if the mesh is already in storage
  String meshName(_mesh->mName.C_Str());
  SPtr<Mesh> meshProcess = rm.searchMesh(meshName);
  // if a mesh can be found
  if (meshProcess) {
    log.registerMessage("Found pre-loaded mesh of name " + meshName + ".", __FILE__, __LINE__);
    return meshProcess;
  }

  // create the mesh
  meshProcess = make_shared<Mesh>();
  meshProcess->setName(meshName);
  meshProcess->m_transform = _transform;
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

  /**
   * Material loading
   */
  meshProcess->material = rm.m_defaultMaterial;
  if (_mesh->mMaterialIndex >= 0) {
    // material data
    const aiMaterial* materialA = _scene->mMaterials[_mesh->mMaterialIndex];
    const String matName = materialA->GetName().C_Str();
    meshProcess->material = rm.newMaterial(matName);
    // material 
    Path filePath;
    
    /**
     * Albedo load.
     */
    const uint32 diffCount = materialA->GetTextureCount(aiTextureType_DIFFUSE);
    if (diffCount < 1) {
      log.registerMessage("Could not find albedo texture of material " + matName + ".",
                          __FILE__, __LINE__, LOG_MSG_TYPE::kWarning);
    }
    // if there are albedo textures.
    for (uint32 i = 0; i < diffCount; ++i) {
      aiString path;
      // albedo texture loading.
      if (materialA->GetTexture(aiTextureType_DIFFUSE, i, &path) == AI_SUCCESS) {
        // load the texture.
        const Path newPath(path.C_Str());
        const SPtr<Texture> texture = getTextureFromPath(newPath);
        // if a texture was loaded.
        if (texture) {
          // log registry.
          log.registerMessage("Loaded albedo texture " + newPath.getFileName() +
                              " in material " + matName + ".",
                              __FILE__,
                              __LINE__);
          meshProcess->material->setAlbedo(texture);
        }
      }
      else { // register that a albedo texture was not found.
        filePath = Path(path.C_Str()).getFileName();
        log.registerMessage("Failed to load albedo texture" + filePath.toString() +
                            " in material " + matName + ".",
                            __FILE__,
                            __LINE__,
                            LOG_MSG_TYPE::kWarning);
      }
    }

    /**
     * Normal load.
     */
    const uint32 normCount = materialA->GetTextureCount(aiTextureType_HEIGHT);
    for (uint32 i = 0; i < normCount; ++i) {
      aiString path;
      // normal texture loading.
      if (materialA->GetTexture(aiTextureType_HEIGHT, i, &path) == AI_SUCCESS) {
        // load the texture.
        const Path newPath(path.C_Str());
        const SPtr<Texture> texture = getTextureFromPath(newPath);
        // if a texture was loaded.
        if (texture) {
          // log registry.
          log.registerMessage("Loaded normal texture " + newPath.getFileName() +
                              " in material " + matName + ".",
                              __FILE__,
                              __LINE__);
          meshProcess->material->setNormal(texture);
        }
      }
      else { // register that a normal texture was not found.
        filePath = Path(path.C_Str()).getFileName();
        log.registerMessage("Failed to load normal texture" + filePath.toString() +
                            " in material " + matName + ".",
                            __FILE__,
                            __LINE__,
                            LOG_MSG_TYPE::kWarning);
      }
    }

    /**
     * AO load.
     */
    const uint32 aoCount = materialA->GetTextureCount(aiTextureType_AMBIENT);
    for (uint32 i = 0; i < aoCount; ++i) {
      aiString path;
      // ambient occlusion texture loading.
      if (materialA->GetTexture(aiTextureType_AMBIENT, i, &path) == AI_SUCCESS) {
        // load the texture.
        const Path newPath(path.C_Str());
        const SPtr<Texture> texture = getTextureFromPath(newPath);
        // if a texture was loaded.
        if (texture) {
          // log registry.
          log.registerMessage("Loaded ambient occlussion texture " + newPath.getFileName() +
                              " in material " + matName + ".",
                              __FILE__,
                              __LINE__);
          meshProcess->material->setOclussion(texture);
        }
      }
      else { // register that an ambient occlussion texture was not found.
        filePath = Path(path.C_Str()).getFileName();
        log.registerMessage("Failed to load ambient occlussion texture" + filePath.toString() +
                            " in material " + matName + ".",
                            __FILE__,
                            __LINE__,
                            LOG_MSG_TYPE::kWarning);
      }
    }

    /**
     * Metallic load.
     */
    const uint32 metallicCount = materialA->GetTextureCount(aiTextureType_METALNESS);
    for (uint32 i = 0; i < metallicCount; ++i) {
      aiString path;
      // metallic texture loading.
      if (materialA->GetTexture(aiTextureType_METALNESS, i, &path) == AI_SUCCESS) {
        // load the texture.
        const Path newPath(path.C_Str());
        const SPtr<Texture> texture = getTextureFromPath(newPath);
        // if a texture was loaded.
        if (texture) {
          // log registry.
          log.registerMessage("Loaded metallic texture " + newPath.getFileName() +
                              " in material " + matName + ".",
                              __FILE__,
                              __LINE__);
          meshProcess->material->setMetallic(texture);
        }
      }
      else { // register that a metallic texture was not found.
        filePath = Path(path.C_Str()).getFileName();
        log.registerMessage("Failed to load metallic texture" + filePath.toString() +
                            " in material " + matName + ".",
                            __FILE__,
                            __LINE__,
                            LOG_MSG_TYPE::kWarning);
      }
    }

    /**
     * Roughness load.
     */
    const uint32 roughnessCount = materialA->GetTextureCount(aiTextureType_REFLECTION);
    for (uint32 i = 0; i < roughnessCount; ++i) {
      aiString path;
      // roughness texture loading.
      if (materialA->GetTexture(aiTextureType_REFLECTION, i, &path) == AI_SUCCESS) {
        // load the texture.
        const Path newPath(path.C_Str());
        const SPtr<Texture> texture = getTextureFromPath(newPath);
        // if a texture was loaded.
        if (texture) {
          // log registry.
          log.registerMessage("Loaded roughness texture " + newPath.getFileName() +
                              " in material " + matName + ".",
                              __FILE__,
                              __LINE__,
                              LOG_MSG_TYPE::kWarning);
          meshProcess->material->setRoughness(texture);
        }
      }
      else { // register that a roughness texture was not found.
        filePath = Path(path.C_Str()).getFileName();
        log.registerMessage("Failed to load roughness texture" + filePath.toString() +
                            " in material " + matName + ".",
                            __FILE__,
                            __LINE__,
                            LOG_MSG_TYPE::kWarning);
      }
    }

    /**
     * Emissive load.
     */
    const uint32 emissiveCount = materialA->GetTextureCount(aiTextureType_EMISSIVE);
    for (uint32 i = 0; i < emissiveCount; ++i) {
      aiString path;
      // emissive texture loading.
      if (materialA->GetTexture(aiTextureType_EMISSIVE, i, &path) == AI_SUCCESS) {
        // load the texture.
        const Path newPath(path.C_Str());
        const SPtr<Texture> texture = getTextureFromPath(newPath);
        // if a texture was loaded.
        if (texture) {
          // log registry.
          log.registerMessage("Loaded roughness texture " + newPath.getFileName() +
                              " in material " + matName + ".",
                              __FILE__,
                              __LINE__,
                              LOG_MSG_TYPE::kWarning);
          meshProcess->material->setEmissive(texture);
        }
      }
      else { // register that an emissive texture was not found.
        filePath = Path(path.C_Str()).getFileName();
        log.registerMessage("Failed to load emissive texture" + filePath.toString() +
                            " in material " + matName + ".",
                            __FILE__,
                            __LINE__,
                            LOG_MSG_TYPE::kWarning);
      }
    }
    // materialA->GetTexture(aiTextureType_DIFFUSE);
    // materialA->Get(AI_MATKEY_COLOR_DIFFUSE, )
    // loadMaterialTextures(meshProcess, _scene->mMaterials[_mesh->mMaterialIndex], _scene);
  }
  log.registerMessage("Loaded mesh of name " + String(_mesh->mName.C_Str()) + ".",
                      __FILE__,
                      __LINE__);
  return meshProcess;
}

Matrix4
aIMatrixToMatrix(const aiMatrix4x4 _node)
{
  return Matrix4(_node.a1, _node.a2, _node.a3, _node.a4,
                 _node.b1, _node.b2, _node.b3, _node.b4,
                 _node.c1, _node.c2, _node.c3, _node.c4,
                 _node.d1, _node.d2, _node.d3, _node.d4);
}

void
extractBoneWeightForVertices(Model& _model,
  // Vector<SimpleVertex>& _vertex,
  // const aiScene* _scene,
  aiMesh* _mesh)
{
  for (uint32 i = 0; i < _mesh->mNumBones; ++i) {
    // uint32 boneID = -1;
    String boneName = _mesh->mBones[i]->mName.C_Str();
    if (_model.boneMap.find(boneName) == _model.boneMap.end()) {
      Bone newBone;
      newBone.setBoneID(_model.boneCounter);
      newBone.setLocalTransform(Matrix4::IDENTITY);
      _model.boneMap[boneName] = newBone;
      // boneID = _model.boneCounter;
      ++_model.boneCounter;
    }
    // else
    // {
    //   // boneID = _model.boneMap[boneName].getBoneID();
    // }
    // auto weights = _mesh->mBones[i]->mWeights;
    // uint32 numWeight = _mesh->mBones[i]->mNumWeights;

    // for (uint32 j = 0; j < numWeight; ++j) {
    //   uint32 vertexID = weights[i].mVertexId;
    //   float weight = weights[i].mWeight;
    //   _model.setVertexBoneData(_model.vertex[vertexID], boneID, weight);
    // }
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

}
