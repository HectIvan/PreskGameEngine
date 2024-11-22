/************************************************************************/
/**
* @pkPlatformTypes pkPlatformTypes.h
* @Hector Ivan Muñoz Ceballos
* @date 06/09/2024
* @Platform Types file for the Presk Game Engine.
* 
* This file contains the mathematics necessary depending in the OS
*
* @bug No bug known.
*
* @HectIvan 06/09/2024
* Added cases for the OS
*/
/************************************************************************/
#pragma once

/**************************************************************/
/**
 * Includes
**/
/**************************************************************/
#include "pkPlatformMath.h"
#include "pkPrerequisitesUtilities.h"

namespace pkEngineSDK {
#if PK_PLATFORM == PK_PLATFORM_WIN32 || \
    PK_PLATFORM == PK_PLATFORM_LINUX || \
    PK_PLATFORM == PK_PLATFORM_OSX

using Math = PlatformMath;

#else
using Math = PlatformMath;
#endif
}
