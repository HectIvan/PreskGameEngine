/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11Shader.h"

namespace pkEngineSDK {

HRESULT
DX11Shaders::compileShaderFromFile(wstring _szFileName,
                                   LPCSTR _szEntryPoint,
                                   LPCSTR _szShaderModel,
                                   ID3DBlob** _ppBlobOut)
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
                          _ppBlobOut,
                          &pErrorBlob);
  if (FAILED(hr))
  {
    if (pErrorBlob != NULL)
    {
      OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
    }
    safeRelease(pErrorBlob);
    return hr;
  }
  
  return S_OK;
}

void
DX11Shaders::clean()
{
  safeRelease(m_pSBlob);
}
}