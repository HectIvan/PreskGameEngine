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

namespace pkEngineSDK {

class DX11Device;

class DX11IndexBuffer : public IndexBuffer
{
 public:
  DX11IndexBuffer() = default;
  virtual ~DX11IndexBuffer()
  {
    safeRelease(m_pBuffer);
  }

  /**
  * Create.
  * 
  * Create the index buffer.
  * 
  * @param _pDevice
  * Device where it will be created.
  * 
  * @param _index
  * Vector of all the index.
  * 
  * @param _usage
  * What usage will be given to the buffer.
  * 
  * @return
  * Shared pointer of the index buffer.
  **/
  SPtr<IndexBuffer>
  create(Device* _pDevice,
         const Vector<uint32>& _index,
         uint32 _usage = D3D11_USAGE_DEFAULT) override;

  /**
  * Set the index buffer.
  * 
  * @param _pDevice.
  * Device where the buffer resides.
  * 
  * @param _format
  * What kind of format will the buffer use.
  * 
  * @param _offset
  * Distance between blobs of data.
  **/
  void
  set(Device* _pDevice,
      uint32 _format = DXGI_FORMAT_R32_UINT,
      uint32 _offset = 0);

  /**
  * Clean the buffer
  **/
  void
  clean();

 public:
  ID3D11Buffer* m_pBuffer = nullptr;
};
}