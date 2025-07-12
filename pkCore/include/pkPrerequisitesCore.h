#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesUtilities.h"
#include "pkVector2.h"
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

struct CBTransform
{
  Matrix4 transform;
};

struct CBCamera
{
  Vector4 eye; // 16
  Vector3 forward; // 12
  Matrix4 view; // 64
  Matrix4 projection; // 64
  float unused; // 4
  Vector4 _padding; // 12
}; // 176 bytes

struct CBLight
{
  Vector4 LightDir; // 16
  Vector4 LightPos; // 32
  Vector4 LightColor; // 48
  float shadowIntensity; // 52
  float spotExponent; // 56
  Vector2 unused1; // 64
};

struct CBAOData
{
  Vector2 screen_size;
  float random_size;

  float sample_rad;
  float scale;
  float bias;
  float intensity;
  float unused;
};

struct CBLuminance
{
  float tolerance;
  Vector3 unused;
};

struct CBBlur
{
  Vector2 targetSize;
  Vector2 unused;
};

struct CBShadowParam
{
  Vector2 winSize;
  Vector2 farNear;
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