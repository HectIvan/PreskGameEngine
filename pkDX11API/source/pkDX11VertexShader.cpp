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
  hr = compileShaderFromFile(L"shaders/pkShader.fx", "VS", "vs_5_0", &m_pSBlob);
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
  hr = _pDevice->m_pd3dDevice->CreateVertexShader(m_pSBlob->GetBufferPointer(), m_pSBlob->GetBufferSize(), nullptr, &m_pShader);
  if (FAILED(hr))
  {
    m_pSBlob->Release();
    return hr;
  }
  return hr;
}

void
DX11VertexShader::clean()
{
  safeRelease(m_pShader);
}
}