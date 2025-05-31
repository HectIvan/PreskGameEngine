/************************************************************************/
/**
* @pkSwapChain pkSwapChain.h
* @Hector Ivan Muñoz Ceballos
* @date 17/12/2024
* @Swap Chain file for the Presk Game Engine.
*
* This file contains the Swap Chain class of the engine
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
#include "pkPrerequisitesCore.h"
#include "pkTexture.h"

namespace pkEngineSDK
{

class SwapChain
{
 public:
  SwapChain() = default;
  virtual ~SwapChain() = default;

  /**
   * @get the back buffer texture.
   * @param _index Position of the buffer.
   * @return Texture of the back buffer.
   */
  virtual SPtr<Texture>
  getBuffer(uint32 _index) = 0;

 public:
  Vector<SPtr<Texture>> m_buffers;
  const uint32 m_bufferCount = 2;
};
}
