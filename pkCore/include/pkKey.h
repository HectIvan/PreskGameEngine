/************************************************************************/
/**
* @pkKey pkKey.h
* @Hector Ivan Muñoz Ceballos
* @date 09/12/2024
* @Key file for the Presk Game Engine.
*
* This file contains the Key struct of the engine
*
* @bug No bug known.
* 
* @HectIvan 02/01/2025
* Added the backspace enum
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

namespace KEY
{
  enum E
  {
    // mouse buttons
    kLButton = 0x01,
    kRButton = 0x02,
    kCButton = 0x04,

    // text editing buttons
    kBackSpace = 0x08,
    kSpace = 0x20,

    // arrows
    kLeft = 0x25,
    kUp = 0x26,
    kRight = 0x27,
    kDown = 0x28,

    // numbers
    k0 = 0x30,
    k1 = 0x31,
    k2 = 0x32,
    k3 = 0x33,
    k4 = 0x34,
    k5 = 0x35,
    k6 = 0x36,
    k7 = 0x37,
    k8 = 0x38,
    k9 = 0x39,

    // used keybinds
    kA = 0x41,
    kB = 0x42,
    kC = 0x43,
    kD = 0x44,
    kE = 0x45,
    kF = 0x46,
    kG = 0x47,
    kH = 0x48,
    kQ = 0x51,
    kS = 0x53,
    kW = 0x57,
    
    kEsc = 0x1B,
    kLControl = 0xA2,
    kRControl = 0xA3,
  };
}

struct Key
{
  uint32 id;
  bool pressed;
};
}