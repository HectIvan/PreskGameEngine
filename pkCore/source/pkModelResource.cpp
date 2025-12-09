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

  AssetResourceManager& assetMan = g_AssetResourceManager();
  MaterialManager& matMan = g_MaterialManager();
  Logger& log = g_Logger();

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
    // create the mesh
    SPtr<Mesh> mesh = make_shared<Mesh>();
    MeshAssetHeader meshHeader;

    file.read(reinterpret_cast<ANSICHAR*>(&meshHeader.nameSize), sizeof(SIZE_T));
    meshHeader.name.resize(meshHeader.nameSize);
    file.read(reinterpret_cast<ANSICHAR*>(&meshHeader.name[0]), meshHeader.nameSize);
    file.read(reinterpret_cast<ANSICHAR*>(&meshHeader.vertexCount), sizeof(uint32));
    file.read(reinterpret_cast<ANSICHAR*>(&meshHeader.indexCount), sizeof(uint32));
    file.read(reinterpret_cast<ANSICHAR*>(&mesh->m_transform), sizeof(Matrix4));
    mesh->setName(meshHeader.name);
    // get mesh activity.
    uint8 isActiveRaw;
    file.read(reinterpret_cast<ANSICHAR*>(&isActiveRaw), sizeof(uint8));
    bool isActive = (isActiveRaw != 0);
    mesh->setActive(isActive);

    // get vertices data.
    const uint32 meshVerticesSize = sizeof(SimpleVertex) * meshHeader.vertexCount;
    Vector<ANSICHAR> meshVertices(meshVerticesSize);
    file.read(meshVertices.data(), meshVerticesSize);
    Vector<SimpleVertex> vertices = Vector<SimpleVertex>(meshHeader.vertexCount);
    memcpy(vertices.data(), meshVertices.data(), meshVerticesSize);

    // get indices data.
    const uint32 meshIndicesSize = sizeof(uint32) * meshHeader.indexCount;
    Vector<ANSICHAR> meshIndices(meshIndicesSize);
    file.read(meshIndices.data(), meshIndicesSize);
    Vector<uint32> indices = Vector<uint32>(meshHeader.indexCount);
    memcpy(indices.data(), meshIndices.data(), meshIndicesSize);

    // set mesh data.
    mesh->vertexCount = meshHeader.vertexCount;
    mesh->numIndex = meshHeader.indexCount;
    // fill in mesh data.
    mesh->vertexVector.resize(meshHeader.vertexCount);
    memcpy(mesh->vertexVector.data(), vertices.data(), meshVerticesSize);
    mesh->indexVector.resize(meshHeader.indexCount);
    memcpy(mesh->indexVector.data(), indices.data(), meshIndicesSize);

    SIZE_T matIDSize;
    String matID;
    file.read(reinterpret_cast<ANSICHAR*>(&matIDSize), sizeof(SIZE_T));
    matID.resize(matIDSize);
    file.read(reinterpret_cast<ANSICHAR*>(&matID[0]), matIDSize);

    // to do: make a default material
    mesh->material = matMan.m_defaultMaterial;
    SPtr<BaseResource> matRes = assetMan.loadResource(matID);
    if (matRes) {
      mesh->material = matMan.loadMaterial(matRes->m_id);
    }
    m_meshes[i] = mesh;
  }

  file.close();

  m_isLoaded = true;

  return;
}
}
