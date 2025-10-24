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
#include "pkModel.h"
#include "pkModelCodec.h"
#include "pkPath.h"
#include "pkLogger.h"
#include "pkModelResource.h"

namespace pkEngineSDK
{

bool
ModelCodec::savePKModel(const SPtr<Model>& _pModel, const Path _path)
{
  Logger& log = g_Logger();
  String filePath = "resources/" + _path.getFileName() + ".pkm";
  ofstream file(filePath, ios::out | ios::binary);
  // char error[256];
  // strerror_s(error, sizeof(error), errno); // last error of io.
  if (!file.is_open()) {
    String msg = "Failed to save model at path " + filePath + ".";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    return false;
  }

  // get and write model header.
  ModelAssetHeader* mHeader = new ModelAssetHeader();
  uint32 meshCount = static_cast<uint32>(_pModel->meshes.size());
  mHeader->meshCount = meshCount;
  file.write(reinterpret_cast<const char*>(&mHeader), sizeof(ModelAssetHeader));
  // for each mesh in the model.
  for (uint32 i = 0; i < meshCount; ++i) {
    // aquire needed objects
    SPtr<Mesh> mesh = _pModel->meshes[i];
    MeshAssetHeader* meshHeader = new MeshAssetHeader();
    uint32 indicesCount = static_cast<uint32>(mesh->indexVector.size());
    uint32 verticesCount = static_cast<uint32>(mesh->vertexVector.size());
    String name = mesh->getName();
    SIZE_T nameSize = name.length();
    meshHeader->indexCount = indicesCount;
    meshHeader->vertexCount = verticesCount;
    meshHeader->nameSize = nameSize;
    meshHeader->name = name;
    // write the mesh header.
    file.write(reinterpret_cast<const char*>(&meshHeader->nameSize), sizeof(SIZE_T));
    file.write(reinterpret_cast<const char*>(meshHeader->name.c_str()), nameSize);
    file.write(reinterpret_cast<const char*>(&meshHeader->vertexCount), sizeof(uint32));
    file.write(reinterpret_cast<const char*>(&meshHeader->indexCount), sizeof(uint32));

    // write all vertices of the mesh.
    file.write(reinterpret_cast<const char*>(mesh->vertexVector.data()),
               sizeof(SimpleVertex) * verticesCount);
    // write all indices of the mesh.
    file.write(reinterpret_cast<const char*>(mesh->indexVector.data()),
               sizeof(uint32) * indicesCount);
  }

  file.close();
  return true;
}

PK_CORE_EXPORT ModelCodec&
g_ModelCodec()
{
  return ModelCodec::instance();
}
}
