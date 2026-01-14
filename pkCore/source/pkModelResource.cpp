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

  ifstream file(m_resourcePath, ios::in | ios::binary);

  // if the direcory cannot be opened, return a warning and a nullptr.
  if (!file.is_open()) {
    const String msg = "Failed to open a model resource at directory " +
                       String(m_resourcePath) +
                       ".";
    LOG_WARNING(msg, __FILE__, __LINE__);
    return;
  }

  // load the base resource header data into the resource.
  loadBaseHeader(file);

  // read the model header.
  ModelAssetHeader modelHeader;
  file.read(reinterpret_cast<ANSICHAR*>(&modelHeader.meshCount), sizeof(uint32));
  m_meshes.resize(modelHeader.meshCount);

  // read through each mesh in the model.
  for (uint32 i = 0; i < modelHeader.meshCount; ++i) {
    MeshAssetHeader meshHeader;
    // create the mesh
    SPtr<Mesh> mesh = make_shared<Mesh>();
    file.read(reinterpret_cast<ANSICHAR*>(&meshHeader.name), PK_RESOURCE_NAME_SIZE);
    file.read(reinterpret_cast<ANSICHAR*>(&meshHeader.vertexCount), sizeof(uint32));
    file.read(reinterpret_cast<ANSICHAR*>(&meshHeader.indexCount), sizeof(uint32));
    file.read(reinterpret_cast<ANSICHAR*>(&mesh->m_transform), sizeof(Matrix4));

    uint8 isActiveRaw = 0;
    file.read(reinterpret_cast<ANSICHAR*>(&isActiveRaw), sizeof(uint8));
    mesh->setActive(isActiveRaw ? 1 : 0);

    // get vertices data
    uint32 meshVerticesSize = sizeof(SimpleVertex) * meshHeader.vertexCount;
    Vector<ANSICHAR> meshVertices(meshVerticesSize);
    file.read(meshVertices.data(), meshVerticesSize);
    Vector<SimpleVertex> vertices = Vector<SimpleVertex>(meshHeader.vertexCount);
    memcpy(vertices.data(), meshVertices.data(), meshVerticesSize);

    // get indices data.
    uint32 meshIndicesSize = sizeof(uint32) * meshHeader.indexCount;
    Vector<ANSICHAR> meshIndices(meshIndicesSize);
    file.read(meshIndices.data(), meshIndicesSize);
    Vector<uint32> indices = Vector<uint32>(meshHeader.indexCount);
    memcpy(indices.data(), meshIndices.data(), meshIndicesSize);

    // set mesh data.
    mesh->setName(meshHeader.name);
    mesh->vertexCount = meshHeader.vertexCount;
    mesh->numIndex = meshHeader.indexCount;
    // fill in mesh data.
    mesh->vertexVector.resize(meshHeader.vertexCount);
    memcpy(mesh->vertexVector.data(), vertices.data(), meshVerticesSize);
    mesh->indexVector.resize(meshHeader.indexCount);
    memcpy(mesh->indexVector.data(), indices.data(), meshIndicesSize);

    UUID matID;
    file.read(reinterpret_cast<ANSICHAR*>(&matID), sizeof(UUID));

    mesh->material = materialMan.loadMaterial(matID);
    if (!mesh->material) {
      mesh->material = materialMan.m_defaultMaterial;
    }

    m_meshes[i] = mesh;
  }

  file.close();

  m_isLoaded = true;

  return;
}

void
ModelResource::unload()
{
  m_isLoaded = false;
  m_vertex.clear();
  m_index.clear();

  const uint32 meshCount = static_cast<uint32>(m_meshes.size());
  for (uint32 i = 0; i < meshCount; ++i) {
    m_meshes[i]->clean();
  }

  m_meshes.clear();
}
}
