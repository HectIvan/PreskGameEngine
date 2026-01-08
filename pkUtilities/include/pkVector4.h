/************************************************************************/
/**
* @pkVector4 pkVector4.h
* @Hector Ivan Muñoz Ceballos
* @date 06/09/2024
* @Vector4 file for the Presk Game Engine.
*
* This file contains the Vector4 class of the engine
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
#include "pkVector3.h"

namespace pkEngineSDK {

class Matrix4;
class Vector2;

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
  * multiply a Vector to this Vector.
  *
  * This operator multiplies another vector to this vector.
  *
  * @param other
  * Vector with which the multiplication will be made.
  *
  * @return
  * This vector with the multiplication done.
  **/
  PKFORCEINLINE Vector4&
  operator*=(const Vector4& other)
  {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    w *= other.w;
    return *this;
  }
  
  /**
  * multiply a float to this Vector.
  *
  * This operator multiplies a float to this vector.
  *
  * @param other
  * float with which the multiplication will be made.
  *
  * @return
  * This vector with the multiplication done.
  **/
  PKFORCEINLINE Vector4&
  operator*=(const float other)
  {
    x *= other;
    y *= other;
    z *= other;
    w *= other;
    return *this;
  }

  /**
  * multiply a Vector to a Vector.
  *
  * This operator multiplies another vector to a vector.
  *
  * @param other
  * Vector with which the multiplication will be made.
  *
  * @return
  * A vector with the multiplication done.
  **/
  PKFORCEINLINE const Vector4
  operator*(const Vector4& other) const
  {
    return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
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
  PKFORCEINLINE const Vector4
  operator*(const float& other) const
  {
    return Vector4(x * other, y * other, z * other, w * other);
  }

  /**
   * @brief Adds a Vector3 to a Vector4.
   * @param other Vector with which the addition will be made.
   * @return A vector with the addition done.
   */
  PKFORCEINLINE Vector4&
  operator+=(const Vector3& other)
  {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  /**
  * add a Vector to this Vector.
  *
  * This operator adds another vector to this vector.
  *
  * @param other
  * Vector with which the addition will be made.
  *
  * @return
  * This vector with the addition done.
  **/
  PKFORCEINLINE Vector4&
  operator+=(const Vector4& other)
  {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
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
  PKFORCEINLINE const Vector4
  operator+(const Vector4& other) const
  {
    return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
  }

  /**
  * @brief Adds a Vector3 to a Vector4.
  * @param other Vector with which the addition will be made.
  * @return A vector with the addition done.
  **/
  PKFORCEINLINE const Vector4
  operator+(const Vector3& other) const
  {
    return Vector4(x + other.x, y + other.y, z + other.z, w);
  }

  /**
  * Substract a Vector to this Vector4's values and set the values to this.
  *
  * This operator substracts a Vector4 to this vector.
  *
  * @param other
  * Vector with which the substraction will be made.
  *
  * @return
  * This vector with the substraction done.
  **/
  PKFORCEINLINE Vector4&
  operator-=(const Vector4& other)
  {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
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
  PKFORCEINLINE Vector4&
  operator-=(const float& other)
  {
    x -= other;
    y -= other;
    z -= other;
    w -= other;
    return *this;
  }

  /**
  * Substract a Vector to this Vector.
  *
  * This operator substracts another vector to this vector.
  *
  * @param other
  * Vector with which the substraction will be made.
  *
  * @return
  * A vector with the substraction done.
  **/
  PKFORCEINLINE const Vector4
  operator-(const Vector4& other) const
  {
    return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
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
  PKFORCEINLINE const Vector4
  operator-(const float& other) const
  {
    return Vector4(x - other, y - other, z - other, w - other);
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
  PKFORCEINLINE bool
  operator==(const Vector4& other) const
  {
    return (x != other.x ||
            y != other.y ||
            z != other.z ||
            w != other.w);
  }

  /**
  * Compare this Vector with another Vector to find if they are different.
  *
  * This operator compares this vector with another
  *
  * @param other
  * Vector with which the comparison will be made.
  *
  * @return
  * A boolean determining if any discrepancies were found.
  **/
  PKFORCEINLINE bool
  operator!=(const Vector4& other) const
  {
    return !operator==(other);
  }

  Vector4
  operator*(const Matrix4& other) const;
  
  /**
  * Calculate the cross product of this vector.
  *
  * This operator calculates the cross product between this vector
  * and another Vector.
  * 
  * @param other
  * Vector with which the cross product will be calculated.
  *
  * @return
  * The final result of the cross product as a Vector.
  **/
  PKFORCEINLINE const Vector4
  operator^(const Vector4& other)
  {
    return vector3Cross(other);
  }

  /**
  * Negates the values of this vector.
  *
  * This operator negates all the values of this vector.
  *
  * @return
  * A Vector with all of its parts negated.
  **/
  PKFORCEINLINE const Vector4
  operator-() const
  {
    return Vector4(-x, -y, -z, -w);
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
  dotProd(const Vector4& _other) const;

  /**
  * Calculate the cross product of this vector.
  *
  * This operator calculates the cross product between this vector
  * and another Vector using only x, y and z.
  *
  * @param _other
  * Vector with which the cross product will be calculated.
  *
  * @return
  * The final result of the cross product as a Vector.
  **/
  float
  dotProd3(const Vector4& _other) const;

  /**
  * Calculates the cross product between 2 vectors.
  *
  * This function calculates the cross product between this vector
  * and the vector parameter, returning a Vector with its w value set at 0.
  *
  * @param _other
  * The vector with which the cross product will be performed.
  *
  * @return
  * The final result of the cross product as a Vector.
  **/
  Vector4
  vector3Cross(const Vector4& _other) const;

  /**
  * Calculates the square multiplication of this vector.
  *
  * This function calculates the square result of this vectors values.
  *
  * @return
  * The final result of the square operation.
  **/
  PKFORCEINLINE float
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
  PKFORCEINLINE float
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
   * @brief returns a copy of this normalized vector.
   */
  Vector4
  normalized() const;

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
  PKFORCEINLINE float
  distanceTo(const Vector4& _other) const;

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
  PKFORCEINLINE void
  clamp(float _x, float _y);

  /**
  * Compares 2 vectors to see if their values differ.
  *
  * This function compares this vector with another one, to see if
  * any of their values differ between each other.
  *
  * @param _other
  * The vector with which the comparison will be performed.
  *
  * @return
  * A boolean determining if any value changes.
  **/
  bool
  isDifferent(const Vector4& _other) const;

  /**
  * Sets all parts of a vector to a single value.
  *
  * This function takes all the parts of the vector and sets
  * them to a single value
  *
  * @param _val
  * The value that all parts of the vector will be set to.
  *
  * @return
  * A Vector4 with all of its parts set to a single value.
  **/
  static Vector4
  vectorSplatValue(float _val);

  /**
  * Calculate the dot product of one vector with this one.
  *
  * @param _other
  * The other vector4.
  *
  * @return
  * The final dot product.
  **/
  static float
  dotProd(const Vector4& _this, const Vector4& _other);

  /**
   * @brief Get vector3 from this vector.
   * @return A vector3 with the x, y and z of this vector.
   */
  const Vector3
  xyz() { return Vector3(x, y, z); }

  /**
   * @brief Get a Vector2 with x and y of this vector.
   * @return A vector2 with the x and y of this vector
   */
  Vector2
  xy();

  /**
   * @brief Get a Vector2 with y and z of this vector.
   * @return A vector2 with the y and z of this vector
   */
  Vector2
  yz();

 public:
  float x, y, z, w;

  static const Vector4 ZERO;
};
}
