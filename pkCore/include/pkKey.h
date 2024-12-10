/************************************************************************/
/**
* @pkKey pkKey.h
* @Hector Ivan Muñoz Ceballos
* @date 09/12/2024
* @Keye file for the Presk Game Engine.
*
* This file contains the Key struct of the engine
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
#include "pkPrerequisitesCore.h"

namespace pkEngineSDK
{

namespace KEY
{
  enum E
  {
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
    kW = 0x57,
    kA = 0x41,
    kS = 0x53,
    kD = 0x44,
    kE = 0x45,
    kQ = 0x51,
    kSpace = 0x20,
    kEsc = 0x1B,
  };
}

struct Key
{
  uint32 id;
  bool pressed;
};
}