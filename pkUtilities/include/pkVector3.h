/*****************************************************************************/
/**
 * @file    pkVector3.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    06/09/2024
 * @brief   Vector3 class of the engine
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

namespace pkEngineSDK {

class Matrix4;
class Vector2;
class Vector4;
class Quaternion;

class PK_UTILITY_EXPORT Vector3
{
 public:
  Vector3() = default;
  PKFORCEINLINE explicit Vector3(float _val) : x(_val), y(_val), z(_val) {};
  PKFORCEINLINE Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
  ~Vector3() = default;

  /**
   * @brief Check if this vector is the same as another.
   */
  PKFORCEINLINE bool
  operator==(const Vector3& _other)
  {
    return ((x == _other.x) ||
            (y == _other.y) ||
            (z == _other.z)) ? true : false;
  }

  /**
   * @brief Check if this vector is different from another.
   */
  PKFORCEINLINE bool
  operator!=(const Vector3& _other)
  {
    return ((x != _other.x) ||
            (y != _other.y) ||
            (z != _other.z)) ? true : false;
  }

  /**
  * @brief Multiply a vector to this vector.
  * @param other Vector with which the multiplication will be made.
  * @return This vector with the multiplication done.
  **/
  PKFORCEINLINE Vector3&
  operator*=(const Vector3& other)
  {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
  }

  /**
   * @brief Multiply a float to this vector.
   * @param other float with which the multiplication will be made.
   * @return This vector with the multiplication done.
   */
  PKFORCEINLINE Vector3&
  operator*=(const float other)
  {
    x *= other;
    y *= other;
    z *= other;
    return *this;
  }

  /**
   * @brief Multiply a vector to a vector.
   * @param other Vector with which the multiplication will be made.
   * @return A vector with the multiplication done.
   */
  PKFORCEINLINE const Vector3
  operator*(const Vector3& other) const
  {
    return Vector3(x * other.x, y * other.y, z * other.z);
  }

  /**
   * @brief Multiply a vector to a float.
   * @param other float with which the multiplication will be made.
   * @return A vector with the multiplication done.
   */
  PKFORCEINLINE const Vector3
  operator*(const float& other) const
  {
    return Vector3(x * other, y * other, z * other);
  }

  /**
   * @brief Multiply the evctor with a quaternion.
   * @param _other Quaternion with which the multiplication will be made.
   * @return Multiplied vector
   */
  const Vector3
  operator*(const Quaternion& _other) const;
  
  /**
   * Divide a vector to this vector.
   * @param other Vector with which the division will be made.
   * @return This vector with the division done.
   */
  PKFORCEINLINE Vector3&
  operator/=(const Vector3& other)
  {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    return *this;
  }

  /**
   * Divide a float to this vector.
   * @param other float with which the division will be made.
   * @return This vector with the division done.
   */
  PKFORCEINLINE Vector3&
  operator/=(const float& other)
  {
    x /= other;
    y /= other;
    z /= other;
    return *this;
  }

  /**
   * @brief Divide a vector to a vector.
   * @param other Vector with which the division will be made.
   * @return A vector with the division done.
   */
  PKFORCEINLINE const Vector3
  operator/(const Vector3& other) const
  {
    return Vector3(x / other.x, y / other.y, z / other.z);
  }

  /**
   * @brief Divide a float to a vector.
   * @param other float with which the division will be made.
   * @return A vector with the division done.
   */
  PKFORCEINLINE const Vector3
  operator/(const float& other) const
  {
    return Vector3(x / other, y / other, z / other);
  }
  
  /**
   * @brief Add a vector to this vector.
   * @param other Vector with which the addition will be made.
   * @return This vector with the addition done.
   */
  PKFORCEINLINE Vector3&
  operator+=(const Vector3& other)
  {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  /**
   * Adds a float to this vector.
   * @param other float with which the addition will be made.
   * @return This vector with the addition done.
   */
  PKFORCEINLINE Vector3&
  operator+=(const float& other)
  {
    x += other;
    y += other;
    z += other;
    return *this;
  }

  /**
   * @brief Add another vector to a vector.
   * @param other Vector with which the addition will be made.
   * @return A vector with the addition done.
   */
  PKFORCEINLINE const Vector3
  operator+(const Vector3& other) const
  {
    return Vector3(x + other.x, y + other.y, z + other.z);
  }

  /**
   * @brief Adds a float to a vector.
   * @param other float with which the addition will be made.
   * @return A vector with the addition done.
   */
  PKFORCEINLINE const Vector3
  operator+(const float& other) const
  {
    return Vector3(x + other, y + other, z + other);
  }

  /**
   * @brief Negates the values of this vector.
   * @return A Vector with all of its parts negated.
   */
  PKFORCEINLINE const Vector3
  operator-() const
  {
    return Vector3(-x, -y, -z);
  }

  /**
   * @brief Substract a vector to this Vectors values.
   * @param other vector with which the substraction will be made.
   * @return
   * This vector with the substraction done.
   */
  PKFORCEINLINE Vector3&
  operator-=(const Vector3& other)
  {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  /**
   * @brief Substract a float to this vector.
   * @param other float with which the substraction will be made.
   * @return
   * This vector with the substraction done.
   */
  PKFORCEINLINE Vector3&
  operator-=(const float& other)
  {
    x -= other;
    y -= other;
    z -= other;
    return *this;
  }

  /**
   * @brief This operator substracts a vector to this vector.
   * @param other Vector with which the substraction will be made.
   * @return A vector with the substraction done.
   */
  PKFORCEINLINE const Vector3
  operator-(const Vector3& other) const
  {
    return Vector3(x - other.x, y - other.y, z - other.z);
  }

  /**
   * @brief This operator substracts a float to this vector.
   * @param other float with which the substraction will be made.
   * @return A vector with the substraction done.
   */
  PKFORCEINLINE const Vector3
  operator-(const float& other) const
  {
    return Vector3(x - other, y - other, z - other);
  }

  /**
   * @brief This operator compares this vector with another.
   * @param other Vector with which the comparison will be made.
   * @return A boolean determining if they are the same.
   */
  PKFORCEINLINE bool
  operator==(const Vector3& other) const
  {
    return (x != other.x ||
            y != other.y ||
            z != other.z);
  }

  const Vector3
  operator*(const Matrix4& other) const;

  /**
   * @brief Cross product between 2 Vector3.
   * @param other Other Vector3.
   * @return The cross poduct.
   */
  PKFORCEINLINE const Vector3
  operator^(const Vector3& other) const;

  /**
   * @brief This function calculates the square result of this vectors values.
   * @return The final result of the square operation.
   */
  PKFORCEINLINE float
  magnitudeSquare() const;

  /**
   * @brief This function calculates the magnitude of this vector by calculating
   * the square root of the vector multiplied by itself.
   * @return The magnitude of the vector.
   */
  PKFORCEINLINE float
  magnitude() const;

  /**
   * @brief This function normalizes this vector by setting it in a range between 1 and 0,
   * making sure that 0 is not done.
   */
  void
  safeNormalize();

  /**
   * @brief This function normalizes this vector by setting it in a range between 1 and 0.
   */
  void 
  normalize();

  /**
   * @brief Return the normalized vector.
   */
  const Vector3
  normalized() const;

  /**
   * @brief Check if the vector is a zero vector.
   * @return Wether the vector is 0 or not.
   */
  PKFORCEINLINE bool
  isZero() const;

  /**
   * @brief Get a Vector3 with random values.
   * @return A random Vector3.
   */
  Vector3
  random();

  /**
   * @brief Get a random Vector3 in between ranges.
   * @param _x Minimum value.
   * @param _y Maximum value.
   * @return Rando Vector3 in range.
   */
  Vector3
  randomRange(int32 _x, int32 _y);

  /**
   * @brief Get a random Vector3 in between ranges.
   * @param _x Minimum value.
   * @param _y Maximum value.
   * @return Rando Vector3 in range.
   */
  Vector3
  randomRange(float _x, float _y);

  /**
   * @brief Get the XY values of the vector.
   */
  PK_NODISCARD const Vector2
  xy() const;

  /**
   * @brief Get the YZ values of the vector.
   */
  PK_NODISCARD const Vector2
  yz() const;

 public:
  float x;
  float y;
  float z;

  static const Vector3 ZERO;

  static const Vector3 FORWARD;

  static const Vector3 BACK;

  static const Vector3 RIGHT;

  static const Vector3 LEFT;

  static const Vector3 UP;

  static const Vector3 DOWN;
};
}
