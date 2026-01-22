/*****************************************************************************/
/**
 * @file    pkQuaternion.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    22/09/2024
 * @brief   Presk Engine Quaternion file.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkMath.h"
#include "pkQuaternion.h"

namespace pkEngineSDK
{

Quaternion::Quaternion(const Vector3& _vFrom, const Vector3& _vTo)
{
  // float a = 1 + _vTo.dotProd(_vFrom);

  const Vector3 vec = _vFrom.cross(_vTo);
  x = vec.x;
  y = vec.y;
  z = vec.z;
  normalize();
}

/* q = (sina * x * ​sin(radian / 2),
        sina * y * ​sin(radian / 2),
        sina * z * ​sin(radian / 2),
        cos(radian / 2)) */
Quaternion::Quaternion(const float& _angleRadian, const Vector3& _axis)
{
  const Vector3 normAxis = _axis.normalized();

  const float hAngle = _angleRadian * 0.5f;
  const float sinA = Math::sin(hAngle);
  const float cosA = Math::cos(hAngle);

  x = sinA * normAxis.x;
  y = sinA * normAxis.y;
  z = sinA * normAxis.z;
  w = cosA;
}

const Quaternion
Quaternion::invert()
{
  x *= -1.0f;
  y *= -1.0f;
  z *= -1.0f;
  return *this;
}

const Quaternion
Quaternion::operator-(const Quaternion& _other) const
{
  Quaternion q = *this;
  q.w -= _other.w;
  q.x -= _other.x;
  q.y -= _other.y;
  q.z -= _other.z;
  return q;
}

const Quaternion
Quaternion::operator-=(const Quaternion& _other)
{
  *this = *this - _other;
  return *this;
}

const Quaternion
Quaternion::operator+=(const Quaternion& _other)
{
  *this = *this + _other;
  return *this;
}

const Quaternion
Quaternion::operator+(const Quaternion& _other) const
{
  Quaternion q = *this;
  q.w += _other.w;
  q.x += _other.x;
  q.y += _other.y;
  q.z += _other.z;
  return q;
}

const Quaternion
Quaternion::operator*(const Quaternion& _other) const
{
  Quaternion p = *this;

  const float w1 = w;
  const float x1 = x;
  const float y1 = y;
  const float z1 = z;

  const float w2 = _other.w;
  const float x2 = _other.x;
  const float y2 = _other.y;
  const float z2 = _other.z;

  p.w = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
  p.x = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
  p.y = w1 * y2 + y1 * w2 + z1 * x2 - x1 * z2;
  p.z = w1 * z2 + z1 * w2 + x1 * y2 - y1 * x2;

  return p;
}

const Quaternion
Quaternion::operator*(const float& _scalar) const
{
  Quaternion q = *this;
  q.w *= _scalar;
  q.x *= _scalar;
  q.y *= _scalar;
  q.z *= _scalar;
  return q;
}

const Quaternion
Quaternion::operator*=(const float& _scalar)
{
  *this = *this * _scalar;
  return *this;
}

Quaternion
Quaternion::axisAngle(const Vector3& _axis, const float& _angle)
{
  const float aHalf = _angle * 0.5f;
  float w = Math::cos(aHalf);
  float x = _axis.x * Math::sin(aHalf);
  float y = _axis.y * Math::sin(aHalf);
  float z = _axis.z * Math::sin(aHalf);
  return Quaternion(w, x, y, z);
}

const Vector3
Quaternion::rotate(const Vector3& _vector) const
{
  const Quaternion& p = *this;

  // q = P (x,0)
  Quaternion q;
  q.x = p.w * _vector.x + p.y * _vector.z - p.z * _vector.y;
  q.y = p.w * _vector.y + p.z * _vector.x - p.x * _vector.z;
  q.z = p.w * _vector.z + p.x * _vector.y - p.y * _vector.x;
  
  q.w = -p.x * _vector.x - p.y * _vector.y - p.z * _vector.z;
  
  // r = q P*
  Vector3 r;
  r.x = q.w * -p.x + p.w * q.x - q.y * p.z + q.z * p.y;
  r.y = q.w * -p.y + p.w * q.y - q.z * p.x + q.x * p.z;
  r.z = q.w * -p.z + p.w * q.z - q.x * p.y + q.y * p.x;
  
  // Vector3 r = (p * Quaternion(0.0f, _vector) * p.conjugate());

  // return r;
  return r;
}

const Quaternion
Quaternion::operator*=(const Quaternion& _other)
{
  *this = *this * _other;
  return *this;
}

Quaternion
Quaternion::rotate(const Quaternion& _quat) const
{
  // should unwrap this for efficiency
  return (*this) * _quat * (*this).conjugate();
}

const Quaternion
Quaternion::fromEuler(const Vector3& _vector) const
{
  float cx = Math::cos(_vector.x * 0.5f);
  float sx = Math::sin(_vector.x * 0.5f);
  float cy = Math::cos(_vector.y * 0.5f);
  float sy = Math::sin(_vector.y * 0.5f);
  float cz = Math::cos(_vector.z * 0.5f);
  float sz = Math::sin(_vector.z * 0.5f);

  Quaternion q;
  q.w = cx * cy * cz + sx * sy * sz;
  q.x = sx * cy * cz - cx * sy * sz;
  q.y = cx * sy * cz + sx * cy * sz;
  q.z = cx * cy * sz - sx * sy * cz;
  return q;
}

// same as inverse but makes a new one instead of moddifying the current quaternion.
FORCEINLINE Quaternion
Quaternion::conjugate() const
{
  return Quaternion(w, -x, -y, -z);
}

float
Quaternion::magnitudeSquare() const
{
  return Math::sqrt(w) + Math::sqrt(x) + Math::sqrt(y) + Math::sqrt(z);
}

float
Quaternion::magnitude() const
{
  return Math::sqrt(magnitudeSquare());
}

const Quaternion
Quaternion::normalize()
{
  float mag = magnitude();
  if (mag == 0.0f) { return *this; }
  mag = 1.0f / mag;
  x *= mag;
  y *= mag;
  z *= mag;
  w *= mag;
  return *this;
}

const Quaternion
Quaternion::normalized() const
{
  Quaternion newQuat(w, x, y, z);
  newQuat.normalize();
  return newQuat;
}
}
