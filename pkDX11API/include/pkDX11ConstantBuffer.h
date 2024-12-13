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
#include "pkConstantBuffer.h"
#include "pkDX11Prerequisites.h"

namespace pkEngineSDK {

class DX11Device;

class DX11ConstantBuffer : public ConstantBuffer
{
 public:
  DX11ConstantBuffer() = default;
  virtual ~DX11ConstantBuffer()
  {
    safeRelease(pCBuffer);
  }

  /**
  * 
  **/
  HRESULT
  create(DX11Device* _pDevice, uint32 _size, const void* _pData = nullptr, uint32 _usage = D3D11_USAGE_DEFAULT); // int _size && int_usage are temporary

  /**
  * Updates the data inside of the buffer
  * 
  * @param _pDevice
  * Device that we will use.
  * 
  * @param _pNewData
  * New data that the buffer will store.
  * 
  * @param _size
  * Size of the data.
  **/
  void
  updateSubResource(DX11Device* _pDevice, const void* _pNewData, uint32 _size); // int _size is temporary

  /**
  * Clean the buffer
  **/
  void
  clean();

 public:
  ID3D11Buffer* pCBuffer = nullptr;
};
}
