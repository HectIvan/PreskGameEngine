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
#include "pkVector3.h"

namespace pkEngineSDK {

class PK_UTILITY_EXPORT Quaternion
{
 public:
  Quaternion() = default;
  ~Quaternion() = default;

  Quaternion(float _w, float _x, float _y, float _z) :
    w(_w),
    x(_x),
    y(_y),
    z(_z) {}

  // construct the quaternion that rotates one vector to another
  Quaternion(const Vector3& _vFrom, const Vector3& _vTo);
  // angle+axis, axis must be normalized
  Quaternion(float _angleRadian, const Vector3& _axis);
  // create quaternion from axis and angle
  static Quaternion
  axisAngle(Vector3 _axis, float _angle);
  
  /**
   * @brief Rotate a vector by this quaternion
   * @param _vector Vector to rotate.
   * @return Rotated vector.
   */
  Vector3
  rotate(const Vector3& _vector) const;
  
  /**
   * @brief multiply the quaternion with another.
   * @param _quat other quaternion to use.
   * @return Ending quaternion.
   */
  Quaternion
  operator*(const Quaternion& _quat) const;

  /**
   * @brief Multiply this quaternion with another.
   * @param _quat other quaternion to use.
   * @return Ending quaternion.
   */
  Quaternion
  operator*=(const Quaternion& _quat);

  /**
   * @brief Multiply the quaternion with a vector.
   */
  Quaternion
  operator*(const Vector3& _vector) const
  {

  }

  /**
   * @brief Rotate the quaternion respective to another quaternion.
   * @param _quat Quaternion to use for rotation.
   * @return The rotated quaternion
   */
  Quaternion
  rotate(const Quaternion& _quat) const;
  
  // length utility
  Quaternion
  conjugate() const;
  /**
   * @brief Get the magnitude Square of the Quaternion.
   * @return The sum of elements elevated by 2.
   */
  float
  magnitudeSquare() const;
  /**
   * @brief Get the magnitude  of the Quaternion.
   * @return The sum of elements.
   */
  float
  magnitude() const;

  /**
  * @brief Invert the x, y and z values of the quaternion.
  */
  void
  invert();

  /**
   * @brief Normalize the Quaternion.
   */
  void
  normalize();

  /**
   * @brief Get the normalized version of this quaternion.
   * @return The normalized quaternion.
   */
  Quaternion
  normalized();

  /**
   * @brief Get the quaternion as a Vector3.
   */
  Vector3
  xyz() const
  {
    return Vector3(x, y, z);
  }

  float w;
  float x;
  float y;
  float z;
};
}