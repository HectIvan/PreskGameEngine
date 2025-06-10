/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11Shader.h"
#include "pkGraphicsAPI.h"

#include <d3dcompiler.h>

namespace pkEngineSDK
{

void
DX11Shader::compile()
{
  GraphicsAPI& api = g_GraphicAPI().instance();
  m_pSBlob = reinterpret_cast<ID3DBlob*>(api.compileShaderFromFile(m_shaderDirectory,
                                                                   m_sEntryPoint,
                                                                   m_sModel));
}

void
DX11Shader::setData(const WString _directory, const char* _entry, const char* _sModel)
{
  m_shaderDirectory = _directory;
  m_sEntryPoint = _entry;
  m_sModel = _sModel;
}
}