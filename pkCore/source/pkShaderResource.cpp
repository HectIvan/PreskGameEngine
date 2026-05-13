/*****************************************************************************/
/**
 * @file    pkShaderResource.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    07/11/2025
 * @brief   Resource for compiled shaders.
 *
 * @bug    No known bugs.
 *
 * @changes Created the file.
 */
 /*****************************************************************************/

 /*********************************************/
/**
 * Includes
 */
 /*********************************************/
#include "pkShaderResource.h"
#include "pkPrerequisitesCore.h"
#include "pkLogger.h"

namespace pkEngineSDK
{

void
ShaderResource::load()
{
  if (m_isLoaded) {
    return;
  }

  ifstream file(m_resourcePath, ios::in | ios::binary);

  // check if the resource could be opened.
  if (!file.is_open()) {
    const String msg = "Failed to load resource at path: " + String(m_resourcePath) + ".";
    LOG_ERROR(msg, __FILE__, __LINE__);
    return;
  }
  // load the base resource header.
  loadBaseHeader(file);

  // read the shader type.
  file.read(reinterpret_cast<ANSICHAR*>(&m_shaderDirectory), PK_RESOURCE_PATH_SIZE);
  file.read(reinterpret_cast<ANSICHAR*>(&m_sEntryPoint), PK_RESOURCE_NAME_SIZE);
  file.read(reinterpret_cast<ANSICHAR*>(&m_sModel), PK_RESOURCE_NAME_SIZE);
  file.read(reinterpret_cast<ANSICHAR*>(&m_type), sizeof(PK_SHADER_TYPE::E));
  // load the shader blob.
  SIZE_T blobSize = 0;
  file.read(reinterpret_cast<ANSICHAR*>(&blobSize), sizeof(SIZE_T));
  m_data.resize(blobSize);
  file.read(reinterpret_cast<ANSICHAR*>(m_data.data()), blobSize);
  file.close();

  // register the action.
  const String msg = "Loaded shader resource " + String(m_resourcePath) + ".";
  LOG_REGISTER(msg, __FILE__, __LINE__);
  m_isLoaded = true;
}

void
ShaderResource::unload()
{
  m_isLoaded = false;
  m_data.clear();
}

UUID
ShaderResource::generateID(const ANSICHAR* _shaderDirectory,
                           const ANSICHAR* _sEntryPoint,
                           const ANSICHAR* _sModel)
{
  const String IDSeed = String(_shaderDirectory) + String(_sEntryPoint) + String(_sModel);
  return UUID::generateRandomUUIDFromString(IDSeed);
}
}
