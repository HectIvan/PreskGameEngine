/*****************************************************************************/
/**
 * @file    pkPass.cpp
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    07/02/2025
 * @brief   Source file of the pass
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkAssetResourceManager.h"
#include "pkGraphicsAPI.h"
#include "pkPass.h"
#include "pkRendererManager.h"
#include "pkSceneManager.h"
#include "pkShaderCodec.h"
#include "pkShaderManager.h"
#include "pkLogger.h"

namespace pkEngineSDK
{

Pass::Pass() {
  m_pInputLayout = pk_shared_ptr_new<InputLayout>();
  m_pSamplerState = pk_shared_ptr_new<SamplerState>();
}

void
Pass::createBasics(const PassDesc& _desc)
{
  GraphicsAPI& api = g_GraphicAPI();

  // create the sampler state
  m_pSamplerState = api.createSamplerState(_desc.samAdress, _desc.samFilters);

  if (_desc.rSExists) {
    // rasterizer state.
    RASTERIZER_DESC rDesc = {};
    rDesc.fillMode = _desc.rSFillMode;
    rDesc.cullMode = _desc.rSCullMode;
    rDesc.frontCounterClockwise = _desc.rSFrontCounterClockwise;
    rDesc.depthClipEnable = _desc.rSDepthClipEnable;
    // create the rasterizer state.
    m_pRasterizerState = api.createRasterizerState(rDesc);
  }
}
}
