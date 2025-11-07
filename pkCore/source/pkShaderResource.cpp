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
  Logger& log = g_Logger();
  ifstream file(m_resourcePath, ios::in | ios::binary);

  // check if the resource could be opened.
  if (!file.is_open()) {
    const String msg = "Failed to open resource at path: " + m_resourcePath + ".";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    return;
  }

  SIZE_T blobSize = 0;
  file.read(reinterpret_cast<char*>(&blobSize), sizeof(SIZE_T));
  m_data.resize(blobSize);
  file.read(reinterpret_cast<char*>(m_data.data()), blobSize);

  file.close();
}
}
