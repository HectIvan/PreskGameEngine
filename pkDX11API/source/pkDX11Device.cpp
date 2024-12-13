/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11Device.h"

namespace pkEngineSDK
{

UPtr<Device>
DX11Device::getDevice()
{
  return make_unique<DX11Device>();
}

HRESULT
DX11Device::init(D3D_DRIVER_TYPE* _pDriverType, D3D_FEATURE_LEVEL* _pFeatureLevel)
{
  *pDriverType = *_pDriverType;
  featureLevel = *_pFeatureLevel;
  return S_OK;
}

void
DX11Device::clean()
{
  safeRelease(pd3dDevice);
  safeRelease(pImmediateContext);
}

void
DX11Device::setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY _topology)
{
  pImmediateContext->IASetPrimitiveTopology(_topology);
}
}