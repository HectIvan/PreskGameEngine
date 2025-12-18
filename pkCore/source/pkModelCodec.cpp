/*****************************************************************************/
/**
 * @file    pkModelCodec.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    08/10/2025
 * @brief   Codec for loading models.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkAssetResourceManager.h"
#include "pkLogger.h"
#include "pkMaterialCodec.h"
#include "pkMaterialManager.h"
#include "pkModel.h"
#include "pkModelCodec.h"
#include "pkModelResource.h"
#include "pkPath.h"
#include "pkUUID.h"

namespace pkEngineSDK
{

SPtr<BaseResource>
ModelCodec::createResourceFromModel(const SPtr<Model>& _pModel, const Path& _path)
{
  AssetResourceManager& assetMan = g_AssetResourceManager();
  MaterialCodec& matCodec = g_MaterialCodec();
  MaterialManager& matMan = g_MaterialManager();

  const String fileName = _path.getFileNameWithoutExtension();
  const String filePath = "resources/" + fileName + ".pkm";
  ofstream file(filePath, ios::out | ios::binary | ios::trunc);
  // char error[256];
  // strerror_s(error, sizeof(error), errno); // last error of io.
  if (!file.is_open()) {
    const String msg = "Failed to save model at path " + filePath + ".";
    LOG_WARNING(msg, __FILE__, __LINE__);
    return nullptr;
  }

  /**
   * Create the model resource.
   */
  SPtr<ModelResource> modelRes = make_shared<ModelResource>();

  modelRes->fillBaseHeader(fileName + "Model", fileName, _path.toString(), filePath);
  modelRes->writeBaseHeader(file);

  modelRes->m_meshes = _pModel->meshes;
  modelRes->m_index = _pModel->index;
  modelRes->m_vertex = _pModel->vertex;

  // generate the base resource header.

  // get and write model header.
  ModelAssetHeader mHeader;
  const uint32 meshCount = static_cast<uint32>(_pModel->meshes.size());
  mHeader.meshCount = meshCount;
  file.write(reinterpret_cast<const ANSICHAR*>(&mHeader.meshCount), sizeof(uint32));
  // for each mesh in the model.
  for (uint32 i = 0; i < meshCount; ++i) {
    // aquire needed objects
    const SPtr<Mesh> mesh = _pModel->meshes[i];
    MeshAssetHeader meshHeader;
    const uint32 indicesCount = static_cast<uint32>(mesh->indexVector.size());
    const uint32 verticesCount = static_cast<uint32>(mesh->vertexVector.size());
    const String name = mesh->getName();
    meshHeader.indexCount = indicesCount;
    meshHeader.vertexCount = verticesCount;
    strcpy_s(meshHeader.name, PK_RESOURCE_NAME_SIZE, name.c_str());
    // write the mesh header.
    file.write(reinterpret_cast<const ANSICHAR*>(meshHeader.name), PK_RESOURCE_NAME_SIZE);
    file.write(reinterpret_cast<const ANSICHAR*>(&meshHeader.vertexCount), sizeof(uint32));
    file.write(reinterpret_cast<const ANSICHAR*>(&meshHeader.indexCount), sizeof(uint32));
    file.write(reinterpret_cast<const ANSICHAR*>(&mesh->m_transform), sizeof(Matrix4));
    // write mesh activity.
    const bool isActive = mesh->getActive();
    const uint8 isActiveRaw = isActive ? 1 : 0;
    file.write(reinterpret_cast<const ANSICHAR*>(&isActiveRaw), sizeof(uint8));
    // write all vertices of the mesh.
    file.write(reinterpret_cast<const ANSICHAR*>(mesh->vertexVector.data()),
               sizeof(SimpleVertex) * verticesCount);
    // write all indices of the mesh.
    file.write(reinterpret_cast<const ANSICHAR*>(mesh->indexVector.data()),
               sizeof(uint32) * indicesCount);

    // create the material resource
    SPtr<MaterialResource> matResource = matCodec.createResource(mesh->material);
    // to do: find a better way to do this
    file.write(reinterpret_cast<const ANSICHAR*>(&matResource->m_id), sizeof(UUID));

    assetMan.insertNewResource(matResource);
    matMan.insertLoadedMaterial(matResource->m_id, mesh->material);
  }
  file.close();

  return modelRes;
}

PK_CORE_EXPORT ModelCodec&
g_ModelCodec()
{
  return ModelCodec::instance();
}
}
