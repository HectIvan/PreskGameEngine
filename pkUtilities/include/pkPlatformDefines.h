/**************************************************************/
/**
 * Initial platform/compiler-related stuff to set
**/
/**************************************************************/
#pragma once

#define PK_EXTERN extern "C"

#define PK_PLATFORM_WIN32   1          // Windows Platform
#define PK_PLATFORM_LINUX   2          // Linux Platform
#define PK_PLATFORM_OSX     3          // Mac Platform


#define PK_COMPILER_MSVC    1         // Visual Studio Compiler
#define PK_COMPILER_GNUC    2         // GCC Compiler
#define PK_COMPILER_INTEL   3         // Intel Compiler
#define PK_COMPILER_CLANG   4         // clang Compiler

#define PK_ARCHITECTURE_X86_32 1      // Intel x86 32 bits
#define PK_ARCHITECTURE_X86_64 2      // Intel x86 64 bits

#define PK_ENDIAN_LITTLE    1
#define PK_ENDIAN_BIG       2

// Define the actual endian type (little endian for Windows, Linux)
#define PK_ENDIAN PK_ENDIAN_LITTLE

#define PK_ENDIAN_MAJOR     0         // Engine version major
#define PK_VERSION_MINIOR   1
#define PK_VERSION_PATCH    0
#define PK_VERSION_BUILD    1

// How much detail will symbols return
#define PK_DEBUG_DETAILED_SYMBOLS 1

/*******************************************************************/
/**
 * Compiler definition
**/
/*******************************************************************/

#if defined(__clang__)
#   define PK_COMPILER PK_COMPILER_CLANG
#   define PK_COMP_VER __cland_version__
#   define PK_THREADLOCAL __thread
#   define PK_STDCALL __atribute__((stdcall))
#   define PK_CDECL __attribute__((cdecl))
#   define PK_FALLTHROUGH [[clang::fallthrough]];
#elif defined (__GNUC__) // Check after Cland
#   define PK_COMPILER PK_COMPILER_CNUC
#   define PK_COMP_VER (((_GNUC)*100)+(_GNUC_MINOR__*10)+__GNUC_PATCHLEVEL__)
#   define PK_THREADLOCAL __thread
#   define PK_STDCALL __attribute__((stdcall))
#   define PK_CDECL __attribute__((cdecl))
#   define PK_FALLTHROUGH __attribute__((fallthrough));
#elif defined (__INTEL_COMPILER)
#   define PK_COMPILER PK_COMPILER_INTEL
#   define PK_COMP_VER __INTEL_COMPILER
#   define PK_STDCALL __stdcall
#   define PK_CDECL __cdecl
#   define PK_FALLTHROUGH

/*******************************************************************/
/**
 * PK_THREADLOCAL define is below because intel defines it differently 
 * depending on platform
**/
/*******************************************************************/

// check after clang end intel
#elif defined (_MSC_VER)
#   define PK_COMPILER PK_COMPILER_MSVC
#   define PK_COMP_VER _MSC_VER
#   define PK_THREADLOCAL __declspec
#   define PK_STDCALL __stdcall
#   define PK_CDECL __cdecl
#   define PK_FALLTHROUGH
#   undef __PRETTY_FUNCTION__
#   define __PRETTY_FUNCTION__ __FUNCSIG__
#else
// no compiler found, send error to output
#   pragma error "No known compiler."
#endif

/*******************************************************************/
/**
 * see if __forceinline can be used or __inline needs to be used instead
**/
/*******************************************************************/

#if PK_COMPILER == PK_COMPILER_MSVC
# if PK_COMP_VER >= 1200
#   define PKFORCEINLINE __forceinline
#   ifndef RESTRICT
#     define RESTRICT __restrict
#   endif
# endif
#elif defined(__MINGW32__)
# if !defined(PKFORCEINLINE)
#   define PKFORCEINLINE __inline
#   ifndef RESTRICT
#     define RESTRICT
#   endif
# endif
#else
# define PKFORCEINLINE __inline
# ifndef RESTRICT
#   define RESTRICT __restrict
# endif
#endif

/*******************************************************************/
/**
 * Find current platform
**/
/*******************************************************************/
#if defined (__WIN32__) || defined (_WIN32)
# define PK_PLATFORM PK_PLATFORM_WIN32
#elif defined (__APPLE_CC__)
# define PK_PLATFORM PK_PLATFORM_OSX
#elif defined (__linux__) || defined(__gnu_linux__)
# define PK_PLATFORM PK_PLATFORM_LINUX
#else
#   pragma error "no platform found."
#endif

/*******************************************************************/
/**
 * Find architecture type
**/
/*******************************************************************/
#if defined (__x86_x64__) || defined (_M_X64)|| defined(__amd64__) || defined(__amd64)
# define PK_ARCH_TYPE PK_ARCHITECTURE_X86_64
#else
# define PK_ARCH_TYPE PK_ARCHITECTURE_X86_32
#endif

