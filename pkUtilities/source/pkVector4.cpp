/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkMath.h"
#include "pkMatrix4.h"

namespace pkEngineSDK {

const Vector4 Vector4::ZERO = Vector4(0);
const Vector4 Vector4::FORWARD(0.0f, 0.0f, 1.0f, 0.0f);
const Vector4 Vector4::BACK(0.0f, 0.0f, -1.0f, 0.0f);
const Vector4 Vector4::LEFT(-1.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::RIGHT(1.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::UP(0.0f, 1.0f, 0.0f, 0.0f);
const Vector4 Vector4::DOWN(0.0f, -1.0f, 0.0f, 0.0f);

Vector4::Vector4(const Vector3& _vec, const float& _w)
{
  x = _vec.x;
  y = _vec.y;
  z = _vec.z;
  w = _w;
}

Vector4::Vector4(const Vector2& _v1, const Vector2& _v2)
{
  x = _v1.x;
  y = _v1.y;
  z = _v2.x;
  w = _v2.y;
}

Vector4
Vector4::operator*(const Matrix4& _other) const
{
  float X = x * _other.matrix[0][0] +
            y * _other.matrix[1][0] +
            z * _other.matrix[2][0] +
            w * _other.matrix[3][0];

  float Y = x * _other.matrix[0][1] +
            y * _other.matrix[1][1] +
            z * _other.matrix[2][1] +
            w * _other.matrix[3][1];

  float Z = x * _other.matrix[0][2] +
            y * _other.matrix[1][2] +
            z * _other.matrix[2][2] +
            w * _other.matrix[3][2];

  float W = x * _other.matrix[0][3] +
            y * _other.matrix[1][3] +
            z * _other.matrix[2][3] +
            w * _other.matrix[3][3];

  return Vector4(X, Y, Z, W);
}

PKFORCEINLINE const Vector4
Vector4::operator^(const Vector4& _other) const
{
  return Math::cross(*this, _other);
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

const Vector4
Vector4::normalized() const
{
  float mag = magnitude();
  if (mag == 0.0f) { return Vector4(0.0f); }
  mag = 1.0f / mag;
  return Vector4(x * mag, y * mag, z * mag, w * mag);
}
}
