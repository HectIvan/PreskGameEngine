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
  float a = 1 + _vTo.dotProd(_vFrom);

  Vector3 vec = _vFrom.cross(_vTo);
  x = vec.x;
  y = vec.y;
  z = vec.z;
  normalize();
}

Quaternion::Quaternion(float _angleRadian, const Vector3& _axis)
{
  float sina = Math::sin(_angleRadian / 2.f);
  float a = Math::cos(_angleRadian / 2.f);

  x = sina * _axis.x;
  y = sina * _axis.y;
  z = sina * _axis.z;
}

void
pkEngineSDK::Quaternion::invert()
{
  x *= -1.0f;
  y *= -1.0f;
  z *= -1.0f;
}

inline Quaternion
Quaternion::operator*(const Quaternion& _quat) const
{
  const Quaternion& p = *this;
  Quaternion r;

  // Quaternion formulas:
  // r.a = p.a * q.a - Dot(p.v,q.v);
  // r.v = p.a * q.v + q.a * p.v + Cross(p.v,q.v);

  r.w = p.w * _quat.w
        - p.x * _quat.x - p.y * _quat.y - p.z * _quat.z;

  r.x = p.w * _quat.x + _quat.w * p.x
        + p.y * _quat.z - p.z * _quat.y;

  r.y = p.w * _quat.y + _quat.w * p.y
        + p.z * _quat.x - p.x * _quat.z;

  r.z = p.w * _quat.z + _quat.w * p.z
        + p.x * _quat.y - p.y * _quat.x;

  return r;
}

Quaternion
Quaternion::axisAngle(Vector3 _axis, float _angle)
{
  float w = Math::cos(_angle / 2);
  float x = _axis.x * Math::sin(_angle / 2);
  float y = _axis.y * Math::sin(_angle / 2);
  float z = _axis.z * Math::sin(_angle / 2);
  return Quaternion(w, x, y, z);
}

Vector3
Quaternion::rotate(const Vector3& x) const
{
  const Quaternion& p = *this;

  // q = P (x,0)
  Quaternion q;
  q.x = p.w * x.x + p.y * x.z - p.z * x.y;
  q.y = p.w * x.y + p.z * x.x - p.x * x.z;
  q.z = p.w * x.z + p.x * x.y - p.y * x.x;

  q.w = -p.x * x.x - p.y * x.y - p.z * x.z;

  // r = q P*
  Vector3 r;
  r.x = q.w * -p.x + p.w * q.x - q.y * p.z + q.z * p.y;
  r.y = q.w * -p.y + p.w * q.y - q.z * p.x + q.x * p.z;
  r.z = q.w * -p.z + p.w * q.z - q.x * p.y + q.y * p.x;

  return r;
}

inline Quaternion
Quaternion::operator*=(const Quaternion& _quat)
{
  *this = *this * _quat;
  return *this;
}

void
Quaternion::operator+=(const Quaternion& _quat)
{
  w += _quat.w;
  x += _quat.x;
  y += _quat.y;
  z += _quat.z;
}

inline Quaternion
Quaternion::rotate(const Quaternion& _quat) const
{
  // should unwrap this for efficiency
  return (*this) * _quat * (*this).conjugate();
}

// same as inverse but makes a new one instead of moddifying the current quaternion.
inline Quaternion
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

void
Quaternion::normalize()
{
  float mag = magnitude();
  if (mag == 0.0f) { return; }
  mag = 1.0f / mag;
  x *= mag;
  y *= mag;
  z *= mag;
  w *= mag;
}

Quaternion
Quaternion::normalized()
{
  Quaternion newQuat(w, x, y, z);
  newQuat.normalize();
  return newQuat;
}

Quaternion
Quaternion::expMap(const Vector3& _vector) {
  float angle = Math::sqrt(_vector.x * _vector.x +
                           _vector.y * _vector.y +
                           _vector.z * _vector.z);

  if (angle < Math::SMALL_NUMBER) {
    // Small-angle approximation
    return Quaternion(1.0f, _vector.x * 0.5f, _vector.y * 0.5f, _vector.z * 0.5f);
  }
  else {
    Vector3 axis = Vector3(_vector.x / angle, _vector.y / angle, _vector.z / angle);
    return Quaternion::fromAxisAngle(axis, angle);
  }
}

Quaternion
Quaternion::fromBiVector(const Vector3& _bivector)
{
  float angle = Math::sqrt(_bivector.x * _bivector.x +  _bivector.y *
                           _bivector.y + _bivector.z * _bivector.z);

  if (angle < Math::SMALL_NUMBER) {
    // Use small-angle approximation
    return Quaternion(1.0f, 0.5f * _bivector.x, 0.5f * _bivector.y, 0.5f * _bivector.z);
  }
  else {
    Vector3 axis = Vector3(_bivector.x / angle, _bivector.y / angle, _bivector.z / angle);

    float half = 0.5f * angle;
    float sinHalf = std::sin(half);
    return Quaternion(Math::cos(half), axis.x * sinHalf, axis.y * sinHalf, axis.z * sinHalf);
  }
}

Quaternion
Quaternion::fromAxisAngle(const Vector3& _axis, float _angle)
{
  Vector3 normAxis = _axis.normalized(); // ensure it's a unit vector
  float halfAngle = 0.5f * _angle;
  float sinHalf = Math::sin(halfAngle);

  return Quaternion(Math::cos(halfAngle),
                    normAxis.x * sinHalf,
                    normAxis.y * sinHalf,
                    normAxis.z * sinHalf);
}
}
