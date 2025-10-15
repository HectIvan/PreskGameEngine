#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "pkGPUResourceManager.h"
#include "pkGraphicsAPI.h"
#include "pkLogger.h"
#include "pkModelResource.h"
#include "pkModel.h"
#include "pkTexture.h"
#include "pkTextureManager.h"

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
processMesh(aiMesh* _mesh, const aiScene* _scene, const Matrix4 _transform);

Matrix4
aiTransformToMatrix4(aiMatrix4x4 _transform);

void
GPUResourceManager::init()
{
  m_defaultMaterial = newMaterial(m_defaultMatName);
}

SPtr<Material>
GPUResourceManager::newMaterial(String _name)
{
  // create the material.
  if (_name == m_defaultMatName) {
    _name = "<Invalid Name>";
  }
  SPtr<Material> pMatComp = make_shared<Material>(_name);
  // return the material.
  return pMatComp;
}

SPtr<Model>
GPUResourceManager::loadModel(Path _directory)
{
  GraphicsAPI& api = g_GraphicAPI();

  // Get model directory
  String dir = _directory.toString();
  // search if the model has been stored before
  for (uint32 i = 0; i < m_models.size(); ++i) {
    if (m_models[i]->directory.toString() == dir) {
      return m_models[i]->model;
      g_Logger().registerMessage("Found pre-loaded model of directory " + dir + ".");
    }
  }

  // create the model pointer
  SPtr<Model> model = make_shared<Model>();

  // load the model from the path depending on extension.
  if (_directory.getExtension() == "pkm") {
    model = loadPKModel(_directory);
    if (!model) {
      model->~Model();
      return nullptr;
    }
  }
  else {
    model = loadAssimpModel(_directory);
    if (!model) {
      model->~Model();
      return nullptr;
    }
  }

  // create the index and vertex buffers
  model->m_vertexB = api.createVertexBuffer(model->vertex);
  model->m_indexB = api.createIndexBuffer(model->index);
  api.setIndexBuffer(model->m_indexB);
  api.setVertexBuffer(model->m_vertexB);

  // store the model in memory for later use if needed
  SPtr<ModelMemory> newModelMem = make_shared<ModelMemory>();
  newModelMem->directory = _directory;
  newModelMem->model = model;
  m_models.push_back(newModelMem);
  // return the final model
  return model;
}

SPtr<Model>
GPUResourceManager::loadPKModel(Path _path)
{
  Logger& log = g_Logger();
  GPUResourceManager& GPUResourceMan = g_GPUResourceManager();

  SPtr<Model> model = make_shared<Model>();
  // to do: change where this is done. (pkAssetResourceManager)
  ifstream file;
  file.open(_path.getPath(), ios::in | ios::binary);

  // if the file fails to open, return false.
  if (!file.is_open()) {
    String msg = "Failed to open model at directory " + _path.getPath() + ".";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }

  // get model header.
  uint32 sizeMHeader = sizeof(ModelAssetHeader);
  ModelAssetHeader* modelHeader = new ModelAssetHeader();
  file.read(reinterpret_cast<char*>(modelHeader), sizeMHeader);

  model->meshes.resize(modelHeader->meshCount);
  // for each mesh in the model, get the mesh data.
  for (uint32 i = 0; i < modelHeader->meshCount; ++i) {
    // get mesh header.
    MeshAssetHeader* mHeader = new MeshAssetHeader();
    // read vertices.
    file.read(reinterpret_cast<char*>(&mHeader->nameSize), sizeof(SIZE_T));
    mHeader->name.resize(mHeader->nameSize);
    file.read(reinterpret_cast<char*>(&mHeader->name[0]), mHeader->nameSize);
    file.read(reinterpret_cast<char*>(&mHeader->vertexCount), sizeof(uint32));
    file.read(reinterpret_cast<char*>(&mHeader->indexCount), sizeof(uint32));


    // get vertex data.
    uint32 meshVerticesSize = sizeof(SimpleVertex) * mHeader->vertexCount;
    Vector<char> meshVertices(meshVerticesSize);
    file.read(meshVertices.data(), meshVerticesSize);
    Vector<SimpleVertex> vertices = Vector<SimpleVertex>(mHeader->vertexCount);
    memcpy(vertices.data(), meshVertices.data(), meshVerticesSize);

    // get index data.
      uint32 meshIndicesSize = sizeof(uint32) * mHeader->indexCount;
    Vector<char> meshIndices(meshIndicesSize);
    file.read(meshIndices.data(), meshIndicesSize);
    Vector<uint32> indices = Vector<uint32>(mHeader->indexCount);
    memcpy(indices.data(), meshIndices.data(), meshIndicesSize);

    // create the mesh
    SPtr<Mesh> mesh = make_shared<Mesh>();
    // set mesh data.
    mesh->setName(mHeader->name); // to do: temporary placeholder for the mesh name.
    mesh->vertexCount = mHeader->vertexCount;
    mesh->numIndex = mHeader->indexCount;
    // fill in mesh data.
    mesh->vertexVector.resize(mHeader->vertexCount);
    memcpy(mesh->vertexVector.data(), vertices.data(), meshVerticesSize);
    mesh->indexVector.resize(mHeader->indexCount);
    memcpy(mesh->indexVector.data(), indices.data(), meshIndicesSize);

    // to do: make a default material
    mesh->material = GPUResourceMan.m_defaultMaterial;

    model->meshes[i] = mesh;

    delete mHeader;
    mHeader = nullptr;
  }

  model->setVerticesIndices();
  // ModelResource res;
  
  file.close();
  return model;
}

