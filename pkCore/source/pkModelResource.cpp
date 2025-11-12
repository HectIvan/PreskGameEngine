#include "pkAssetResourceManager.h"
#include "pkGPUResourceManager.h"
#include "pkLogger.h"
#include "pkMaterialManager.h"
#include "pkModelResource.h"
#include "pkTextureManager.h"

namespace pkEngineSDK
{

void
ModelResource::load()
{
  if (m_isLoaded) {
    return;
  }

  MaterialManager& materialMan = g_MaterialManager();
  Logger& log = g_Logger();

  ifstream file(m_resourcePath, ios::in | ios::binary);

  // if the direcory cannot be opened, return a warning and a nullptr.
  if (!file.is_open()) {
    const String msg = "Failed to open a model resource at directory " + m_resourcePath + ".";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kWarning);
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

    SIZE_T matIDSize;
    String matID;
    file.read(reinterpret_cast<char*>(&matIDSize), sizeof(SIZE_T));
    matID.resize(matIDSize);
    file.read(reinterpret_cast<char*>(&matID[0]), matIDSize);

    // to do: make a default material
    mesh->material = materialMan.loadMaterial(matID);
    m_meshes[i] = mesh;
  }

  file.close();

  m_isLoaded = true;

  return;
}
}
