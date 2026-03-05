/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11Shader.h"
#include "pkGraphicsAPI.h"
#include "pkShaderResource.h"
#include "pkLogger.h"

namespace pkEngineSDK
{

void
DX11Shader::compileFromFile()
{
  m_pSBlob = g_GraphicAPI().compileShaderFromFile(m_shaderDirectory, m_sEntryPoint, m_sModel);
}

void
DX11Shader::compileFromResource(const SPtr<BaseResource>& _pResource) {
  // if the resource is not a shader resource.
  if (!_pResource || RESOURCE_TYPE::kShader != _pResource->getType()) {
    const String msg = "Resource is not a shader.";
    LOG_ERROR(msg, __FILE__, __LINE__);
    THROW_ERROR(msg);
    return;
  }

  // reinterpret to a shader resource.
  auto resource = reinterpret_pointer_cast<ShaderResource>(_pResource);

  // load the shader data.
  resource->load();
  // check if there is data in the shader
  if (resource->m_data.empty()) {
    const String msg = "Data from resource " + String(_pResource->m_name) + " is empty.";
    LOG_WARNING(msg, __FILE__, __LINE__);
    return;
  }

  // shader specific data.
  m_shaderDirectory = String(resource->m_shaderDirectory);
  m_sEntryPoint = resource->m_sEntryPoint;
  m_sModel = resource->m_sModel;
  m_shaderType = resource->m_type;

  // copy the new data into the blob
  const SIZE_T blobSize = resource->m_data.size();
  m_pSBlob = new PKBlob(resource->m_data);

  const String blobSizeString = to_string(blobSize);

  // if the blob failed to be created.
  if (!m_pSBlob) {
    const String msg = "Failed to create sBlob of size " + blobSizeString + ".";
    LOG_ERROR(msg, __FILE__, __LINE__);
    THROW_ERROR(msg);
    return;
  }

  // check if the data transfer was succesful.
  if (!m_pSBlob) {
    const String msg = "Failed to create a shader blob of size " +
                       blobSizeString +
                       " from the shader resource " +
                       resource->m_resourcePath +
                       ".";
    LOG_ERROR(msg, __FILE__, __LINE__);
    THROW_ERROR(msg);
    return;
  }

  const String msg = "Created shader from resource " + String(resource->m_resourcePath) + ".";
  LOG_REGISTER(msg, __FILE__, __LINE__);
}
}