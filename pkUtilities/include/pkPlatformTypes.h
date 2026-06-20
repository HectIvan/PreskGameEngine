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
using WCHAR = wchar_t;
using ANSICHAR = char;
using UNICHAR = WCHAR;
using UANSICHAR = unsigned char;

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

/**************************************************************/
/**
 * Byte data type
**/
/**************************************************************/
using byte = uint8;

/**************************************************************/
/**
 * Max path length
**/
/**************************************************************/
#define PK_MAX_PATH          260

/**************************************************************/
/**
 * Platform pointer
**/
/**************************************************************/
#if PK_ARCH_TYPE == PK_ARCHITECTURE_X86_32
using PlatformPointer = int32*;
#else
using PlatformPointer = int64*;
#endif

#if PK_PLATFORM == PK_PLATFORM_WIN32
#ifndef _PKRESULT_DEFINED
#define _PKRESULT_DEFINED
#ifdef __midl
typedef LONG PKRESULT;
#else
typedef _Return_type_success_(return >= 0) long PKRESULT;
#endif // __midl
#endif // !_PKRESULT_DEFINED
#endif

#define PK_FAILED(hr) (((PKRESULT)(hr)) < 0)
#define _PKRESULT_TYPEDEF_(_sc) ((PKRESULT)_sc)
#define PKNOERROR             0

#define PK_OK                                   ((PKRESULT)0L)
#define PK_FALSE                                ((PKRESULT)1L)

#define PK_NOTIMPL                        _PKRESULT_TYPEDEF_(0x80004001L)
#define PK_OUTOFMEMORY                    _PKRESULT_TYPEDEF_(0x8007000EL)
#define PK_INVALIDARG                     _PKRESULT_TYPEDEF_(0x80070057L)
#define PK_NOINTERFACE                    _PKRESULT_TYPEDEF_(0x80004002L)
#define PK_POINTER                        _PKRESULT_TYPEDEF_(0x80004003L)
#define PK_HANDLE                         _PKRESULT_TYPEDEF_(0x80070006L)
#define PK_ABORT                          _PKRESULT_TYPEDEF_(0x80004004L)
#define PK_FAIL                           _PKRESULT_TYPEDEF_(0x80004005L)
#define PK_ACCESSDENIED                   _PKRESULT_TYPEDEF_(0x80070005L)
}
