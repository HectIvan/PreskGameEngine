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
  Vector4 eye;
  Vector3 forward;
  Matrix4 view;
  Matrix4 projection;
  float unused;
};

struct CBLight
{
  float Type;
  float SpotCutoff;
  float SpotExponent;
  Vector3 LightDir;
  Vector3 LightPos;
  Vector3 LightColor;
  Vector2 unused1;
  Vector2 unused2;
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