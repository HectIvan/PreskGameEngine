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
#include "pkModel.h"
#include "pkModelCodec.h"
#include "pkModelResource.h"
#include "pkPath.h"
#include "pkUUID.h"

namespace pkEngineSDK
{

SPtr<BaseResource>
ModelCodec::createResourceFromModel(const SPtr<Model>& _pModel, const Path _path)
{
  Logger& log = g_Logger();
  AssetResourceManager& assetMan = g_AssetResourceManager();
  MaterialCodec& matCodec = g_MaterialCodec();

  const String fileName = _path.getFileNameWithoutExtension();
  const String filePath = "resources/" + fileName + ".pkm";
  ofstream file(filePath, ios::out | ios::binary);
  // char error[256];
  // strerror_s(error, sizeof(error), errno); // last error of io.
  if (!file.is_open()) {
    const String msg = "Failed to save model at path " + filePath + ".";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }

  /**
   * Create the model resource.
   */
  SPtr<ModelResource> modelRes = make_shared<ModelResource>();

  modelRes->m_originalPath = _path.toString();
  modelRes->m_resourcePath = filePath;
  modelRes->m_name = fileName;
  modelRes->m_id = UUID::generateRandomUUIDFromString(fileName + " Model");

  modelRes->writeBaseHeader(file, modelRes->m_id, fileName, modelRes->m_resourcePath);

  modelRes->m_meshes = _pModel->meshes;
  modelRes->m_index = _pModel->index;
  modelRes->m_vertex = _pModel->vertex;

  // generate the base resource header.

  // get and write model header.
  ModelAssetHeader mHeader;
  uint32 meshCount = static_cast<uint32>(_pModel->meshes.size());
  mHeader.meshCount = meshCount;
  file.write(reinterpret_cast<const char*>(&mHeader.meshCount), sizeof(uint32));
  // for each mesh in the model.
  for (uint32 i = 0; i < meshCount; ++i) {
    // aquire needed objects
    SPtr<Mesh> mesh = _pModel->meshes[i];
    MeshAssetHeader meshHeader;
    uint32 indicesCount = static_cast<uint32>(mesh->indexVector.size());
    uint32 verticesCount = static_cast<uint32>(mesh->vertexVector.size());
    String name = mesh->getName();
    SIZE_T nameSize = name.length();
    meshHeader.indexCount = indicesCount;
    meshHeader.vertexCount = verticesCount;
    meshHeader.nameSize = nameSize;
    meshHeader.name = name;
    // write the mesh header.
    file.write(reinterpret_cast<const char*>(&meshHeader.nameSize), sizeof(SIZE_T));
    file.write(reinterpret_cast<const char*>(meshHeader.name.c_str()), nameSize);
    file.write(reinterpret_cast<const char*>(&meshHeader.vertexCount), sizeof(uint32));
    file.write(reinterpret_cast<const char*>(&meshHeader.indexCount), sizeof(uint32));

    // write all vertices of the mesh.
    file.write(reinterpret_cast<const char*>(mesh->vertexVector.data()),
               sizeof(SimpleVertex) * verticesCount);
    // write all indices of the mesh.
    file.write(reinterpret_cast<const char*>(mesh->indexVector.data()),
               sizeof(uint32) * indicesCount);

    // create the material resource
    SPtr<MaterialResource> matResource = matCodec.createResource(mesh->material);
    assetMan.insertNewResource(matResource);

    // to do: find a better way to do this

    SIZE_T matIDSize = matResource->m_id.length();
    file.write(reinterpret_cast<const char*>(&matIDSize), sizeof(SIZE_T));
    file.write(reinterpret_cast<const char*>(matResource->m_id.c_str()), matIDSize);
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
