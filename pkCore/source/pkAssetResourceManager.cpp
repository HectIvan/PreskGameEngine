/*****************************************************************************/
/**
 * @file    pkAssetResourceManager.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    18/09/2025
 * @brief   Definitions for the functions of the Asset Resource Manager used for the engine.
 *
 * @bug    No known bugs.
 *
 * @changes Created the file.
 */
 /*****************************************************************************/
#include "pkAssetResourceManager.h"
#include "pkTextureResource.h"
#include "pkPath.h"
#include "pkLogger.h"
#include "pkPrerequisitesCore.h"
#include "pkModelResource.h"
#include "pkGPUResourceManager.h"

namespace pkEngineSDK
{
void
AssetResourceManager::init()
{
  // To do: implement the asset resource manager.
}

SPtr<BaseResource>
AssetResourceManager::loadResource()
{
  return SPtr<BaseResource>();
}

SPtr<BaseResource>
AssetResourceManager::unloadResource()
{
  return SPtr<BaseResource>();
}

ModelResource*
AssetResourceManager::loadModelResource(const Path _path)
{
  GPUResourceManager& GPUResourceMan = g_GPUResourceManager();
  Logger& log = g_Logger();

  ifstream file(_path.toString(), ios::in | ios::binary);

  // if the direcory cannot be opened, return a warning and a nullptr.
  if (!file.is_open()) {
    String msg = "Failed to open model resource at directory " + _path.toString() + ".";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }

  ModelResource* modelResource = new ModelResource();

  // read the model header.
  ModelAssetHeader* modelHeader = new ModelAssetHeader();
  file.read(reinterpret_cast<char*>(&modelHeader->meshCount), sizeof(uint32));
  modelResource->m_meshes.resize(modelHeader->meshCount);

  // read through each mesh in the model.
  for (uint32 i = 0; i < modelHeader->meshCount; ++i) {
    MeshAssetHeader* meshHeader = new MeshAssetHeader();
    file.read(reinterpret_cast<char*>(&meshHeader->nameSize), sizeof(SIZE_T));
    meshHeader->name.resize(meshHeader->nameSize);
    file.read(reinterpret_cast<char*>(&meshHeader->name[0]), meshHeader->nameSize);
    file.read(reinterpret_cast<char*>(&meshHeader->vertexCount), sizeof(uint32));
    file.read(reinterpret_cast<char*>(&meshHeader->indexCount), sizeof(uint32));

    // get vertices data
    uint32 meshVerticesSize = sizeof(SimpleVertex) * meshHeader->vertexCount;
    Vector<char> meshVertices(meshVerticesSize);
    file.read(meshVertices.data(), meshVerticesSize);
    Vector<SimpleVertex> vertices = Vector<SimpleVertex>(meshHeader->vertexCount);
    memcpy(vertices.data(), meshVertices.data(), meshVerticesSize);

    // get indices data.
    uint32 meshIndicesSize = sizeof(uint32) * meshHeader->indexCount;
    Vector<char> meshIndices(meshIndicesSize);
    file.read(meshIndices.data(), meshIndicesSize);
    Vector<uint32> indices = Vector<uint32>(meshHeader->indexCount);
    memcpy(indices.data(), meshIndices.data(), meshIndicesSize);

    // create the mesh
    SPtr<Mesh> mesh = make_shared<Mesh>();
    // set mesh data.
    mesh->setName(meshHeader->name); // to do: temporary placeholder for the mesh name.
    mesh->vertexCount = meshHeader->vertexCount;
    mesh->numIndex = meshHeader->indexCount;
    // fill in mesh data.
    mesh->vertexVector.resize(meshHeader->vertexCount);
    memcpy(mesh->vertexVector.data(), vertices.data(), meshVerticesSize);
    mesh->indexVector.resize(meshHeader->indexCount);
    memcpy(mesh->indexVector.data(), indices.data(), meshIndicesSize);

    // to do: make a default material
    mesh->material = GPUResourceMan.m_defaultMaterial;
    modelResource->m_meshes[i] = mesh;
  }

  delete modelHeader;
  modelHeader = nullptr;

  return modelResource;
}

TextureResource*
AssetResourceManager::loadTextureResource(const Path _path)
{
  Logger& log = g_Logger();

  ifstream file(_path.toString(), ios::in | ios::binary);

  // if the direcory cannot be opened, return a warning and a nullptr.
  if (!file.is_open()) {
    String msg = "Failed to open texture resource at directory " + _path.toString() + ".";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }

  TextureResource* texResource = new TextureResource();

  TextureAssetHeader* texHeader = new TextureAssetHeader();
  file.read(reinterpret_cast<char*>(&texHeader->width), sizeof(int32));
  file.read(reinterpret_cast<char*>(&texHeader->height), sizeof(int32));
  file.read(reinterpret_cast<char*>(&texHeader->bpp), sizeof(int32));
  file.read(reinterpret_cast<char*>(&texHeader->format), sizeof(uint32));
  file.read(reinterpret_cast<char*>(&texHeader->dataSize), sizeof(uint32));

  texResource->m_width = texHeader->width;
  texResource->m_height = texHeader->height;
  texResource->m_bpp = texHeader->bpp;
  texResource->m_format = texHeader->format;

  texResource->m_data = new unsigned char[texHeader->dataSize];
  file.read(reinterpret_cast<char*>(&texResource->m_data[0]), texHeader->dataSize);

  delete texHeader;
  texHeader = nullptr;

  return texResource;
}

PK_CORE_EXPORT AssetResourceManager&
g_AssetResourceManager()
{
  return AssetResourceManager::instance();
}
}