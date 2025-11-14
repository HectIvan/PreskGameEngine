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

  Logger& log = g_Logger();
  ifstream file(m_resourcePath, ios::in | ios::binary);

  // check if the resource could be opened.
  if (!file.is_open()) {
    const String msg = "Failed to load resource at path: " + m_resourcePath + ".";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    return;
  }
  // load the base resource header.
  loadBaseHeader(file);
  // load the shader blob.
  SIZE_T blobSize = 0;
  file.read(reinterpret_cast<char*>(&blobSize), sizeof(SIZE_T));
  m_data.resize(blobSize);
  file.read(reinterpret_cast<char*>(m_data.data()), blobSize);
  file.close();

  // register the action.
  const String msg = "Loaded shader resource " + m_resourcePath + ".";
  log.registerMessage(msg, __FILE__, __LINE__);

  m_isLoaded = true;
}
}
