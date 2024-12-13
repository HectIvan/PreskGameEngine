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

namespace pkEngineSDK
{

class DX11Device;

class DX11VertexBuffer : public VertexBuffer
{
 public:
  DX11VertexBuffer() = default;
  virtual ~DX11VertexBuffer()
  {
    safeRelease(pBuffer);
  }

 public:
  ID3D11Buffer* pBuffer = nullptr;
};
}