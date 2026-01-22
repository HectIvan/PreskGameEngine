/*****************************************************************************/
/**
 * @file    pkQuaternion.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    22/09/2024
 * @brief   Presk Engine Quaternion file.
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

  /**
   * @brief Negate the quaternion.
   * @return The negated quaternion.
   */
  PKFORCEINLINE const Quaternion
  operator-() const {
    return Quaternion(-w, -x, -y, -z);
  }

  /**
   * @brief Subtract another quaternion from this one.
   * @param _other other quaternion to subtract.
   * @return Ending quaternion.
   */
  const Quaternion
  operator-(const Quaternion& _other) const;

  /**
   * @brief Subtract another quaternion from this one.
   * @param _other other quaternion to subtract.
   * @return Ending quaternion.
   */
  const Quaternion
  operator-=(const Quaternion& _other);

  /**
   * @brief Add another quaternion to this one.
   * @param _other other quaternion to add.
   * @return Ending quaternion.
   */
  const Quaternion
  operator+=(const Quaternion& _other);

  /**
   * @brief Add another quaternion to this one.
   * @param _other other quaternion to add.
   * @return Ending quaternion.
   */
  const Quaternion
  operator+(const Quaternion& _other) const;

  /**
   * @brief multiply the quaternion with another.
   * @param _other other quaternion to use.
   * @return Ending quaternion.
   */
  const Quaternion
  operator*(const Quaternion& _other) const;

  /**
   * @brief Multiply the quaternion with a scalar.
   * @param _scalar Scalar to use.
   * @return Ending quaternion.
   */
  const Quaternion
  operator*(const float& _scalar) const;

  /**
   * @brief Multiply this quaternion with another.
   * @param _other other quaternion to use.
   * @return Ending quaternion.
   */
  const Quaternion
  operator*=(const Quaternion& _other);

  /**
   * @brief Multiply this quaternion with a scalar.
   * @param _scalar Scalar to use.
   * @return Ending quaternion.
   */
  const Quaternion
  operator*=(const float& _scalar);

  /**
   * @brief Multiply the quaternion with a vector.
   * @param _vector Vector to use.
   * @return Ending quaternion.
   */
  Quaternion
  operator*(const Vector3& _vector) const;

  // construct the quaternion that rotates one vector to another
  Quaternion(const Vector3& _vFrom, const Vector3& _vTo);
  // angle+axis, axis must be normalized
  Quaternion(const float& _angleRadian, const Vector3& _axis);
  // create quaternion from axis and angle
  static Quaternion
  axisAngle(const Vector3& _axis, const float& _angle);
  
  /**
   * @brief Rotate a vector by this quaternion
   * @param _vector Vector to rotate.
   * @return Rotated vector.
   */
  const Vector3
  rotate(const Vector3& _vector) const;

  /**
   * @brief Rotate the quaternion respective to another quaternion.
   * @param _quat Quaternion to use for rotation.
   * @return The rotated quaternion
   */
  Quaternion
  rotate(const Quaternion& _quat) const;

  /**
   * @brief Create a quaternion from euler angles.
   * @param _vector Vector3 containing the euler angles in radians.
   * @return The created quaternion.
   */
  const Quaternion
  fromEuler(const Vector3& _vector) const;
  
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
  const Quaternion
  invert();

  /**
   * @brief Normalize the Quaternion.
   */
  const Quaternion
  normalize();

  /**
   * @brief Get the normalized version of this quaternion.
   * @return The normalized quaternion.
   */
  const Quaternion
  normalized() const;

  /**
   * @brief Get the quaternion as a Vector3.
   */
  Vector3
  xyz() const
  {
    return Vector3(x, y, z);
  }

 public:
  float w;
  float x;
  float y;
  float z;
};
}