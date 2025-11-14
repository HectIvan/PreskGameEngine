/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11Shader.h"
#include "pkGraphicsAPI.h"
#include "pkShaderResource.h"
#include "pkLogger.h"

#include <d3dcompiler.h>

namespace pkEngineSDK
{

void
DX11Shader::compileFromFile()
{
  GraphicsAPI& api = g_GraphicAPI();
  m_pSBlob = reinterpret_cast<ID3DBlob*>(api.compileShaderFromFile(m_shaderDirectory,
                                                                   m_sEntryPoint,
                                                                   m_sModel));
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
  m_pSBlob = reinterpret_cast<ID3DBlob*>(resource->m_data.data());

  // check if the data transfer was succesful.
  if (!m_pSBlob) {
    const String msg = "Failed to create a shader blob from the shader resource " +
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