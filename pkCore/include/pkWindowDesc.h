/************************************************************************/
/**
* @pkWindowDesc pkWindowDesc.h
* @Hector Ivan Muñoz Ceballos
* @date 30/09/2024
* @Window descriptor file for the Presk Game Engine.
*
* This file will contain the window descriptor used for the engine
*
* @bug No bug known.
*
* @HectIvan 30/09/2024
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

#if PK_PLATFORM == PK_PLATFORM_WIN32
  using WinFunctEvent = Function<int64*(PlatformPointer,
                                        uint32,
                                        PlatformPointer,
                                        PlatformPointer)>;
#endif


struct PK_CORE_EXPORT PKWindowDesc
{
  PKWindowDesc() = default;
  PKWindowDesc(const uint32& _width,
               const uint32& _height,
               const int32& _posX,
               const int32& _posY,
               const String& _name) {
     width = _width;
     height = _height;
     posX = _posX;
     posY = _posY;
     name = _name;
  }
  uint32 width;
  uint32 height;

  int32 posX;
  int32 posY;

  String name;
  WinFunctEvent funct;
};
}