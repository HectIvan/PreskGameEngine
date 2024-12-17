/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11Device.h"
#include "pkDX11InputLayout.h"
#include "pkDX11VertexShader.h"

namespace pkEngineSDK
{

void
DX11InputLayout::set(SPtr<Device> _pDevice)
{
  // cast to directX device
  auto dxDV = dynamic_pointer_cast<DX11Device>(_pDevice);
  // set the input layout
  dxDV->pImmediateContext->IASetInputLayout(pVertexLayout);
}
}