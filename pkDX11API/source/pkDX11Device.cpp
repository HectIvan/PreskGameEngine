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
  return m_pd3dDevice;
}

void*
DX11Device::getDeviceContext()
{
  return m_pImmediateContext;
}

PKRESULT
DX11Device::init(D3D_DRIVER_TYPE* _pDriverType, D3D_FEATURE_LEVEL* _pFeatureLevel)
{
  *m_pDriverType = *_pDriverType;
  m_featureLevel = *_pFeatureLevel;
  return PK_OK;
}

void
DX11Device::setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY _topology)
{
  m_pImmediateContext->IASetPrimitiveTopology(_topology);
}
}