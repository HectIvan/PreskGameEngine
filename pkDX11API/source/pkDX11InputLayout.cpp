/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11InputLayout.h"
#include "pkDX11Device.h"
#include "pkDX11VertexShader.h"

namespace pkEngineSDK
{

HRESULT
DX11InputLayout::create(DX11Device* _pDevice, DX11VertexShader _vs)
{
  HRESULT hr = S_OK;
  // define the input layout
  D3D11_INPUT_ELEMENT_DESC layout[]
  {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
  };
  uint32 numElem = ARRAYSIZE(layout);

  // create input layout
  hr = _pDevice->pd3dDevice->CreateInputLayout(layout,
                                               numElem,
                                               _vs.pSBlob->GetBufferPointer(),
                                               _vs.pSBlob->GetBufferSize(),
                                               &pVertexLayout);

  _vs.pSBlob->Release(); // already used
  if (FAILED(hr))
  {
    return hr;
  }
  return hr;
}

void
DX11InputLayout::set(DX11Device* _pDevice)
{
  // set the input layout
  _pDevice->pImmediateContext->IASetInputLayout(pVertexLayout);
}
}