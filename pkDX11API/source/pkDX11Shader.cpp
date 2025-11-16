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
  Logger& log = g_Logger();
  // if the resource is not a shader resource.
  if (RESOURCE_TYPE::kShader != _pResource->getType()) {
    return;
  }

  // reinterpret to a shader resource.
  auto resource = reinterpret_pointer_cast<ShaderResource>(_pResource);

  // load the shader data.
  resource->load();
  // check if there is data in the shader
  if (resource->m_data.empty()) {
    const String msg = "Data from resource " + _pResource->m_name + " is empty.";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kWarning);
    return;
  }

  // copy the new data into the blob
  const SIZE_T blobSize = resource->m_data.size();
  m_pSBlob = new PKBlob(resource->m_data);

  const String blobSizeString = to_string(blobSize);

  // if the blob failed to be created.
  if (!m_pSBlob) {
    const String msg = "Failed to create sBlob of size " + blobSizeString + ".";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    return;
  }

  // check if the data transfer was succesful.
  if (!m_pSBlob) {
    const String msg = "Failed to create a shader blob of size " +
                       blobSizeString +
                       " from the shader resource " +
                       resource->m_resourcePath +
                       ".";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    return;
  }

  const String msg = "Created shader from resource " + resource->m_resourcePath + ".";
  log.registerMessage(msg, __FILE__, __LINE__);
}

void
DX11Shader::setData(const Path _directory, const char* _entry, const char* _sModel)
{
  m_shaderDirectory = _directory;
  m_sEntryPoint = _entry;
  m_sModel = _sModel;
}
}