#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include <cassert>
#include <codecvt>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <stack>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace pkEngineSDK
{

template<class T>
using Function = std::function<T>;

using std::make_unique;

using std::rand;

using std::to_string;

template <class T>
using WString_Conv = std::wstring_convert<T>;

template <class T>
using Codecvt_utf8 = std::codecvt_utf8<T>;  

template <class T>
using Queue = std::queue<T>;

template<class T>
using SPtr = std::shared_ptr<T>;

template<class T>
using UPtr = std::unique_ptr<T>;

template <class T>
using Stack = std::stack<T>;

using String = std::string;

// to do: change for linux too
using WString = std::wstring;

template<class _Kty, class _Ty>
using UMap = std::unordered_map<_Kty, _Ty>;

template<class _path, class T>
using Map = std::map<_path, T>;

template <class T>
using Vector = std::vector<T>;

template<typename T>
using WPtr = std::weak_ptr<T>;

}