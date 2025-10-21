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
struct SimpleVertex;

class PK_CORE_EXPORT VertexBuffer
{
 public:
  VertexBuffer() = default;
  virtual ~VertexBuffer() = default;
};
}