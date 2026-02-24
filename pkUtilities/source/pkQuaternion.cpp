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

const Quaternion Quaternion::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f);

Quaternion::Quaternion(const Vector3& _vFrom, const Vector3& _vTo)
{
  // float a = 1 + _vTo.dotProd(_vFrom);
  const Vector3 f = _vFrom.normalized();
  const Vector3 t = _vTo.normalized();

  const float FdT = f.dotProd(t);

  if (FdT >= 1.0f) {
    // vectors are the same
    w = 1.0f;
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
  }
  // if vectors are opposite.
  else if (FdT <= -1.0f) {
    Vector3 ortho = Vector3::RIGHT.cross(f);
    if (ortho.magnitudeSquare() < 0.0001f) {
      ortho = Vector3::UP.cross(f);
    }
    ortho.normalize();
    w = 0.0f;
    x = ortho.x;
    y = ortho.y;
    z = ortho.z;
  }
  else {
    const Vector3 cross = f.cross(t);
    w = 1.0f + FdT;
    x = cross.x;
    y = cross.y;
    z = cross.z;
  }
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

Quaternion
Quaternion::fromAxisAngle(const Vector3& _axis, const float& _angle)
{
  const Vector3 axis = _axis.normalized();
  const float aHalf = _angle * 0.5f;

  float w = Math::cos(aHalf);
  float x = axis.x * Math::sin(aHalf);
  float y = axis.y * Math::sin(aHalf);
  float z = axis.z * Math::sin(aHalf);

  return Quaternion(w, x, y, z);
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
/*
             a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,  // 1
             a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,  // i
             a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,  // j
             a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w   // k
*/
const Quaternion
Quaternion::operator*(const Quaternion& _other) const
{
  Quaternion result = Quaternion::IDENTITY;

  const float aw = w;
  const float ax = x;
  const float ay = y;
  const float az = z;

  const float bw = _other.w;
  const float bx = _other.x;
  const float by = _other.y;
  const float bz = _other.z;

  result.w = aw * bw - ax * bx - ay * by - az * bz;
  result.x = aw * bx + ax * bw + ay * bz - az * by;
  result.y = aw * by - ax * bz + ay * bw + az * bx;
  result.z = aw * bz + ax * by - ay * bx + az * bw;

  return result;
}

const Vector3
Quaternion::operator*(const Vector3& _other) const
{
  Vector3 vectQuat(x, y, z);
  Vector3 t = vectQuat.cross(_other) * 2.0f;
  return _other + t * w + vectQuat.cross(t);
}

bool
Quaternion::hasNan() const
{
  return Math::isNan(w) || Math::isNan(x) || Math::isNan(y) || Math::isNan(z);
}

// https://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
// 1.- p=(vx,vy,vz,0)⇔p=(v,0)
// 2.- p′=q×p×q∗
// 3.- v′=(p′x,p′y,p′z)
const Vector3
Quaternion::rotate(const Vector3& _vector) const
{
  const Quaternion PureFromVector = Quaternion(0.0f, _vector.x, _vector.y, _vector.z);
  
  const Quaternion pure = (*this) * PureFromVector * this->conjugate();

  return Vector3(pure.x, pure.y, pure.z);
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

// const Vector3
// Quaternion::rotate(const Vector3& _vector) const
// {
//   const Quaternion& p = *this;
// 
//   // q = P (x,0)
//   Quaternion q;
//   q.x = p.w * _vector.x + p.y * _vector.z - p.z * _vector.y;
//   q.y = p.w * _vector.y + p.z * _vector.x - p.x * _vector.z;
//   q.z = p.w * _vector.z + p.x * _vector.y - p.y * _vector.x;
//   
//   q.w = -p.x * _vector.x - p.y * _vector.y - p.z * _vector.z;
//   
//   // r = q P*
//   Vector3 r;
//   r.x = q.w * -p.x + p.w * q.x - q.y * p.z + q.z * p.y;
//   r.y = q.w * -p.y + p.w * q.y - q.z * p.x + q.x * p.z;
//   r.z = q.w * -p.z + p.w * q.z - q.x * p.y + q.y * p.x;
//   
//   // Vector3 r = (p * Quaternion(0.0f, _vector) * p.conjugate());
// 
//   // return r;
//   return r;
// }

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
Quaternion::fromEuler(const Vector3& _vector)
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

/**
 source: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
 */
const Vector3
Quaternion::toEuler() const
{
  Vector3 euler;

  const Quaternion q = *this;

 
  const float sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
  const float cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
  euler.x = Math::atan2(sinr_cosp, cosr_cosp);

  
  const float sinp = Math::sqrt(1.0f + 2.0f * (q.w * q.y - q.x * q.z));
  const float cosp = Math::sqrt(1.0f - 2.0f * (q.w * q.y - q.x * q.z));
  euler.y = 2.0f * Math::atan2(sinp, cosp) - Math::PI * 0.5f;

  
  const float siny_cosp = 2.0f * (q.w * q.z + q.x * q.y);
  const float cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
  euler.z = Math::atan2(siny_cosp, cosy_cosp);

  return euler;
}

Quaternion
Quaternion::conjugate() const
{
  return Quaternion(w, -x, -y, -z);
}

float
Quaternion::magnitudeSquare() const
{
  return (w * w) + (x * x) + (y * y) + (z * z);
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
  if (mag < Math::SMALL_NUMBER) {
    assert(!isnan(mag));
    *this = Quaternion::IDENTITY;
    return *this;
  }
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
