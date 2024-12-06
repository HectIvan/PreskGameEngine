#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesUtilities.h"
#include "pkVector3.h"
#include "pkVector4.h"

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