/*****************************************************************************/
/**
 * @file    pkPrerequisitesCore.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    N.A.
 * @brief   Prerequisites for the core of the game engine.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkCoreDefines.h"
#include "pkPrerequisitesUtilities.h"
#include "pkVector2.h"
#include "pkVector3.h"
#include "pkVector4.h"
#include "pkMatrix4.h"

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