/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkMath.h"
#include "pkVector3.h"
#include "pkMatrix4.h"

namespace pkEngineSDK {

const Vector3 Vector3::FORWARD(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::RIGHT(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UP(0.0f, 1.0f, 0.0f);

const Vector3
Vector3::operator*(const Matrix4& other) const
{
  // new X value
  float X = (x * other.matrix[0][0]) + 
            (x * other.matrix[0][1]) + 
            (x * other.matrix[0][2]);
  // new Y value
  float Y = (y * other.matrix[1][0]) + 
            (y * other.matrix[1][1]) + 
            (y * other.matrix[1][2]);
  // new Z value
  float Z = (z * other.matrix[2][0]) + 
            (z * other.matrix[2][1]) + 
            (z * other.matrix[2][2]);
  // return the final vector
  return Vector3(X, Y, Z);
}

float
Vector3::dotProd(const Vector3& _other) const
{
  return (x * _other.x) + (y * _other.y) + (z * _other.z);
}

float
Vector3::magnitudeSquare() const
{
  return x * x + y * y + z * z;
}

float
Vector3::magnitude() const
{
  return Math::sqrt(magnitudeSquare());
}

void
Vector3::normalize()
{
  float mag = magnitude();
  mag = 1.0f / mag;
  x *= mag;
  y *= mag;
  z *= mag;
}

float
Vector3::distanceTo(const Vector3& _other)
{
  return Math::sqrt(Math::pow((_other.x - x), 2) + 
                    Math::pow((_other.y - y), 2) + 
                    Math::pow((_other.z - z), 2));
}

void
Vector3::clamp(float _x, float _y)
{
  x = Math::clamp(x, _x, _y);
  y = Math::clamp(y, _x, _y);
  z = Math::clamp(z, _x, _y);
}

float
Vector3::dotProd(const Vector3 _this, const Vector3 _other)
{
  return (_this.x * _other.x) + (_this.y * _other.y) + (_this.z * _other.z);
}

float
Vector3::dotProd(const Vector3 _this, const Vector4 _other)
{
  return (_this.x * _other.x) + (_this.y * _other.y) + (_this.z * _other.z);
}
}
