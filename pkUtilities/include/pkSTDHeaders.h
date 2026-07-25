#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include <cassert>
#include <codecvt>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <stack>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>

#if PK_PLATFORM == PK_PLATFORM_WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#endif

namespace pkEngineSDK
{

using Exception = std::exception;

using String = std::string;

// to do: change for linux too
using WString = std::wstring;

template<class T>
using Function = std::function<T>;

template<class T>
using Queue = std::queue<T>;

template<class T>
using SPtr = std::shared_ptr<T>;

template<class T>
using Stack = std::stack<T>;

template<class T>
using UPtr = std::unique_ptr<T>;

using std::cout;

using std::endl;

using std::fstream;

using std::hash;

using std::ifstream;

using std::ios;

using std::make_unique;

using std::make_shared;

using std::ofstream;

using std::ios_base;

using std::move;

using std::rand;

using std::reinterpret_pointer_cast;

using std::to_string;

using std::swap;

using Runtime_error = std::runtime_error;

PKFORCEINLINE String
stringToLower(const String& _string)
{
  String newString;
  const uint32 length = static_cast<uint32>(_string.length());
  newString.resize(length);

  for (uint32 i = 0; i < length; ++i) {
    newString[i] = tolower(_string[i]);
  }
  
  return newString;
}

#if PK_PLATFORM == PK_PLATFORM_WIN32

PKFORCEINLINE WString
stringToWString(const String& str)
{
  if (str.empty()) {
    return WString();
  }

  int32_t size_needed = MultiByteToWideChar(CP_UTF8,
                                            0,
                                            str.c_str(),
                                            static_cast<int32_t>(str.size()),
                                            nullptr,
                                            0);
  WString wstr(size_needed, 0);
  MultiByteToWideChar(CP_UTF8,
                      0, str.c_str(),
                      static_cast<int32_t>(str.size()),
                      &wstr[0],
                      size_needed);
  return wstr;
}

#endif

template<typename T> PKFORCEINLINE uint32
toUint32(const T& _x) {
  return static_cast<uint32>(_x);
}

template<class _Kty, class _Ty>
using UMap = std::unordered_map<_Kty, _Ty>;

template<class _path, class T>
using Map = std::map<_path, T>;

template <class T>
using Vector = std::vector<T>;

template<typename T>
using WPtr = std::weak_ptr<T>;

#if PK_PLATFORM == PK_PLATFORM_WIN32
  #undef min
  #undef max
#if !defined(NOMINMAX) && defined(_MSC_VER)
#  define NOMINMAX     //Required to stop windows.h messing up std::min
#endif
#if defined(__MINGW32__)
#  include <unistd.h>
#endif

#endif
}
