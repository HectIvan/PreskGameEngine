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
#include "pkSimpleVertex.h"

namespace pkEngineSDK {

class DX11Device;
class DX11VertexBuffer
{
 public:
  DX11VertexBuffer() = default;
  virtual ~DX11VertexBuffer()
  {
    safeRelease(m_pBuffer);
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
  SPtr<DX11VertexBuffer> VertexBuffer
  create(DX11Device* _pDevice,
         const vector<simpleVertex>& _vertex,
         uint32 _usage = D3D11_USAGE_DEFAULT); // int _usage is temporary

  /**
  * Sets the data of the vertex buffer.
  * 
  * @param _device
  * device that will set the buffer
  **/
  void
  set(DX11Device* _pDevice, uint32 _start = 0, uint32 _bufferCount = 1, uint32 _offset = 0); // int use is temporary

 public:
  ID3D11Buffer* m_pBuffer = nullptr;
};
}