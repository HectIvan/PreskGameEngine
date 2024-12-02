/************************************************************************/
/**
* @pkVertexbuffer pkVertexbuffer.h
* @Hector Ivan Muñoz Ceballos
* @date 20/11/2024
* @Vertex Buffer file for the Presk Game Engine.
*
* This file will contain the Vertex Buffer used for the engine
*
* @bug No bug known.
*
* @HectIvan 20/11/2024
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"

namespace pkEngineSDK
{

class Device;

class VertexBuffer
{
 public:
  VertexBuffer() = default;
  virtual ~VertexBuffer() = default;

  virtual SPtr<VertexBuffer>
  create(Device* _pDevice,
         const Vector<SimpleVertex>& _vertex,
         uint32 _usage) = 0;

  virtual void
  set(Device* _pDevice,
      uint32 _start = 0,
      uint32 _bufferCount = 1,
      uint32 _offset = 0) = 0;
};
}