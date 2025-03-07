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
class Vector4;

class PK_UTILITY_EXPORT Vector3
{
 public:
  Vector3() = default;
  FORCEINLINE explicit Vector3(float _val) : x(_val), y(_val), z(_val) {};
  FORCEINLINE Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
  ~Vector3() = default;

  /**
  * @brief Multiply a vector to this vector.
  * @param other Vector with which the multiplication will be made.
  * @return This vector with the multiplication done.
  **/
  FORCEINLINE Vector3&
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
  FORCEINLINE Vector3&
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
  FORCEINLINE const Vector3
  operator*(const Vector3& other) const
  {
    return Vector3(x * other.x, y * other.y, z * other.z);
  }

  /**
   * @brief Multiply a float to a vector.
   * @param other float with which the multiplication will be made.
   * @return A vector with the multiplication done.
   */
  FORCEINLINE const Vector3
  operator*(const float& other) const
  {
    return Vector3(x * other, y * other, z * other);
  }
  
  /**
   * Divide a vector to this vector.
   * @param other Vector with which the division will be made.
   * @return This vector with the division done.
   */
  FORCEINLINE Vector3&
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
  FORCEINLINE Vector3&
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
  FORCEINLINE const Vector3
  operator/(const Vector3& other) const
  {
    return Vector3(x / other.x, y / other.y, z / other.z);
  }

  /**
   * @brief Divide a float to a vector.
   * @param other float with which the division will be made.
   * @return A vector with the division done.
   */
  FORCEINLINE const Vector3
  operator/(const float& other) const
  {
    return Vector3(x / other, y / other, z / other);
  }
  
  /**
   * @brief Add a vector to this vector.
   * @param other Vector with which the addition will be made.
   * @return This vector with the addition done.
   */
  FORCEINLINE Vector3&
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
  FORCEINLINE Vector3&
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
  FORCEINLINE const Vector3
  operator+(const Vector3& other) const
  {
    return Vector3(x + other.x, y + other.y, z + other.z);
  }

  /**
   * @brief Adds a float to a vector.
   * @param other float with which the addition will be made.
   * @return A vector with the addition done.
   */
  FORCEINLINE const Vector3
  operator+(const float& other) const
  {
    return Vector3(x + other, y + other, z + other);
  }

  /**
   * @brief Substract a vector to this Vectors values.
   * @param other vector with which the substraction will be made.
   * @return
   * This vector with the substraction done.
   */
  FORCEINLINE Vector3&
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
  FORCEINLINE Vector3&
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
  FORCEINLINE const Vector3
  operator-(const Vector3& other) const
  {
    return Vector3(x - other.x, y - other.y, z - other.z);
  }

  /**
   * @brief This operator substracts a float to this vector.
   * @param other float with which the substraction will be made.
   * @return A vector with the substraction done.
   */
  FORCEINLINE const Vector3
  operator-(const float& other) const
  {
    return Vector3(x - other, y - other, z - other);
  }

  /**
   * @brief This operator compares this vector with another.
   * @param other Vector with which the comparison will be made.
   * @return A boolean determining if they are the same.
   */
  FORCEINLINE const bool
  operator==(const Vector3& other) const
  {
    return (x != other.x ||
            y != other.y ||
            z != other.z);
  }

  const Vector3
  operator*(const Matrix4& other) const;

  FORCEINLINE const Vector3
  operator^(const Vector3& other) const;

  /**
   * @brief Get the cross product between this vector and another.
   * @param _other The other vector.
   * @return The cross Vector.
   */
  Vector3
  cross(const Vector3& _other) const;

  /**
   * @brief This function calculates the dot product between this vector
   * and the vector parameter.
   * @param _other The vector with which the dot product will be performed.
   * @return The final result of the dot product as a Vector.
   */
  float 
  dotProd(const Vector3& _other) const;

  /**
   * @brief This function calculates the square result of this vectors values.
   * @return The final result of the square operation.
   */
  FORCEINLINE float
  magnitudeSquare() const;

  /**
   * @brief This function calculates the magnitude of this vector by calculating
   * the square root of the vector multiplied by itself.
   * @return The magnitude of the vector.
   */
  FORCEINLINE float
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
   * @Brief This function gets the distance between this vector and
   * another vector
   * @param _other the vector to be targeted for distance measuring
   * @return the distance between the 2 vectors
   */
  FORCEINLINE float
  distanceTo(const Vector3& _other);

  /**
   * @Brief This function clamps the vector between one value and the other
   * @param _x Minimum clamp range.
   * @param _y Maximum clamp range.
   */
  FORCEINLINE void
  clamp(float _x, float _y);

  /**
   * @Brief Gets the reflected vector off a normal.
   * @param _direction The vector to reflect.
   * @param _normal The normal of the surface collision.
   * @return The reflected vector.
   */
  static Vector3
  reflect(Vector3 _direction, Vector3 _normal);

  /**
   * @Brief Calculate the dot product of one vector with this one.
   * @param _this The vector where the dot product is performed.
   * @param _other The other vector3.
   * @return The final dot product.
   */
  FORCEINLINE static float
  dotProd(const Vector3 _this, const Vector3 _other);

  /**
   * @Brief Calculate the dot product of one vector with this one.
   * @param _other The other vector4.
   * @return The final dot product.
   */
  FORCEINLINE static float
  dotProd(const Vector3 _this, const Vector4 _other);

  /**
   * @brief Check if the vector is a zero vector.
   * @return Wether the vector is 0 or not.
   */
  FORCEINLINE bool
  isZero();

  /**
   * @brief Check if the vector has a non number.
   * @return wether the vector has or does not have a nan.
   */
  FORCEINLINE bool
  hasNan();

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