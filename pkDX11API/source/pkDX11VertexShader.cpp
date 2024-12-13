/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11VertexShader.h"
#include "pkDX11Device.h"

namespace pkEngineSDK {

HRESULT
DX11VertexShader::compile()
{
  HRESULT hr = S_OK;
  hr = compileShaderFromFile(L"D:/Work/visual studio/PreskGameEngine/pkDX11API/shaders/pkShader.hlsl", "VS", "vs_5_0", &pSBlob);
  if (FAILED(hr))
  {
    MessageBox(nullptr, "FX file could not be compiled", "Error", MB_OK);
    return hr;
  }
  return hr;
}

HRESULT
DX11VertexShader::create(DX11Device* _pDevice)
{
  HRESULT hr = S_OK;
  hr = _pDevice->pd3dDevice->CreateVertexShader(pSBlob->GetBufferPointer(), pSBlob->GetBufferSize(), nullptr, &pShader);
  if (FAILED(hr))
  {
    pSBlob->Release();
    return hr;
  }
  return hr;
}
}