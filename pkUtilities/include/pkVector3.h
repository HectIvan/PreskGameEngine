/************************************************************************/
/**
* @pkVector3 pkVector3.h
* @Hector Ivan Muñoz Ceballos
* @date 06/09/2024
* @Vector3 file for the Presk Game Engine.
*
* This file contains the Vector3 class of the engine
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
  FORCEINLINE Vector3&
  operator*=(const Vector3& other)
  {
    x *= other.x;
    y *= other.y;
    z *= other.z;
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
  FORCEINLINE Vector3&
  operator*=(const float other)
  {
    x *= other;
    y *= other;
    z *= other;
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
  FORCEINLINE const Vector3
  operator*(const Vector3& other) const
  {
    return Vector3(x * other.x, y * other.y, z * other.z);
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
  FORCEINLINE const Vector3
  operator*(const float& other) const
  {
    return Vector3(x * other, y * other, z * other);
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
  FORCEINLINE Vector3&
  operator/=(const Vector3& other)
  {
    x /= other.x;
    y /= other.y;
    z /= other.z;
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
  FORCEINLINE Vector3&
  operator/=(const float& other)
  {
    x /= other;
    y /= other;
    z /= other;
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
  FORCEINLINE const Vector3
  operator/(const Vector3& other) const
  {
    return Vector3(x / other.x, y / other.y, z / other.z);
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
  FORCEINLINE const Vector3
  operator/(const float& other) const
  {
    return Vector3(x / other, y / other, z / other);
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
  FORCEINLINE Vector3&
  operator+=(const Vector3& other)
  {
    x += other.x;
    y += other.y;
    z += other.z;
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
  FORCEINLINE Vector3&
  operator+=(const float& other)
  {
    x += other;
    y += other;
    z += other;
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
  FORCEINLINE const Vector3
  operator+(const Vector3& other) const
  {
    return Vector3(x + other.x, y + other.y, z + other.z);
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
  FORCEINLINE const Vector3
  operator+(const float& other) const
  {
    return Vector3(x + other, y + other, z + other);
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
  FORCEINLINE Vector3&
  operator-=(const Vector3& other)
  {
    x -= other.x;
    y -= other.y;
    z -= other.z;
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
  FORCEINLINE Vector3&
  operator-=(const float& other)
  {
    x -= other;
    y -= other;
    z -= other;
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
  FORCEINLINE const Vector3
  operator-(const Vector3& other) const
  {
    return Vector3(x - other.x, y - other.y, z - other.z);
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
  FORCEINLINE const Vector3
  operator-(const float& other) const
  {
    return Vector3(x - other, y - other, z - other);
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
  operator==(const Vector3& other) const
  {
    return (x != other.x ||
            y != other.y ||
            z != other.z);
  }

  const Vector3
  operator*(const Matrix4& other) const;

  /**
  * Calculates the dot product between 2 vectors.
  *
  * This function calculates the dot product between this vector
  * and the vector parameter.
  *
  * @param other
  * The vector with which the dot product will be performed.
  *
  * @return
  * The final result of the dot product as a Vector.
  **/
  float 
  dotProd(const Vector3& _other) const;

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
  void 
  normalize();

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
  distanceTo(const Vector3& _other);

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
  * Calculate the dot product of one vector with this one.
  * 
  * @param _this
  * The vector where the dot product is performed.
  * 
  * @param _other
  * The other vector3.
  * 
  * @return
  * The final dot product.
  **/
  FORCEINLINE static float
  dotProd(const Vector3 _this, const Vector3 _other);

  /**
  * Calculate the dot product of one vector with this one.
  *
  * @param _other
  * The other vector4.
  *
  * @return
  * The final dot product.
  **/
  FORCEINLINE static float
  dotProd(const Vector3 _this, const Vector4 _other);

  float x, y, z;

  static const Vector3 FORWARD;
  static const Vector3 RIGHT;
  static const Vector3 UP;
};
}