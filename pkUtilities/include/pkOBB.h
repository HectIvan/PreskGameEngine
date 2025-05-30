/************************************************************************/
/**
* @pkOBB pkOBB.h
* @Hector Ivan Muñoz Ceballos
* @date 27/09/2024
* @OBB file for the Presk Game Engine.
*
* This file contains the OBB class of the engine
*
* @bug No bug known.
*
* @HectIvan 27/09/2024
* Added comments and class
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesUtilities.h"
#include "pkMatrix4.h"
#include "pkVector3.h"

namespace pkEngineSDK {

class PK_UTILITY_EXPORT OBB
{
 public:
  OBB() = default;
  ~OBB() = default;

  Matrix4 m_transform;
  Vector3 m_halfSize;
};
}