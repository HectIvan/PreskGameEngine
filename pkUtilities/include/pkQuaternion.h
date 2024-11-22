/************************************************************************/
/**
* @pkQuaternion pkQuaternion.h
* @Hector Ivan Muñoz Ceballos
* @date 22/09/2024
* @Quaternion file for the Presk Game Engine.
*
* This file contains the Quaternion class of the engine
*
* @bug No bug known.
*
* @HectIvan 22/09/2024
* Created file and added header comments
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesUtilities.h"

namespace pkEngineSDK {

class PK_UTILITY_EXPORT Quaternion
{
 public:
  Quaternion() = default;
  ~Quaternion() = default;

  /**
  * Invert Quaternion.
  *
  * This function inverts the x, y and z values of the quaternion.
  *
  **/
  void
  invert();

  /**
  * magnitude Square Quaternion.
  *
  * This function returns the sum of all the values multiplied
  * with eachother.
  * 
  * @return
  * The sum of elements elevated by 2.
  **/
  float
  magnitudeSquare() const;

  /**
  * magnitude Quaternion.
  *
  * This function returns the magnitude of the Quaternion.
  *
  * @return
  * The magnitude of this quaternion.
  **/
  float
  magnitude() const;

  /**
  * Normalize Quaternion.
  *
  * This function normalizes the  quaternion.
  *
  **/
  void
  normalize();

  float x, y, z, w;
};
}