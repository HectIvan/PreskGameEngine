/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkMath.h"
#include "pkMatrix4.h"
#include "pkVector2.h"

namespace pkEngineSDK {

const Vector4 Vector4::ZERO = Vector4(0);

Vector4::Vector4(Vector3 _vec, float _w)
{
  x = _vec.x;
  y = _vec.y;
  z = _vec.z;
  w = _w;
}

const Vector4
Vector4::operator*(const Matrix4& other) const
{
  // new X value
  float X = (x * other.matrix[0][0]) + 
            (x * other.matrix[1][0]) + 
            (x * other.matrix[2][0]);
  // new Y value
  float Y = (y * other.matrix[0][1]) + 
            (y * other.matrix[1][1]) + 
            (y * other.matrix[2][1]);
  // new Z value
  float Z = (z * other.matrix[0][2]) + 
            (z * other.matrix[1][2]) + 
            (z * other.matrix[2][2]);
  // new W value
  float W = (w * other.matrix[0][3]) + 
            (w * other.matrix[1][3]) + 
            (w * other.matrix[2][3]);
  // return the final vector
  return Vector4(X, Y, Z, W);
}

float
Vector4::dotProd(const Vector4& _other) const
{
  return (x * _other.x) + (y * _other.y) + (z * _other.z) + (w * _other.w);
}

float
Vector4::dotProd3(const Vector4& _other) const
{
  return (x * _other.x) + (y * _other.y) + (z * _other.z);
}

Vector4 
Vector4::vector3Cross(const Vector4& _other) const
{
  return Vector4((y * _other.z) - (z * _other.y),
                 (z * _other.x) - (x * _other.z),
                 (x * _other.y) - (y * _other.x),
                 0.0f);
}

float 
Vector4::magnitudeSquare() const
{
  return x * x + y * y + z * z + w * w;
}

float 
Vector4::magnitude() const
{
  return Math::sqrt(magnitudeSquare());
}

void 
Vector4::normalize()
{
  float mag = magnitude();
  if (mag == 0.0f) { return; }
  mag = 1.0f / mag;
  x *= mag;
  y *= mag;
  z *= mag;
  w *= mag;
}

Vector4
Vector4::normalized() const
{
  float mag = magnitude();
  if (mag == 0.0f) { return Vector4(0.0f); }
  mag = 1.0f / mag;
  return Vector4(x * mag, y * mag, z * mag, w * mag);
}

float
Vector4::distanceTo(const Vector4& _other) const
{
  return Math::sqrt(Math::pow((_other.x - x), 2) +
                    Math::pow((_other.y - y), 2) +
                    Math::pow((_other.z - z), 2) +
                    Math::pow((_other.w - w), 2));
}

void
Vector4::clamp(float _x, float _y)
{
  x = Math::clamp(x, _x, _y);
  y = Math::clamp(y, _x, _y);
  z = Math::clamp(z, _x, _y);
  w = Math::clamp(w, _x, _y);
}

bool
Vector4::isDifferent(const Vector4& _other) const
{
  if ((x != _other.x) ||
      (y != _other.y) ||
      (z != _other.z) ||
      (w != _other.w))
  { 
    return true;
  }
  return false;
}

Vector4 
Vector4::vectorSplatValue(float _val)
{
  Vector4 result;
  result.x = _val;
  result.y = _val;
  result.z = _val;
  result.w = _val;
  return result;
}

float
Vector4::dotProd(const Vector4& _this, const Vector4& _other)
{
  return (_this.x * _other.x) +
         (_this.y * _other.y) +
         (_this.z * _other.z) + 
         (_this.w * _other.w);
}

Vector2
Vector4::xy()
{
  return Vector2(x, y);
}

Vector2
Vector4::yz()
{
  return Vector2(y, z);
}
}
