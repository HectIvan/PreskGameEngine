/************************************************************************/
/**
* @pkDx11IndexBuffer pkDx11IndexBuffer.h
* @Hector Ivan Muñoz Ceballos
* @date 11/11/2024
* @DirectX Index Buffer file for the Presk Game Engine.
*
* This file contains the Index Buffer class using DirectX11 of the engine
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
#include "pkIndexBuffer.h"

namespace pkEngineSDK
{

class DX11IndexBuffer : public IndexBuffer
{
 public:
  DX11IndexBuffer() = default;
  virtual ~DX11IndexBuffer()
  {
    safeRelease(m_pBuffer);
  }

  /**
  * Clean the buffer
  **/
  void
  clean();

 public:
  ID3D11Buffer* m_pBuffer = nullptr;
};
}