/*******************************************************************/
/**
 * Memory alignment macros
**/
/*******************************************************************/
#if PK_COMPILER == PK_COMPILER_MSVC
# define MS_ALLIGN(n) __declspec(align(n))
# ifndef GCC_PACK
#   define GCC_PACK(n)
# endif
# ifndef GCC_ALIGN
#   define GCC_ALIGN(n)
# endif
#elif (PK_COMPILER == PK_COMPILER_GNUC)
# define MS_ALIGN(n)
# define GCC_PACK(n)
# define GCC_ALIGN(n) __attribute__( (__aligned__(n)) )
#else
# define MS_ALIGN(n)
# define GCC_PACK(n) __attribute__((packed, aligned(n)))
# define GCC_ALIGN(n) __ attribute__( (__aligned(n)) )
#endif

/*******************************************************************/
/**
 * for throw override
**/
/*******************************************************************/
#if PK_COMPILER == PK_COMPILER_MSVC
# define _NOEXCEPT noexcept
#elif PK_COMPILER == PK_COMPILER_INTEL
# define _NOEXCEPT noexcept
#elif PK_COMPILER == PK_COMPILER_GNUC
# define _NOEXCEPT noexcept
#else
# define _NOEXCEPT
#endif

/*******************************************************************/
/**
 * library exports specifics
**/
/*******************************************************************/
#if PK_PLATFORM == PK_PLATFORM_WIN32
# if PK_COMPILER == PK_COMPILER_MSVC
#   if defined( PK_STATIC_LIB )
#     define PK_UTILITY_EXPORT
#   else
#     if defined ( PK_UTILITY_EXPORTS )
#       define PK_UTILITY_EXPORT __declspec( dllexport )
#     else
#       define PK_UTILITY_EXPORT __declspec( dllimport )
#     endif
#   endif
# else // any other compiler
#   if defined( PK_STATIC_LIB )
#     define PK_UTILITY_EXPORT
#   else
#     if defined ( PK_UTILITY_EXPORTS)
#       define PK_UTILITY_EXPORT __attribute__ ((dllexport))
#     else
#       define PK_UTILITY_EXPORT __attribute__ ((dllimport))
#     endif
#   endif
# endif
# define PK_UTILITY_HIDDEN
#else
# define PK_UTILITY_EXPORT __attribute__ ((visibility ("default")))
# define PK_UTILITY_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

// DLL export plug ins
#if PK_PLATFORM == PK_PLATFORM_WIN32
# if PK_COMPILER == PK_COMPILER_MSVC
#   define PK_PLUGIN_EXPORT __declspec(dllexport)
# else
#   define PK_PLUGIN_EXPORT __attribute__ ((dllexport))
# endif
#else
#  define PK_PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

/*******************************************************************/
/**
 * window specific settings
**/
/*******************************************************************/
#if PK_PLATFORM == PK_PLATFORM_WIN32
# if defined(_DEBUG) || defined(DEBUG)
#   define PK_DEBUG_MODE 1
# else
#   define PK_DEBUG_MODE 0
# endif
# if PK_COMPILER == PK_COMPILER_INTEL
#   define PK_THREADLOCAL __declspec(thread)
# endif
#endif

/*******************************************************************/
/**
 * LINUX-Apple specific settings
**/
/*******************************************************************/

#if PK_PLATFORM == PK_PLATFORM_LINUX || PK_PLATFORM == PK_PLATFORM_OSX

// if in debug mode

# if defined(_DEBUG) || defined(DEBUG)
#   define PK_DEBUG_MODE 1
# else
#   define PK_DEBUG_MODE 0
# endif
# if PK_COMPILER == PK_COMPILER_INTEL
#   define PK_THREAD_LOCAL thread
# endif
#endif

/*******************************************************************/
/**
 * Definition of Debug macros
**/
/*******************************************************************/
#if PK_DEBUG_MODE
# define PK_DEBUG_ONLY(x) x
# define PK_ASSERT(x) assert(x)
#else
# define PK_DEBUG_ONLY(x)
# define PK_ASSERT(x)
#endif

/*******************************************************************/
/**
 * Extern
**/
/*******************************************************************/
#define PK_EXTERN extern "C"

/*******************************************************************/
/**
 * Disable some compiler warnings
**/
/*******************************************************************/
// if in Visual Studio
#if PK_COMPILER == PK_COMPILER_MSVC
# define _CRT_SECURE_NO_WARNINGS
# pragma warning (disable : 4201)
# pragma warning (disable : 4251)
# pragma warning (disable : 4996)
# pragma warning (disable : 4503)
#endif
