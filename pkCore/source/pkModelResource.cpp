#include "pkModelResource.h"
#include "pkGPUResourceManager.h"
#include "pkLogger.h"

namespace pkEngineSDK
{

void
ModelResource::load()
{
  GPUResourceManager& GPUResourceMan = g_GPUResourceManager();
  Logger& log = g_Logger();

  String path = m_resourcePath.toString();
  ifstream file(path, ios::in | ios::binary);

  // if the direcory cannot be opened, return a warning and a nullptr.
  if (!file.is_open()) {
    String msg = "Failed to open a model resource at directory " + path + ".";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    return;
  }

  // load the base resource header data into the resource.
  loadBaseHeader(file);

  // read the model header.
  ModelAssetHeader modelHeader;
  file.read(reinterpret_cast<char*>(&modelHeader.meshCount), sizeof(uint32));
  m_meshes.resize(modelHeader.meshCount);

  // read through each mesh in the model.
  for (uint32 i = 0; i < modelHeader.meshCount; ++i) {
    MeshAssetHeader meshHeader;
    file.read(reinterpret_cast<char*>(&meshHeader.nameSize), sizeof(SIZE_T));
    meshHeader.name.resize(meshHeader.nameSize);
    file.read(reinterpret_cast<char*>(&meshHeader.name[0]), meshHeader.nameSize);
    file.read(reinterpret_cast<char*>(&meshHeader.vertexCount), sizeof(uint32));
    file.read(reinterpret_cast<char*>(&meshHeader.indexCount), sizeof(uint32));

    // get vertices data
    uint32 meshVerticesSize = sizeof(SimpleVertex) * meshHeader.vertexCount;
    Vector<char> meshVertices(meshVerticesSize);
    file.read(meshVertices.data(), meshVerticesSize);
    Vector<SimpleVertex> vertices = Vector<SimpleVertex>(meshHeader.vertexCount);
    memcpy(vertices.data(), meshVertices.data(), meshVerticesSize);

    // get indices data.
    uint32 meshIndicesSize = sizeof(uint32) * meshHeader.indexCount;
    Vector<char> meshIndices(meshIndicesSize);
    file.read(meshIndices.data(), meshIndicesSize);
    Vector<uint32> indices = Vector<uint32>(meshHeader.indexCount);
    memcpy(indices.data(), meshIndices.data(), meshIndicesSize);

    // create the mesh
    SPtr<Mesh> mesh = make_shared<Mesh>();
    // set mesh data.
    mesh->setName(meshHeader.name); // to do: temporary placeholder for the mesh name.
    mesh->vertexCount = meshHeader.vertexCount;
    mesh->numIndex = meshHeader.indexCount;
    // fill in mesh data.
    mesh->vertexVector.resize(meshHeader.vertexCount);
    memcpy(mesh->vertexVector.data(), vertices.data(), meshVerticesSize);
    mesh->indexVector.resize(meshHeader.indexCount);
    memcpy(mesh->indexVector.data(), indices.data(), meshIndicesSize);

    // to do: make a default material
    mesh->material = GPUResourceMan.m_defaultMaterial;
    m_meshes[i] = mesh;
  }

  return;
}
}
