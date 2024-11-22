#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesUtilities.h"

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

struct Texture {
  uint32 id;
  String type;
};

template<class T>
void safeRelease(T* x) { if (x) { x->Release(); x = nullptr; } }
}