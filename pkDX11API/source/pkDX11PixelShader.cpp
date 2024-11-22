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
  hr = compileShaderFromFile(L"shaders/pkShader.fx", "PS", "ps_5_0", &m_pSBlob);
  if (FAILED(hr))
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
  hr = _pDevice->m_pd3dDevice->CreatePixelShader(m_pSBlob->GetBufferPointer(), m_pSBlob->GetBufferSize(), nullptr, &m_pPixelShader);
  if (FAILED(hr))
  {
    m_pSBlob->Release();
    return hr;
  }
  return hr;
}

void
DX11PixelShader::clean()
{
  safeRelease(m_pPixelShader);
}
}