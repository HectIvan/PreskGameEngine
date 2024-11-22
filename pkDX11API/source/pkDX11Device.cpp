/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11Device.h"

namespace pkEngineSDK
{

HRESULT
DX11Device::init(HWND& _hWnd, D3D_DRIVER_TYPE* _pDriverType, D3D_FEATURE_LEVEL* _pFeatureLevel)
{
  *m_pDriverType = D3D_DRIVER_TYPE_NULL;
  m_featureLevel = D3D_FEATURE_LEVEL_11_0;
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