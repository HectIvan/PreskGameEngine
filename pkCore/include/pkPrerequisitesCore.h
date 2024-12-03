#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesUtilities.h"

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

//struct Texture {
//  uint32 id;
//  uint32 type;
//};

template<class T>
void safeRelease(T* x) { if (x) { x->Release(); x = nullptr; } }
}