/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11SamplerState.h"

namespace pkEngineSDK {

void
DX11SamplerState::clean()
{
  safeRelease(m_pSampler);
}
}