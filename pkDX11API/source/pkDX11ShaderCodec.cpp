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
#include "pkAssetResourceManager.h"
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
  PK_ASSERT(_pShader);

  auto shader = reinterpret_pointer_cast<DX11Shader>(_pShader);

  // check if the DirectX shader is valid.
  const Path shaderPath = _pShader->getShaderDirectory();
  if (!shader) {
    const String msg = "Failed to reinterpret a shader " +
                       shaderPath.toString() +
                       " into a DirectX shader.";
    LOG_WARNING(msg, __FILE__, __LINE__);
    return nullptr;
  }

  // Necessary shader data.
  const String shaderName = shaderPath.getFileNameWithoutExtension();
  const String originalDirectory = shader->getShaderDirectory().toString();
  const String shaderEntry = shader->getEntryPoint();
  const String shaderModel = shader->getShaderModel();
  const String resourceDirectory = PK_RESOURCE_FOLDER + shaderName + ".pks";

  /* set the directory of the shader object to the directory of the resource,
     replacing the path based of the original shader file. */
  shader->setShaderDirectory(resourceDirectory);
  ofstream file(resourceDirectory, ios::out | ios::binary | ios::trunc);

  // check if the resource creation failed.
  if (!file.is_open()) {
    const String msg = "Failed to generate the shader resource: " + resourceDirectory + ".";
    LOG_WARNING(msg, __FILE__, __LINE__);
    return nullptr;
  }

  SPtr<ShaderResource> resource = make_shared<ShaderResource>();

  // What is used to generate a UUID for the shader resource.
  const String UUIDSource = shaderName + shaderEntry + shaderModel;

  resource->m_isLoaded = true;
  resource->fillBaseHeader(UUIDSource, shaderName, originalDirectory, resourceDirectory);
  resource->writeBaseHeader(file);

  strcpy_s(resource->m_shaderDirectory, PK_RESOURCE_PATH_SIZE, originalDirectory.c_str());
  strcpy_s(resource->m_sEntryPoint, PK_RESOURCE_NAME_SIZE, shaderEntry.c_str());
  strcpy_s(resource->m_sModel, PK_RESOURCE_NAME_SIZE, shaderModel.c_str());
  resource->m_type = shader->getType();

  // write shader specific data.
  file.write(reinterpret_cast<const ANSICHAR*>(&resource->m_shaderDirectory), PK_RESOURCE_PATH_SIZE);
  file.write(reinterpret_cast<const ANSICHAR*>(&resource->m_sEntryPoint), PK_RESOURCE_NAME_SIZE);
  file.write(reinterpret_cast<const ANSICHAR*>(&resource->m_sModel), PK_RESOURCE_NAME_SIZE);
  file.write(reinterpret_cast<const ANSICHAR*>(&resource->m_type), sizeof(PK_SHADER_TYPE::E));

  // write shader data.
  const void* pointer = shader->m_pSBlob.getBufferPointer();
  const SIZE_T pointerSize = shader->m_pSBlob.getBufferSize();
  resource->m_data = Vector<ANSICHAR>(static_cast<const ANSICHAR*>(pointer),
                                      static_cast<const ANSICHAR*>(pointer) +
                                      pointerSize );

  file.write(reinterpret_cast<const ANSICHAR*>(&pointerSize), sizeof(SIZE_T));
  // write the shader blob data into the resource.
  file.write(reinterpret_cast<const ANSICHAR*>(pointer), pointerSize);

  file.close();

  // register the creation.
  const String msg = "Created shader resource " +
                     shaderName + 
                     ".pks" +
                     " of size " +
                     to_string(pointerSize);
  LOG_REGISTER(msg, __FILE__, __LINE__);

  return resource;
}
}