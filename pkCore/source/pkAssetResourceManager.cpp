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
#include "pkGPUResourceManager.h"
#include "pkLogger.h"
#include "pkMaterialResource.h"
#include "pkModelResource.h"
#include "pkPath.h"
#include "pkPrerequisitesCore.h"
#include "pkTextureResource.h"
#include "pkFileSystem.h"

namespace pkEngineSDK
{
void
AssetResourceManager::init(const String _resourcesPath)
{
  m_resourcesFolder = Path(_resourcesPath);
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

SPtr<ModelResource>
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

  SPtr<ModelResource> modelResource = make_shared<ModelResource>();

  // read the model header.
  ModelAssetHeader modelHeader;
  file.read(reinterpret_cast<char*>(&modelHeader.meshCount), sizeof(uint32));
  modelResource->m_meshes.resize(modelHeader.meshCount);

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
    modelResource->m_meshes[i] = mesh;
  }

  return modelResource;
}

SPtr<TextureResource>
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

  SPtr<TextureResource> texResource = make_shared<TextureResource>();

  TextureAssetHeader texHeader;
  file.read(reinterpret_cast<char*>(&texHeader.width), sizeof(int32));
  file.read(reinterpret_cast<char*>(&texHeader.height), sizeof(int32));
  file.read(reinterpret_cast<char*>(&texHeader.bpp), sizeof(int32));
  file.read(reinterpret_cast<char*>(&texHeader.format), sizeof(uint32));
  file.read(reinterpret_cast<char*>(&texHeader.dataSize), sizeof(uint32));

  texResource->m_width = texHeader.width;
  texResource->m_height = texHeader.height;
  texResource->m_bpp = texHeader.bpp;
  texResource->m_format = texHeader.format;

  texResource->m_data = new unsigned char[texHeader.dataSize];
  file.read(reinterpret_cast<char*>(&texResource->m_data[0]), texHeader.dataSize);

  return texResource;
}

SPtr<MaterialResource>
AssetResourceManager::loadMaterialResource(const Path _path)
{
  Logger& log = g_Logger();
  ifstream file(_path.toString(), ios::in | ios::binary);
  // if the direcory cannot be opened, return a warning and a nullptr.
  if (!file.is_open()) {
    String msg = "Failed to open material resource at directory " + _path.toString() + ".";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }

  SPtr<MaterialResource> matResource = make_shared<MaterialResource>();
  MaterialAssetHeader matHeader;

  // read diffuse data
  file.read(reinterpret_cast<char*>(&matHeader.diffusePathSize), sizeof(SIZE_T));
  matResource->m_diffusePath.resize(matHeader.diffusePathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_diffusePath[0]),
                                    matHeader.diffusePathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_diffuseColor), sizeof(Color));

  // read normal data
  file.read(reinterpret_cast<char*>(&matHeader.normalPathSize), sizeof(SIZE_T));
  matResource->m_normalPath.resize(matHeader.normalPathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_normalPath[0]), matHeader.normalPathSize);

  // read ao data
  file.read(reinterpret_cast<char*>(&matHeader.aoPathSize), sizeof(SIZE_T));
  matResource->m_aoPath.resize(matHeader.aoPathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_aoPath[0]), matHeader.aoPathSize);

  // read roughness data
  file.read(reinterpret_cast<char*>(&matHeader.roughnessPathSize), sizeof(SIZE_T));
  matResource->m_roughnessPath.resize(matHeader.roughnessPathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_roughnessPath[0]),
                                    matHeader.roughnessPathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_roughValue), sizeof(float));

  // read metallic data
  file.read(reinterpret_cast<char*>(&matHeader.metallicPathSize), sizeof(SIZE_T));
  matResource->m_metallicPath.resize(matHeader.metallicPathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_metallicPath[0]),
                                    matHeader.metallicPathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_metallicValue), sizeof(float));

  // read emissive data
  file.read(reinterpret_cast<char*>(&matHeader.emissivePathSize), sizeof(SIZE_T));
  matResource->m_emissivePath.resize(matHeader.emissivePathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_emissivePath[0]),
                                    matHeader.emissivePathSize);
  file.read(reinterpret_cast<char*>(&matResource->m_emissiveColor), sizeof(Color));

  return matResource;
}

void
AssetResourceManager::loadAssetsFromResourcesFolder()
{
  m_allResources.clear();
  Vector<Path> paths = FileSystem::getFilesFromFolder(m_resourcesFolder);
  uint32 pathCount = static_cast<uint32>(paths.size());

  // iterate through all found resources.
  for (uint32 i = 0; i < pathCount; ++i) {
    Path path = paths[i];
    // check if the file is a PK resource.
    if (isPKResource(path)) {
      // SPtr<BaseResource> resource = make_shared<BaseResource>();
      // resource->m_resourcePath = path;
      // resource->m_name = path.getFileName();
      m_allResources.insert({ i, path });
    }
  }
}

bool
AssetResourceManager::isPKResource(const Path _path)
{
  String extension = _path.getExtension();
  if (extension == "pkm" || extension == "pkt" || extension == "pkmat") {
    return true;
  }
  return false;
}

PK_CORE_EXPORT AssetResourceManager&
g_AssetResourceManager()
{
  return AssetResourceManager::instance();
}
}