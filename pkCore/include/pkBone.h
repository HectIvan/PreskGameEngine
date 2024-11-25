/************************************************************************/
/**
* @pkBone pkBone.h
* @Hector Ivan Muñoz Ceballos
* @date 23/11/2024
* @Bone file for the Presk Game Engine.
*
* This file will contain the bone used for the engine
*
* @bug No bug known.
*
* @HectIvan 23/11/2024
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include <assimp/anim.h>

#include "pkMatrix4.h"
#include "pkPrerequisitesCore.h"

namespace pkEngineSDK
{

class Bone
{
 public:
  Bone() = default;
  virtual ~Bone() = default;
};
}