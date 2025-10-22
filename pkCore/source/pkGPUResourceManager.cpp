#include "pkLogger.h"
#include "pkGPUResourceManager.h"
#include "pkGraphicsAPI.h"
#include "pkModel.h"
#include "pkModelResource.h"
#include "pkTexture.h"
#include "pkTextureManager.h"

namespace pkEngineSDK
{

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

PK_CORE_EXPORT GPUResourceManager&
g_GPUResourceManager()
{
  return GPUResourceManager::instance();
}
}
