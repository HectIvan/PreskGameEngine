/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11Shader.h"
#include <d3dcompiler.h>

namespace pkEngineSDK {

uint32
DX11Shaders::compileShaderFromFile(wstring _szFileName,
                                   const char* _szEntryPoint,
                                   const char* _szShaderModel)
{
  HRESULT hr = S_OK;

  DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
  // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
  // Setting this flag improves the shader debugging experience, but still allows 
  // the shaders to be optimized and to run exactly the way they will run in 
  // the release configuration of this program.
  dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
  ID3DBlob* pErrorBlob;
  hr = D3DCompileFromFile(_szFileName.c_str(),
                          nullptr,
                          nullptr,
                          _szEntryPoint,
                          _szShaderModel,
                          dwShaderFlags,
                          0,
                          &pSBlob,
                          &pErrorBlob);
  if (hr != 0x00000000)
  {
    if (pErrorBlob != nullptr)
    {
      String errText(reinterpret_cast<char*>(pErrorBlob->GetBufferPointer()));
    }
    safeRelease(pErrorBlob);
    return hr;
  }
  
  return S_OK;
}
}