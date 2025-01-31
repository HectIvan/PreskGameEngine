/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "cmath"
#include "pkPlatformMath.h"
#include "pkVector4.h"

using std::abs;
using std::pow;
using std::sqrt;
using std::lerp;
using std::min;
using std::max;
using std::sin;
using std::asin;
using std::cos;
using std::acos;
using std::atan;
using std::tan;

namespace pkEngineSDK
{

const float PlatformMath::PI = PlatformMath::acos(-1.0f);

const float PlatformMath::EULER = PlatformMath::exp(1.0f);

const float PlatformMath::SMALL_NUMBER = 1e-6f;

const float PlatformMath::DEG2RAD = PI / 180.0f;

const float PlatformMath::RAD2DEG = 180.0f / PI;

float
PlatformMath::abs(const float _x)
{
  return static_cast<float>(std::abs(_x));
}

float
PlatformMath::pow(const float _x, const float _y)
{
  return static_cast<float>(std::pow(_x, _y));
}

float
PlatformMath::exp(const float _x)
{
  return static_cast<float>(std::exp(_x));
}

float
PlatformMath::sqrt(const float _x)
{
  return static_cast<float>(std::sqrt(_x));
}

float
PlatformMath::min(const float _x, const float _y)
{
  return static_cast<float>(std::min(_x, _y));
}

float
PlatformMath::max(const float _x, const float _y)
{
  return static_cast<float>(std::max(_x, _y));
}

float
PlatformMath::sin(const float _x)
{
  return static_cast<float>(std::sin(_x));
}

float
PlatformMath::asin(const float _x)
{
  return static_cast<float>(std::asin(_x));
}

float
PlatformMath::cos(const float _x)
{
  return static_cast<float>(std::cos(_x));
}

float
PlatformMath::acos(const float _x)
{
  return static_cast<float>(std::acos(_x));
}

float
PlatformMath::tan(const float _x)
{
  return static_cast<float>(std::tan(_x));
}

float
PlatformMath::atan(const float _x)
{
  return static_cast<float>(std::atan(_x));
}

float
PlatformMath::clamp(const float _t, const float _x, const float _y)
{
  float t = _t;
  if (t < _x) { t = _x; }
  if (t > _y) { t = _y; }
  return t;
}

float
PlatformMath::lerp(const float _x, const float _y, const float _t)
{
  return std::lerp(_x, _y, _t);
}

Vector3
PlatformMath::lerp3(const Vector3 _x, const Vector3 _y, const float _t)
{
  float x = std::lerp(_x.x, _y.x, _t);
  float y = std::lerp(_x.y, _y.y, _t);
  float z = std::lerp(_x.z, _y.z, _t);
  return Vector3(x, y, z);
}

Vector4
PlatformMath::lerp4(const Vector4 _x, const Vector4 _y, const float _t)
{
  float x = std::lerp(_x.x, _y.x, _t);
  float y = std::lerp(_x.y, _y.y, _t);
  float z = std::lerp(_x.z, _y.z, _t);
  float w = std::lerp(_x.w, _y.w, _t);
  return Vector4(x, y, z, w);
}

/**
 * Sphere
**/
bool
PlatformMath::intersectSphereSphere(Sphere& _sphere, const Sphere& _other)
{
  float distance = _sphere.m_origin.distanceTo(_other.m_origin);

  return (distance < (_sphere.m_radius + _other.m_radius));
}

bool
PlatformMath::intersectSpherePoint(Sphere& _sphere, const Vector3& _other)
{
  float distance = _sphere.m_origin.distanceTo(_other);

  return (distance < _sphere.m_radius);
}

/**
 * Capsule
**/
bool
PlatformMath::intersectCapsuleCapsule(Capsule& _capsule, const Capsule& _other)
{
  return false;
}

bool
PlatformMath::intersectCapsulePoint(Capsule& _capsule, Vector3& _other)
{
  /**
   * check if its near the 2 half spheres
  **/
  if (_capsule.m_topCircle.distanceTo(_other) < _capsule.m_radius ||
      _capsule.m_bottomCircle.distanceTo(_other) < _capsule.m_radius)
  {
    return true;
  }
  /**
   * if not near the 2 half spheres, check if it may be inside of the cylinder
  **/
  else
  {
    Vector3 cylinderAxis = _capsule.m_topCircle - _capsule.m_bottomCircle;
    Vector3 pointToBase = _other - _capsule.m_bottomCircle;

    // projection of pointToBase onto the axis of the cylinder
    float projection = pointToBase.dotProd(cylinderAxis) /
      cylinderAxis.dotProd(cylinderAxis);

    projection = PlatformMath::clamp(projection, 0.0f, 1.0f);

    // gets entire axis and multiplies by the brojection gotten
    Vector3 closestAxisToPoint = _capsule.m_bottomCircle + cylinderAxis * projection;

    // gets distance between point in axis and the _other point
    float distAtoOther = closestAxisToPoint.distanceTo(_other);

    return distAtoOther <= _capsule.m_radius;
  }
}

/**
 * Cube
**/

bool
PlatformMath::intersectCubeCube(Cube& _cube, const Cube& _other)
{
  float sizeComb = _cube.m_size + _other.m_size;

  if (PlatformMath::abs(_cube.m_origin.x - _other.m_origin.x) < sizeComb)
  {
    if (PlatformMath::abs(_cube.m_origin.y - _other.m_origin.y) < sizeComb)
    {
      if (PlatformMath::abs(_cube.m_origin.z - _other.m_origin.z) < sizeComb)
      {
        return true;
      }
    }
  }
  return false;
}

bool
PlatformMath::intersectCubePoint(Cube& _cube, const Vector3& _other)
{
  if (PlatformMath::abs(_cube.m_origin.x - _other.x) < _cube.m_size)
  {
    if (PlatformMath::abs(_cube.m_origin.y - _other.y) < _cube.m_size)
    {
      if (PlatformMath::abs(_cube.m_origin.z - _other.z) < _cube.m_size)
      {
        return true;
      }
    }
  }
  return false;
}

bool
PlatformMath::isNan(float _x)
{
  return std::isnan(_x);
}
}
