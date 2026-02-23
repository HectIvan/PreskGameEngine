/*****************************************************************************/
/**
 * @file    pkVector4.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    06/09/2024
 * @brief   Vector of 4 values ordered in an x, y, z, w format for the Presk Game Engine.
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
#include "pkVector2.h"

namespace pkEngineSDK {

class Matrix4;

class PK_UTILITY_EXPORT Vector4
{
 public:
  Vector4() = default;
  PKFORCEINLINE Vector4(float _val) : x(_val), y(_val), z(_val), w(_val) {};
  PKFORCEINLINE Vector4(const Vector3& _vec, const float& _w);
  Vector4(const Vector2& _v1, const Vector2& _v2);
  PKFORCEINLINE Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {};
  ~Vector4() = default;

  /**
   * @brief multiply a Vector to this Vector.
   * @param _other Vector with which the multiplication will be made.
   * @return This vector with the multiplication done.
   */
  PKFORCEINLINE Vector4&
  operator*=(const Vector4& _other)
  {
    *this = *this * _other;
    return *this;
  }
  
  /**
   * @brief multiply a float to this Vector.
   * @param _other float with which the multiplication will be made.
   * @return This vector with the multiplication done.
   */
  PKFORCEINLINE Vector4&
  operator*=(const float _other)
  {
    *this = *this * _other;
    return *this;
  }

  /**
   * @brief multiply a Vector to a Vector.
   * @param _other Vector with which the multiplication will be made.
   * @return A vector with the multiplication done.
   */
  PKFORCEINLINE const Vector4
  operator*(const Vector4& _other) const
  {
    return Vector4(x * _other.x, y * _other.y, z * _other.z, w * _other.w);
  }

  /**
   * @brief multiply a float to a Vector.
   * @param _other float with which the multiplication will be made.¿
   * @return A vector with the multiplication done.
   */
  PKFORCEINLINE const Vector4
  operator*(const float& _other) const
  {
    return Vector4(x * _other, y * _other, z * _other, w * _other);
  }

  /**
   * @brief Adds a Vector3 to a Vector4.
   * @param _other Vector with which the addition will be made.
   * @return A vector with the addition done.
   */
  PKFORCEINLINE Vector4&
  operator+=(const Vector3& _other)
  {
    *this = *this + _other;
    return *this;
  }

  /**
   * @brief add a Vector to this Vector.
   * @param _other Vector with which the addition will be made.
   * @return This vector with the addition done.
   */
  PKFORCEINLINE Vector4&
  operator+=(const Vector4& _other)
  {
    *this = *this + _other;
    return *this;
  }

  /**
   * @brief add a Vector to a Vector.
   * @param _other Vector with which the addition will be made.
   * @return A vector with the addition done.
   */
  PKFORCEINLINE const Vector4
  operator+(const Vector4& _other) const
  {
    return Vector4(x + _other.x, y + _other.y, z + _other.z, w + _other.w);
  }

  /**
   * @brief Adds a Vector3 to a Vector4.
   * @param _other Vector with which the addition will be made.
   * @return A vector with the addition done.
   */
  PKFORCEINLINE const Vector4
  operator+(const Vector3& _other) const
  {
    return Vector4(x + _other.x, y + _other.y, z + _other.z, w);
  }

  /**
   * @brief subtract a Vector to this Vector.
   * @param _other Vector with which the substraction will be made.
   * @return This vector with the substraction done.
   */
  PKFORCEINLINE Vector4&
  operator-=(const Vector4& _other)
  {
    *this = *this - _other;
    return *this;
  }

  /**
   * @brief subtract a float to this Vector.
   * @param _other float with which the substraction will be made.
   * @return This vector with the substraction done.
   */
  PKFORCEINLINE Vector4&
  operator-=(const float& _other)
  {
    *this = *this - _other;
    return *this;
  }

  /**
   * @brief Substract a Vector to this Vector.
   * @param _other Vector with which the substraction will be made.
   * @return A vector with the substraction done.
   */
  PKFORCEINLINE const Vector4
  operator-(const Vector4& _other) const
  {
    return Vector4(x - _other.x, y - _other.y, z - _other.z, w - _other.w);
  }

  /**
   * @brief Substract a float to this Vector.
   * @param _other float with which the substraction will be made.
   * @return A vector with the substraction done.
   */
  PKFORCEINLINE const Vector4
  operator-(const float& _other) const
  {
    return Vector4(x - _other, y - _other, z - _other, w - _other);
  }

  /**
   * @brief Compare this Vector with another Vector.
   * @param _other Vector with which the comparison will be made.
   * @return A boolean determining if they are the same.
   */
  PKFORCEINLINE bool
  operator==(const Vector4& _other) const
  {
    return (x != _other.x ||
            y != _other.y ||
            z != _other.z ||
            w != _other.w);
  }

  /**
   * @brief Compare this Vector with another Vector.
   * @param other Vector with which the comparison will be made.
   * @return A boolean determining if any discrepancies were found.
   */
  PKFORCEINLINE bool
  operator!=(const Vector4& _other) const
  {
    return !operator==(_other);
  }

  Vector4
  operator*(const Matrix4& other) const;
  
  /**
   * @brief Calculate the cross product of this vector.
   * @param _other Vector with which the cross product will be calculated.
   * @return The final result of the cross product as a Vector.
   */
  PKFORCEINLINE const Vector4
  operator^(const Vector4& _other) const
  {
    return vector3Cross(_other);
  }

  /**
   * @brief Negates the values of this vector.
   * @return A Vector with all of its parts negated.
   */
  PKFORCEINLINE const Vector4
  operator-() const
  {
    return Vector4(-x, -y, -z, -w);
  }

  /**
   * @brief Calculate the dot product between 2 vectors.
   * @param _other The vector with which the dot product will be performed.
   * @return The final result of the dot product as a Vector.
   */
  float
  dotProd(const Vector4& _other) const;

  /**
   * @brief Calculate the cross product of this vector.
   * @param _other Vector with which the cross product will be calculated.
   * @return The final result of the cross product as a Vector.
   */
  float
  dotProd3(const Vector4& _other) const;

  /**
   * @brief Calculate the cross product between 2 vectors.
   * @param _other The vector with which the cross product will be performed.
   * @return The final result of the cross product as a Vector.
   */
  Vector4
  vector3Cross(const Vector4& _other) const;

  /**
   * @brief Calculate the square multiplication of this vector.
   * @return The final result of the square operation.
   */
  PKFORCEINLINE float
  magnitudeSquare() const;

  /**
   * @brief Calculate the magnitude of this vector.
   * @return The magnitude of the vector.
   */
  PKFORCEINLINE float
  magnitude() const;

  /**
   * @brief Normalize this vector.
   */
  void
  normalize();

  /**
   * @brief returns a copy of this normalized vector.
   */
  Vector4
  normalized() const;

  /**
   * @brief Get the distance between this vector and another.
   * @param _other the vector to be targeted for distance measuring.
   * @return the distance between the 2 vectors
   */
  PKFORCEINLINE float
  distanceTo(const Vector4& _other) const;

  /**
    * @brief Clamp the values of the Vector4.
    * @param _x Minimum clamp range.
    * @param _y Maximum clamp range.
    */
  PKFORCEINLINE void
  clamp(const float& _x, const float& _y);

  /**
   * @brief Compare 2 vectors to see if their values differ.
   * @param _other The vector with which the comparison will be performed.
   * @return A boolean determining if any value changes.
   */
  bool
  isDifferent(const Vector4& _other) const;

  /**
   * @brief Set a vector to one single value.
   * @param _val The value that all parts of the vector will be set to.
   * @return A Vector4 with all of its parts set to a single value.
   */
  static const Vector4
  vectorSplatValue(const float& _val);

  /**
   * @brief Calculate the dot product of one vector with this one.
   * @param _other The other vector4.
   * @return The final dot product.
   */
  static float
  dotProd(const Vector4& _this, const Vector4& _other);

  /**
   * @brief Get vector3 from this vector.
   * @return A vector3 with the x, y and z of this vector.
   */
  const Vector3
  xyz() const { return Vector3(x, y, z); }

  /**
   * @brief Get a Vector2 with x and y of this vector.
   * @return A vector2 with the x and y of this vector
   */
  const Vector2
  xy() const { return Vector2(x, y); };

  /**
   * @brief Get a Vector2 with y and z of this vector.
   * @return A vector2 with the y and z of this vector
   */
  const Vector2
  yz() const { return Vector2(y, z); };

 public:
  float x, y, z, w;

  static const Vector4 ZERO;

  static const Vector4 FORWARD;

  static const Vector4 BACK;

  static const Vector4 RIGHT;

  static const Vector4 LEFT;

  static const Vector4 UP;

  static const Vector4 DOWN;
};
}
