#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesUtilities.h"
#include "pkVector3.h"
#include "pkVector4.h"
#include "pkMatrix4.h"

// defines
#include "pkCoreDefines.h"

/*********************************************/
/**
* Defines
**/
/*********************************************/
#define BONE_LIMIT 100
#define MAX_BONE_INFLUENCE 4
#define MAX_BONE_WEIGHT 100

namespace pkEngineSDK
{

using std::reinterpret_pointer_cast;
using std::make_shared; 

struct CBView
{
  Matrix4 view;
};

struct CBProjection
{
  Matrix4 projection;
};

struct CBWorld
{
  Matrix4 world;
  // Vector4 meshColor;
};

struct CBCamera
{
  Vector4 eye;
  Vector3 forward;
};

struct KeyPosition
{
  Vector3 position;
  float timeStamp;
};

struct KeyRotation
{
  Vector4 rotation;
  float timeStamp;
};

struct KeyScale
{
  Vector3 scale;
  float timeStamp;
};

template<class T>
void safeRelease(T* x) { if (x) { x->Release(); x = nullptr; } }
}