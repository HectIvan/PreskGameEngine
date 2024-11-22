/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11DepthStencilView.h"

namespace pkEngineSDK {

void
DX11DepthStencilView::clean()
{
  safeRelease(m_pDepthSV);
}
}
