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
DX11PixelShader::compile()
{
  HRESULT hr = S_OK;
  hr = compileShaderFromFile(L"D:/Work/visual studio/PreskGameEngine/pkDX11API/shaders/pkShader.hlsl",
                             "PS",
                             "ps_5_0",
                             &pSBlob);
  if (hr != 0x00000000)
  {
    MessageBox(nullptr, "FX file could not be compiled", "Error", MB_OK);
    return hr;
  }
  return hr;
}

HRESULT
DX11PixelShader::create(DX11Device* _pDevice)
{
  HRESULT hr = S_OK;
  hr = _pDevice->pd3dDevice->CreatePixelShader(pSBlob->GetBufferPointer(),
                                               pSBlob->GetBufferSize(),
                                               nullptr, &pShader);
  if (FAILED(hr))
  {
    pSBlob->Release();
    return hr;
  }
  return hr;
}
}