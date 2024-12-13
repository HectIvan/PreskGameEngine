/************************************************************************/
/**
* @pkDX11ConstantBuffer pkDX11ConstantBuffer.h
* @Hector Ivan Muñoz Ceballos
* @date 21/10/2024
* @DirectX11 constant buffer file for the Presk Game Engine.
*
* This file contains the constant buffer of DX11 class of the engine
*
* @bug.
* Currently, te file operates on int, it should not.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11Prerequisites.h"
#include "pkConstantBuffer.h"

namespace pkEngineSDK
{

class DX11Device;

class DX11ConstantBuffer : public ConstantBuffer
{
public:
  DX11ConstantBuffer() = default;
  virtual ~DX11ConstantBuffer()
  {
    safeRelease(pCBuffer);
  }

 public:
  ID3D11Buffer* pCBuffer = nullptr;
};
}
