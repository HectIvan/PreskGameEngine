/************************************************************************/
/**
* @pkPlatformTypes pkPlatformTypes.h
* @Hector Ivan Muñoz Ceballos
* @date 06/09/2024
* @Platform Types file for the Presk Game Engine.
* 
* Contains all the data types to be used in the engine
*
* @bug No bug known.
*
* @HectIvan 06/09/2024 
* Added the needed datatypes for the time being
*/
/************************************************************************/
#pragma once

/**************************************************************/
/**
 * Includes
**/
/**************************************************************/
#include <cstdint>
#include <cstddef>
#include "pkPlatformDefines.h"

/**
 * Intermediate language definition, any changes are made here
**/

namespace pkEngineSDK {
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::size_t;

/**************************************************************/
/**
 * Basic unsigned types
**/
/**************************************************************/
using uint8  = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

/**************************************************************/
/**
 * Basic signed types
**/
/**************************************************************/
using int8  = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

/**************************************************************/
/**
 * Character types
**/
/**************************************************************/
#if PK_COMPILER == PK_COMPILER_MSVC
using WCHAR = wchar_t;
#else
using WCHAR = unsigned short;
#endif
using ANSICHAR = char;
using UNICHAR = WCHAR;
using unchar = unsigned char;

/**************************************************************/
/**
 * NULL data type
**/
/**************************************************************/
using TYPE_OF_NULL = int32;

/**************************************************************/
/**
 * SIZE_T data type dependent on architecture
**/
/**************************************************************/
using SIZE_T = size_t;
}
