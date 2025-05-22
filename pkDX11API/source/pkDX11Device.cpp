/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11Device.h"

namespace pkEngineSDK
{

void*
DX11Device::getDevice()
{
  return pd3dDevice;
}

void*
DX11Device::getDeviceContext()
{
  return pImmediateContext;
}

HRESULT
DX11Device::init(D3D_DRIVER_TYPE* _pDriverType, D3D_FEATURE_LEVEL* _pFeatureLevel)
{
  *pDriverType = *_pDriverType;
  featureLevel = *_pFeatureLevel;
  return S_OK;
}

void
DX11Device::setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY _topology)
{
  pImmediateContext->IASetPrimitiveTopology(_topology);
}
}