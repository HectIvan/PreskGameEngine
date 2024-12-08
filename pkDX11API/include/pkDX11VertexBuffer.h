/************************************************************************/
/**
* @DX11VertexBuffer DX11VertexBuffer.h
* @Hector Ivan Muñoz Ceballos
* @date 21/10/2024
* @DirectX11 Vertex buffer file for the Presk Game Engine.
*
* This file contains the Vertex buffer of DX11 class of the engine
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
#include "pkVertexBuffer.h"
#include "pkSimpleVertex.h"

namespace pkEngineSDK {

class DX11Device;
class DX11VertexBuffer : public VertexBuffer
{
 public:
  DX11VertexBuffer() = default;
  virtual ~DX11VertexBuffer()
  {
    clean();
  }

  /**
  * Create the Vertex Buffer
  * 
  * @param _pDevice
  * Device that the buffer will be linked to
  * 
  * @param _vertex
  * Vertex info.
  * 
  * @param _usage
  * The use that is to be given to the buffer
  * 
  * @return
  * the shared pointer to the vertex buffer
  **/
  VertexBuffer*
  create(Device* _pDevice,
         const Vector<SimpleVertex>& _vertex,
         uint32 _usage) override;

  /**
  * Sets the data of the vertex buffer.
  * 
  * @param _device
  * device that will set the buffer
  **/
  void
  set(Device* _pDevice,
      uint32 _start = 0,
      uint32 _bufferCount = 1,
      uint32 _offset = 0);

  /**
  * Clean the vertex buffer.
  **/
  void
  clean();

 public:
  ID3D11Buffer* m_pBuffer = nullptr;
};
}