SPtr<Model>
GPUResourceManager::loadAssimpModel(Path _path)
{
  SPtr<Model> model = make_shared<Model>();

  String modelPath = _path.toString();
  model->path = _path;
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(modelPath.c_str(),
                                           aiProcessPreset_TargetRealtime_MaxQuality |
                                           aiProcess_RemoveRedundantMaterials |
                                           aiProcess_FlipUVs);
  if (!scene) {
    g_Logger().registerMessage("Failed to load model at directory " + modelPath + ".",
                               LOG_MSG_TYPE::kWarning);
    return nullptr;
  }
  model->setName(scene->mName.C_Str());
  processNode(*model, scene->mRootNode, scene);
  model->setVerticesIndices();
  return model;
}

SPtr<Mesh>
GPUResourceManager::searchMesh(const String _name)
{
  for (uint32 i = 0; i < m_meshes.size(); ++i) {
    SPtr<Mesh> mesh = m_meshes[i];
    if (_name == mesh->getName()) {
      return mesh;
    }
  }
  return nullptr;
}


void
processNode(Model& _model, aiNode* _node, const aiScene* _scene)
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
processMesh(aiMesh* _mesh, const aiScene* _scene, const Matrix4 _transform)
{
  // modules
  GPUResourceManager& rm = g_GPUResourceManager();
  TextureManager& tm = g_TextureManager();
  Logger& log = g_Logger();

  // check if the mesh is already in storage
  String meshName(_mesh->mName.C_Str());
  SPtr<Mesh> meshProcess = rm.searchMesh(meshName);
  // if a mesh can be found
  if (meshProcess) {
    log.registerMessage("Found pre-loaded mesh of name " + meshName + ".");
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
  meshProcess->material = rm.m_defaultMaterial;
  if (_mesh->mMaterialIndex >= 0) {
    // material data
    aiMaterial* materialA = _scene->mMaterials[_mesh->mMaterialIndex];
    String matName = materialA->GetName().C_Str();
    meshProcess->material = rm.newMaterial(matName);
    // material 
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
      Transform transform(Matrix4::IDENTITY,
        Matrix4::IDENTITY,
        aIMatrixToMatrix(_mesh->mBones[i]->mOffsetMatrix));
      newBone.setLocalTransform(transform);
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

PK_CORE_EXPORT GPUResourceManager&
g_GPUResourceManager()
{
  return GPUResourceManager::instance();
}
}
