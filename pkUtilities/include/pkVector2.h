/************************************************************************/
/**
* @pkVector2 pkVector2.h
* @Hector Ivan Muñoz Ceballos
* @date 06/09/2024
* @Vector2 file for the Presk Game Engine.
*
* This file contains the Vector2 class of the engine
*
* @bug No bug known.
*
* @HectIvan 09/09/2024
* Added comments and missing functions
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

class PK_UTILITY_EXPORT Vector2
{
 public:
  Vector2() = default;
  FORCEINLINE explicit Vector2(float _val) : x(_val), y(_val) {};
  FORCEINLINE Vector2(float _x, float _y) : x(_x), y(_y) {};
  ~Vector2() = default;

  /**
  * multiplies a Vector to this Vector.
  *
  * This operator multiplies a vector to this vector.
  *
  * @param other
  * Vector with which the multiplication will be made.
  *
  * @return
  * This vector with the multiplication done.
  **/
  FORCEINLINE Vector2&
  operator*=(const Vector2& other)
  {
    x *= other.x;
    y *= other.y;
    return *this;
  }

  /**
  * multiplies a float to this Vector.
  *
  * This operator multiplies a float to this vector.
  *
  * @param other
  * float with which the multiplication will be made.
  *
  * @return
  * This vector with the multiplication done.
  **/
  FORCEINLINE Vector2&
  operator*=(const float other)
  {
    x *= other;
    y *= other;
    return *this;
  }

  /**
  * multiply a Vector to a Vector.
  *
  * This operator multiplies a vector to a vector.
  *
  * @param other
  * Vector with which the multiplication will be made.
  *
  * @return
  * A vector with the multiplication done.
  **/
  FORCEINLINE const Vector2
  operator*(const Vector2& other) const
  {
    return Vector2(x * other.x, y * other.y);
  }

  /**
  * multiply a float to a Vector.
  *
  * This operator multiplies a float to a vector.
  *
  * @param other
  * float with which the multiplication will be made.
  *
  * @return
  * A vector with the multiplication done.
  **/
  FORCEINLINE const Vector2
  operator*(const float& other) const
  {
    return Vector2(x * other, y * other);
  }

  /**
  * divides a Vector to this Vector.
  *
  * This operator divides a vector to this vector.
  *
  * @param other
  * Vector with which the division will be made.
  *
  * @return
  * This vector with the division done.
  **/
  FORCEINLINE Vector2&
  operator/=(const Vector2& other)
  {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  /**
  * divides a float to this Vector.
  *
  * This operator divides a float to this vector.
  *
  * @param other
  * float with which the division will be made.
  *
  * @return
  * This vector with the division done.
  **/
  FORCEINLINE Vector2&
  operator/=(const float& other)
  {
    x /= other;
    y /= other;
    return *this;
  }

  /**
  * divide a Vector to a Vector.
  *
  * This operator divides a vector to a vector.
  *
  * @param other
  * Vector with which the division will be made.
  *
  * @return
  * A vector with the division done.
  **/
  FORCEINLINE const Vector2
  operator/(const Vector2& other) const
  {
    return Vector2(x / other.x, y / other.y);
  }

  /**
  * divice a float to a Vector.
  *
  * This operator divides a float to a vector.
  *
  * @param other
  * float with which the division will be made.
  *
  * @return
  * A vector with the division done.
  **/
  FORCEINLINE const Vector2
  operator/(const float& other) const
  {
    return Vector2(x / other, y / other);
  }

  /**
  * add a Vector to this Vector.
  *
  * This operator adds a vector to this vector.
  *
  * @param other
  * Vector with which the addition will be made.
  *
  * @return
  * This vector with the addition done.
  **/
  FORCEINLINE Vector2&
  operator+=(const Vector2& other)
  {
    x += other.x;
    y += other.y;
    return *this;
  }

  /**
  * add a float to this Vector.
  *
  * This operator adds a float to this vector.
  *
  * @param other
  * float with which the addition will be made.
  *
  * @return
  * This vector with the addition done.
  **/
  FORCEINLINE Vector2&
  operator+=(const float& other)
  {
    x += other;
    y += other;
    return *this;
  }

  /**
  * add a Vector to a Vector.
  *
  * This operator adds another vector to a vector.
  *
  * @param other
  * Vector with which the addition will be made.
  *
  * @return
  * A vector with the addition done.
  **/
  FORCEINLINE const Vector2
  operator+(const Vector2& other) const
  {
    return Vector2(x + other.x, y + other.y);
  }

  /**
  * add a float to a Vector.
  *
  * This operator adds a float to a vector.
  *
  * @param other
  * float with which the addition will be made.
  *
  * @return
  * A vector with the addition done.
  **/
  FORCEINLINE const Vector2
  operator+(const float& other) const
  {
    return Vector2(x + other, y + other);
  }

  /**
  * Substract a vector to this Vector's values and set the values to this.
  *
  * This operator substracts a vector to this vector.
  *
  * @param other
  * vector with which the substraction will be made.
  *
  * @return
  * This vector with the substraction done.
  **/
  FORCEINLINE Vector2&
  operator-=(const Vector2& other)
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  /**
  * Substract a float to this Vector's values and set the values to this.
  *
  * This operator substracts a float to this vector.
  *
  * @param other
  * float with which the substraction will be made.
  *
  * @return
  * This vector with the substraction done.
  **/
  FORCEINLINE Vector2&
  operator-=(const float& other)
  {
    x -= other;
    y -= other;
    return *this;
  }

  /**
  * Substract a vector to this Vector.
  *
  * This operator substracts a vector to this vector.
  *
  * @param other
  * vector with which the substraction will be made.
  *
  * @return
  * A vector with the substraction done.
  **/
  FORCEINLINE const Vector2
  operator-(const Vector2& other) const
  {
    return Vector2(x - other.x, y - other.y);
  }

  /**
  * Substract a float to this Vector.
  *
  * This operator substracts a float to this vector.
  *
  * @param other
  * float with which the substraction will be made.
  *
  * @return
  * A vector with the substraction done.
  **/
  FORCEINLINE const Vector2
  operator-(const float& other) const
  {
    return Vector2(x - other, y - other);
  }

  /**
  * Compare this Vector with another Vector to find if they are the same.
  *
  * This operator compares this vector with another.
  *
  * @param other
  * Vector with which the comparison will be made.
  *
  * @return
  * A boolean determining if they are the same.
  **/
  FORCEINLINE const bool
    operator==(const Vector2& other) const
  {
    return (x != other.x ||
            y != other.y);
  }

  /**
  * Calculates the dot product between 2 vectors.
  *
  * This function calculates the dot product between this vector
  * and the vector parameter.
  *
  * @param _other
  * The vector with which the dot product will be performed.
  *
  * @return
  * The final result of the dot product as a Vector.
  **/
  float
  dotProd(const Vector2& _other);

  /**
  * Calculates the square multiplication of this vector.
  *
  * This function calculates the square result of this vectors values.
  *
  * @return
  * The final result of the square operation.
  **/
  FORCEINLINE float
  magnitudeSquare() const;

  /**
  * Calculates the magnitude of this vector.
  *
  * This function calculates the magnitude of this vector by calculating
  * the square root of the vector multiplied by itself.
  *
  * @return
  * The magnitude of the vector.
  **/
  FORCEINLINE float
  magnitude() const;

  /**
  * Normalizes this vector.
  *
  * This function normalizes this vector by setting it in a range between
  * 1 and 0.
  **/
  FORCEINLINE void 
  normalize();

  /**
  * clamps the vector.
  *
  * This function clamps the vector between one value and the other
  *
  * @param _x
  * Minimum clamp range.
  *
  * @param _y
  * Maximum clamp range.
  *
  **/
  FORCEINLINE void
  clamp(float _x, float _y);

  /**
  * Gets the distance between this vector and another.
  *
  * This function gets the distance between this vector and
  * another vector
  *
  * @param _other
  * the vector to be targeted for distance measuring
  *
  * @return
  * the distance between the 2 vectors
  **/
  FORCEINLINE float
  distanceTo(const Vector2& _other);

  float x, y;
};
}
