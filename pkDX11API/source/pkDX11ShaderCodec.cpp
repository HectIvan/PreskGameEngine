/*****************************************************************************/
/**
 * @file    pkDX11ShaderCodec.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    07/11/2025
 * @brief   Codec for DirectX compiled shaders.
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
#include "pkDX11ShaderCodec.h"
#include "pkDX11Shader.h"
#include "pkLogger.h"
#include "pkPrerequisitesCore.h"
#include "pkShaderResource.h"
#include "pkUUID.h"

namespace pkEngineSDK
{

SPtr<BaseResource>
DX11ShaderCodec::createResourceFromShader(const SPtr<Shader>& _pShader)
{
  Logger& log = g_Logger();
  
  // check if the shader is not null.
  if (!_pShader) {
    const String msg = "Shader is null!";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }

  auto shader = reinterpret_pointer_cast<DX11Shader>(_pShader);

  // check if the DirectX shader is valid.
  Path shaderDir = _pShader->getShaderDirectory();
  if (!shader) {
    const String msg = "Failed to reinterpret a shader " +
                       shaderDir.toString() +
                       " into a DirectX shader.";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }

  // attempt to create the shader resource.
  const String shaderName = shaderDir.getFileNameWithoutExtension();
  const String resourceDir = "resources/" + shaderName + ".pks";
  ofstream file(resourceDir, ios::out | ios::binary);

  // check if the resource creation failed.
  if (!file.is_open()) {
    const String msg = "Failed to generate the shader resource: " + resourceDir + ".";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }

  SPtr<ShaderResource> resource = make_shared<ShaderResource>();

  resource->m_id = UUID::generateRandomUUIDFromString(shaderName + " Shader");
  resource->m_name = shaderName;
  resource->m_isLoaded = true;
  resource->m_originalPath = shaderDir.toString();
  resource->m_resourcePath = resourceDir;

  resource->writeBaseHeader(file, resource->m_id, shaderName, resourceDir);

  resource->m_data = Vector<char>(static_cast<const char*>(shader->m_pSBlob->GetBufferPointer()),
                                  static_cast<const char*>(shader->m_pSBlob->GetBufferPointer()) +
                                  shader->m_pSBlob->GetBufferSize() );

  const SIZE_T fileSize = shader->m_pSBlob->GetBufferSize();
  file.write(reinterpret_cast<const char*>(&fileSize), sizeof(SIZE_T));
  // write the shader blob data into the resource.
  file.write(reinterpret_cast<const char*>(shader->m_pSBlob->GetBufferPointer()), fileSize);

  file.close();

  // register the creation.
  const String msg = "Created shader resource " +
                     resourceDir +
                     " of size " +
                     to_string(fileSize);
  log.registerMessage(msg, __FILE__, __LINE__);

  return resource;
}
}