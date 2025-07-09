/*****************************************************************************/
/**
 * @file    pkVector2.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    06/09/2024
 * @brief   Vector2 data type for the Presk Game Engine.
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

class PK_UTILITY_EXPORT Vector2
{
 public:
  Vector2() = default;
  FORCEINLINE explicit Vector2(float _val) : x(_val), y(_val) {};
  FORCEINLINE Vector2(float _x, float _y) : x(_x), y(_y) {};
  FORCEINLINE Vector2(uint32 _x, uint32 _y) : 
    x(static_cast<float>(_x)),
    y(static_cast<float>(_y))
  {};
  ~Vector2() = default;

  /**
   * @brief Multiplies a Vector to this Vector.
   * @param other Vector with which the multiplication will be made.
   * @return This vector with the multiplication done.
   */
  FORCEINLINE Vector2&
  operator*=(const Vector2& other)
  {
    x *= other.x;
    y *= other.y;
    return *this;
  }

  /**
   * @brief Multiplies a float to this Vector.
   * @param other Float with which the multiplication will be made.
   * @return This vector with the multiplication done.
   */
  FORCEINLINE Vector2&
  operator*=(const float other)
  {
    x *= other;
    y *= other;
    return *this;
  }

  /**
   * @brief Multiply a Vector to another Vector.
   * @param other Vector with which the multiplication will be made.
   * @return A vector with the multiplication done.
   */
  FORCEINLINE const Vector2
  operator*(const Vector2& other) const
  {
    return Vector2(x * other.x, y * other.y);
  }

  /**
   * @brief Multiply a float to another Vector.
   * @param other Float with which the multiplication will be made.
   * @return A vector with the multiplication done.
   */
  FORCEINLINE const Vector2
  operator*(const float& other) const
  {
    return Vector2(x * other, y * other);
  }

  /**
   * @brief Divides a Vector to this Vector.
   * @param other Vector with which the division will be made.
   * @return This vector with the division done.
   */
  FORCEINLINE Vector2&
  operator/=(const Vector2& other)
  {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  /**
   * @brief Divides a float to this Vector.
   * @param other float with which the division will be made.
   * @return This vector with the division done.
   */
  FORCEINLINE Vector2&
  operator/=(const float& other)
  {
    x /= other;
    y /= other;
    return *this;
  }

  /**
   * @brief Divide a Vector to another Vector.
   * @param other Vector with which the division will be made.
   * @return A vector with the division done.
   */
  FORCEINLINE const Vector2
  operator/(const Vector2& other) const
  {
    return Vector2(x / other.x, y / other.y);
  }

  /**
   * @brief Divice a float to another vector.
   * @param other Float with which the division will be made.
   * @return A vector with the division done.
   */
  FORCEINLINE const Vector2
  operator/(const float& other) const
  {
    return Vector2(x / other, y / other);
  }

  /**
   * @brief Add a Vector to this Vector.
   * @param other Vector with which the addition will be made.
   * @return This Vector with the addition done.
   */
  FORCEINLINE Vector2&
  operator+=(const Vector2& other)
  {
    x += other.x;
    y += other.y;
    return *this;
  }

  /**
   * @brief Add a float to this Vector.
   * @param other Float with which the addition will be made.
   * @return This Vector with the addition done.
   */
  FORCEINLINE Vector2&
  operator+=(const float& other)
  {
    x += other;
    y += other;
    return *this;
  }

  /**
   * @brief Add a vector to another vector.
   * @param other Vector with which the addition will be made.
   * @return A vector with the addition done.
   */
  FORCEINLINE const Vector2
  operator+(const Vector2& other) const
  {
    return Vector2(x + other.x, y + other.y);
  }

  /**
   * @brief Add a float to another Vector.
   * @param other float with which the addition will be made.
   * @return A vector with the addition done.
   */
  FORCEINLINE const Vector2
  operator+(const float& other) const
  {
    return Vector2(x + other, y + other);
  }

  /**
   * @brief Substract a vector to this Vectors values.
   * @param other vector with which the substraction will be made.
   * @return This vector with the substraction done.
   */
  FORCEINLINE Vector2&
  operator-=(const Vector2& other)
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  /**
   * @brief Substract a float to this Vectors values.
   * @param other float with which the substraction will be made.
   * @return This vector with the substraction done.
   */
  FORCEINLINE Vector2&
  operator-=(const float& other)
  {
    x -= other;
    y -= other;
    return *this;
  }

  /**
   * @brief Substract a vector to this Vector.
   * @param other vector with which the substraction will be made.
   * @return A vector with the substraction done.
   */
  FORCEINLINE const Vector2
  operator-(const Vector2& other) const
  {
    return Vector2(x - other.x, y - other.y);
  }

  /**
   * @brief Substract a float to this Vector.
   * @param other float with which the substraction will be made.
   * @return A vector with the substraction done.
   */
  FORCEINLINE const Vector2
  operator-(const float& other) const
  {
    return Vector2(x - other, y - other);
  }

  /**
   * @brief Compare this Vector with another Vector to find if they are the same.
   * @param other Vector with which the comparison will be made.
   * @return A boolean determining if they are the same.
   */
  FORCEINLINE const bool
  operator==(const Vector2& other) const
  {
    return (x != other.x ||
            y != other.y);
  }

  /**
   * @brief Calculate the dot product between this and another vector.
   * @param _other The vector with which the dot product will be performed.
   * @return The final result of the dot product as a Vector.
   */
  float
  dotProd(const Vector2& _other);

  /**
   * @brief Calculate the square multiplication of this vector.
   * @return The final result of the square operation.
   */
  float
  magnitudeSquare() const;

  /**
   * @brief Calculate the magnitude of this vector.
   * @return The magnitude of the vector.
   */
  float
  magnitude() const;

  /**
   * @brief Normalize this vector.
   */
  void 
  normalize();

  /**
   * @brief Return the normalized vector.
   * @return The normalized vector.
   */
  const Vector2
  normalized() const;

  /**
   * @brief Clamp the vector between 2 values.
   * @param _x Minimum clamp range.
   * @param _y Maximum clamp range.
   *
   */
  void
  clamp(float _x, float _y);

  /**
   * @brief Get the distance between this vector and another.
   * @param _other The vector to be targeted for distance measuring.
   * @return The distance between the 2 vectors.
   */
  float
  distanceTo(const Vector2& _other);

 public:
  float x, y;
};
}
