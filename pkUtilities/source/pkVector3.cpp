/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkMath.h"
#include "pkMatrix4.h"
#include "pkVector3.h"
#include "pkQuaternion.h"

namespace pkEngineSDK {

const Vector3 Vector3::ZERO(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::FORWARD(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::BACK(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::LEFT(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::RIGHT(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UP(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::DOWN(0.0f, -1.0f, 0.0f);

const Vector3
Vector3::operator*(const Quaternion& _other) const
{
  Vector3 vectQuat(_other.x, _other.y, _other.z);

  Vector3 t = vectQuat.cross(*this) * 2.0f;

  return *this + t * _other.w + vectQuat.cross(t);
}

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

const Vector3
Vector3::operator^(const Vector3& other) const
{
  return cross(other);
}

const Vector3
Vector3::cross(const Vector3& _other) const
{
  return Vector3((y * _other.z) - (z * _other.y),
                 (z * _other.x) - (x * _other.z),
                 (x * _other.y) - (y * _other.x));
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
Vector3::safeNormalize()
{
  float mag = magnitude();
  if (mag == 0.0f) {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    return;
  }
  mag = 1.0f / mag;
  x *= mag;
  y *= mag;
  z *= mag;
}

void
Vector3::normalize()
{
  float mag = magnitude();
  mag = 1.0f / mag;
  // mag = Math::abs(mag);
  x *= mag;
  y *= mag;
  z *= mag;
}

const Vector3
Vector3::normalized() const
{
  float mag = magnitude();
  mag = 1.0f / mag;
  return Vector3(x * mag, y * mag, z * mag);
}

float
Vector3::distanceTo(const Vector3& _other)
{
  return Math::sqrt(Math::pow((_other.x - x), 2) + 
                    Math::pow((_other.y - y), 2) + 
                    Math::pow((_other.z - z), 2));
}

void
Vector3::clamp(const float& _x, const float& _y)
{
  x = Math::clamp(x, _x, _y);
  y = Math::clamp(y, _x, _y);
  z = Math::clamp(z, _x, _y);
}

const Vector3
Vector3::reflect(const Vector3& _direction, const Vector3& _normal)
{
  return _direction + (_normal * -2.0f) * ((Vector3::dotProd(_direction, _normal)));
}

float
Vector3::dotProd(const Vector3& _this, const Vector3& _other)
{
  return (_this.x * _other.x) + (_this.y * _other.y) + (_this.z * _other.z);
}

float
Vector3::dotProd(const Vector3& _this, const Vector4& _other)
{
  return (_this.x * _other.x) + (_this.y * _other.y) + (_this.z * _other.z);
}

bool
Vector3::isZero() const
{
  if (!(x == 0.0f)) { return false; }
  if (!(y == 0.0f)) { return false; }
  if (!(z == 0.0f)) { return false; }
  return true;
}

bool
Vector3::hasNan() const
{
  if (Math::isNan(x)) { return true; }
  if (Math::isNan(y)) { return true; }
  if (Math::isNan(z)) { return true; }
  return false;
}
Vector3
Vector3::random()
{
  return Vector3(static_cast<float>(rand()), 
                 static_cast<float>(rand()), 
                 static_cast<float>(rand()));
}
Vector3 Vector3::randomRange(int32 _x, int32 _y)
{
  return Vector3(static_cast<float>(_x + (rand() % _y)),
                 static_cast<float>(_x + (rand() % _y)),
                 static_cast<float>(_x + (rand() % _y)));
}

Vector3
Vector3::randomRange(float _x, float _y)
{
  float tx = _x + static_cast <float> (rand()) / _y;
  float ty = _x + static_cast <float> (rand()) / _y;
  float tz = _x + static_cast <float> (rand()) / _y;
  return Vector3(tx, ty, tz);
}
}
