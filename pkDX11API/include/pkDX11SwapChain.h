/************************************************************************/
/**
* @pkDX11SwapChain pkDX11SwapChain.h
* @Hector Ivan Muñoz Ceballos
* @date 17/12/2024
* @Swap Chain file using DirectX for the Presk Game Engine.
*
* This file contains the Swap Chain class using DirectX of the engine
*
* @bug No bug known.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11Prerequisites.h"
#include "pkSwapChain.h"

namespace pkEngineSDK
{

class DX11Device;

class DX11SwapChain : public SwapChain
{
public:
  DX11SwapChain() = default;
  virtual ~DX11SwapChain()
  {
    safeRelease(m_pSch);
    m_buffers.clear();
  }

/**
 * @brief Create the back buffers for the swaap chain.
 * @param _pDevice API specific device.
 */
void
createRenderTargetView(const SPtr<DX11Device>& _pDevice);

/**
 * @get the back buffer texture.
 * @param _index Position of the buffer.
 * @return Texture of the back buffer.
 */
PKFORCEINLINE SPtr<Texture>&
getBuffer(const uint32& _index) override { return m_buffers[_index]; }

/**
 * @brief Resize the buffers.
 * @param _size New size of the buffers.
 */
void
resizebuffers(const Vector2& _size) override;

 public:
  PKSwapChain* m_pSch;
};
}