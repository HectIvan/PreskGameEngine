#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include <map>
#include <optional>
#include <queue>
#include <stack>
#include <string>
#include <vector>

namespace pkEngineSDK
{

template <class T>
using Queue = std::queue<T>;

template<typename T>
using SPtr = std::shared_ptr<T>;

template <class T>
using Stack = std::stack<T>;

using String = std::string;

template<class _Kty, class _Ty>
using UMap = std::unordered_map<_Kty, _Ty>;

template<class _path, class T>
using Map = std::map<_path, T>;

template <class T>
using Vector = std::vector<T>;

template<typename T>
using WPtr = std::weak_ptr<T>;
}