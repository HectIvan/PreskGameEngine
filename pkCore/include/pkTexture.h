/************************************************************************/
/**
* @pkTexture pkTexture.h
* @Hector Ivan Muñoz Ceballos
* @date 22/11/2024
* @Texture file for the Presk Game Engine.
*
* Texture class for the engine
*
* @bug No bug known.
*
* @HectIvan 25/11/2024
* Created the file
*/
/************************************************************************/
#pragma once
/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"
#include "pkColor.h"

namespace pkEngineSDK
{

class Device;

namespace TEXTURE_TYPE
{
  enum E
  {
    kDiffuse = 0,
    kNormal,
    kEmmissive,
  };
}

class PK_CORE_EXPORT Texture
{
 public:
  Texture() = default;
  virtual ~Texture() = default;

 public:
  uint32 id;
  TEXTURE_TYPE::E type;
  String path;
  uint32 width;
  uint32 height;
};
}
