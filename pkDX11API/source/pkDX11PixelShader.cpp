/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11PixelShader.h"
#include "pkDX11Device.h"

namespace pkEngineSDK
{

HRESULT
DX11PixelShader::compile(wstring _fileName, const char* _entryPoint, const char* _model)
{
  HRESULT hr = S_OK;
  hr = compileShaderFromFile(_fileName,
                             _entryPoint,
                             _model,
                             &pSBlob);
  if (hr != 0x00000000)
  {
    MessageBox(nullptr, "FX file could not be compiled", "Error", MB_OK);
    return hr;
  }
  return hr;
}
}