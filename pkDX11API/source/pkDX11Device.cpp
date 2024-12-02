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
  *m_pDriverType = *_pDriverType;
  m_featureLevel = *_pFeatureLevel;
  return S_OK;
}

void
DX11Device::clean()
{
  safeRelease(m_pd3dDevice);
  safeRelease(m_pImmediateContext);
}

void
DX11Device::setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY _topology)
{
  m_pImmediateContext->IASetPrimitiveTopology(_topology);
}
}