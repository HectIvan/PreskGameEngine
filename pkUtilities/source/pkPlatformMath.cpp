/*****************************************************************************/
/**
 * @file    pkPlatformMath.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    06/09/2024
 * @brief   Basic math used for the game engine
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPlatformMath.h"
#include "pkVector4.h"
#include "pkMatrix4.h"
#include "pkQuaternion.h"

#include <cmath>
#include <algorithm>
/*
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
using std::tan;*/

namespace pkEngineSDK
{

const float Math::PI = Math::acos(-1.0f);

const float Math::EULER = Math::exp(1.0f);

const float Math::SMALL_NUMBER = 1e-6f;

const float Math::DEG2RAD = PI / 180.0f;

const float Math::RAD2DEG = 180.0f / PI;

float
PlatformMath::abs(const float& _x)
{
  return static_cast<float>(std::abs(_x));
}

float
PlatformMath::pow(const float& _x, const float& _y)
{
  return static_cast<float>(std::pow(_x, _y));
}

float
PlatformMath::exp(const float& _x)
{
  return static_cast<float>(std::exp(_x));
}

float
PlatformMath::sqrt(const float& _x)
{
  return static_cast<float>(std::sqrt(_x));
}

uint32
PlatformMath::min(const uint32& _x, const uint32& _y)
{
  return static_cast<uint32>(std::min(_x, _y));
}

float
PlatformMath::min(const float& _x, const float& _y)
{
  return static_cast<float>(std::min(_x, _y));
}

uint32
PlatformMath::max(const uint32& _x, const uint32& _y)
{
  return static_cast<uint32>(std::max(_x, _y));
}

float
PlatformMath::max(const float& _x, const float& _y)
{
  return std::max(_x, _y);
}

float
PlatformMath::maxf(const float& _x, const float& _y)
{
  return static_cast<float>(std::max(_x, _y));
}

float
PlatformMath::sin(const float& _x)
{
  return static_cast<float>(std::sin(_x));
}

float
PlatformMath::sinf(const float& _x)
{
  return static_cast<float>(std::sinf(_x));
}

float
PlatformMath::asin(const float& _x)
{
  return static_cast<float>(std::asin(_x));
}

float
PlatformMath::cos(const float& _x)
{
  return static_cast<float>(std::cos(_x));
}

float
PlatformMath::acos(const float& _x)
{
  return static_cast<float>(std::acos(_x));
}

float
PlatformMath::tan(const float& _x)
{
  return static_cast<float>(std::tan(_x));
}

float
PlatformMath::atan(const float& _x)
{
  return static_cast<float>(std::atan(_x));
}

float
PlatformMath::atan2(const float& _x, const float& _y)
{
  return static_cast<float>(std::atan2(_x, _y));
}

bool
PlatformMath::isNan(const float& _x)
{
  return std::isnan(_x);
}

bool
PlatformMath::isNan(const Vector2& _x)
{
  return (std::isnan(_x.x) || std::isnan(_x.y)) ? true : false;
}

bool
PlatformMath::isNan(const Vector3& _x)
{
  return (std::isnan(_x.x) || std::isnan(_x.y) || std::isnan(_x.z)) ? true : false;
}

bool
PlatformMath::isNan(const Vector4& _x)
{
  return (std::isnan(_x.x) ||
          std::isnan(_x.y) ||
          std::isnan(_x.z) ||
          std::isnan(_x.w)) ? true : false;
}

bool
PlatformMath::isNan(const Quaternion& _x)
{
  return (std::isnan(_x.x) ||
          std::isnan(_x.y) ||
          std::isnan(_x.z) ||
          std::isnan(_x.w)) ? true : false;
}

bool
PlatformMath::isInf(const float& _x)
{
  return std::isinf(_x);
}

bool
PlatformMath::isInf(const Vector2& _x)
{
  return (std::isinf(_x.x) || std::isinf(_x.y)) ? true : false;
}

bool
PlatformMath::isInf(const Vector3& _x)
{
  return (std::isinf(_x.x) || std::isinf(_x.y) || std::isinf(_x.z)) ? true : false;
}

bool
PlatformMath::isInf(const Vector4& _x)
{
  return (std::isinf(_x.x) ||
          std::isinf(_x.y) ||
          std::isinf(_x.z) ||
          std::isinf(_x.w)) ? true : false;
}

bool
PlatformMath::isInf(const Quaternion& _x)
{
  return (std::isinf(_x.x) ||
          std::isinf(_x.y) ||
          std::isinf(_x.z) ||
          std::isinf(_x.w)) ? true : false;
}

float
PlatformMath::log2(const float& _x)
{
  return std::log2f(_x);
}

float
PlatformMath::clamp(const float& _t, const float& _x, const float& _y)
{
  float t = _t;
  if (t < _x) { t = _x; }
  if (t > _y) { t = _y; }
  return t;
}

Vector2
PlatformMath::clamp(const Vector2& _t, const float& _x, const float& _y)
{
  Vector2 vec;
  vec.x = std::clamp(_t.x, _x, _y);
  vec.y = std::clamp(_t.y, _x, _y);
  return vec;
}

Vector3
PlatformMath::clamp(const Vector3& _t, const float& _x, const float& _y)
{
  Vector3 vec;
  vec.x = std::clamp(_t.x, _x, _y);
  vec.y = std::clamp(_t.y, _x, _y);
  vec.z = std::clamp(_t.z, _x, _y);
  return vec;
}

Vector4
PlatformMath::clamp(const Vector4& _t, const float& _x, const float& _y)
{
  Vector4 vec;
  vec.x = std::clamp(_t.x, _x, _y);
  vec.y = std::clamp(_t.y, _x, _y);
  vec.z = std::clamp(_t.z, _x, _y);
  vec.w = std::clamp(_t.w, _x, _y);
  return vec;;
}


bool
PlatformMath::isInRange(const float& _t, const float& _x, const float& _y)
{
  if (_t >= _x && _t <= _y) {
    return true;
  }
  return false;
}

bool
PlatformMath::isInRange(const uint32& _t, const uint32& _x, const uint32& _y)
{
  if (_t >= _x && _t <= _y) {
    return true;
  }
  return false;
}

float
PlatformMath::lerp(const float& _x, const float& _y, const float& _t)
{
  return std::lerp(_x, _y, _t);
}

Vector2
PlatformMath::lerp(const Vector2& _x, const Vector2& _y, const float& _t)
{
  float x = std::lerp(_x.x, _y.x, _t);
  float y = std::lerp(_x.y, _y.y, _t);
  return Vector2(x, y);
}

Vector3
PlatformMath::lerp(const Vector3& _x, const Vector3& _y, const float& _t)
{
  float x = std::lerp(_x.x, _y.x, _t);
  float y = std::lerp(_x.y, _y.y, _t);
  float z = std::lerp(_x.z, _y.z, _t);
  return Vector3(x, y, z);
}

Vector4
PlatformMath::lerp(const Vector4& _x, const Vector4& _y, const float& _t)
{
  float x = std::lerp(_x.x, _y.x, _t);
  float y = std::lerp(_x.y, _y.y, _t);
  float z = std::lerp(_x.z, _y.z, _t);
  float w = std::lerp(_x.w, _y.w, _t);
  return Vector4(x, y, z, w);
}

float
PlatformMath::hookeLaw(const float& _elasticity, const float& _displacement)
{
  // @source: https://www.britannica.com/science/Hookes-law
  // F = KX
  return _elasticity * _displacement;
}

float
PlatformMath::dotProd(const Vector2& _x, const Vector2& _y)
{
  return (_x.x * _y.x) +
         (_x.y * _y.y);
}

float
PlatformMath::dotProd(const Vector3& _x, const Vector3& _y)
{
  return (_x.x * _y.x) +
         (_x.y * _y.y) +
         (_x.z * _y.z);
}

float
PlatformMath::dotProd(const Vector4& _x, const Vector4& _y)
{
  return (_x.x * _y.x) +
         (_x.y * _y.y) +
         (_x.z * _y.z) + 
         (_x.w * _y.w);
}

Vector3
PlatformMath::cross(const Vector3& _x, const Vector3& _y)
{
  return Vector3((_x.y * _y.z) - (_x.z * _y.y),
                 (_x.z * _y.x) - (_x.x * _y.z),
                 (_x.x * _y.y) - (_x.y * _y.x));
}

Vector4
PlatformMath::cross(const Vector4& _x, const Vector4& _y)
{
  return Vector4((_x.y * _y.z) - (_x.z * _y.y),
                 (_x.z * _y.x) - (_x.x * _y.z),
                 (_x.x * _y.y) - (_x.y * _y.x),
                 0.0f);
}

Vector2
PlatformMath::reflect(const Vector2& _direction, const Vector3& _normal)
{
  const Vector3 direction = Vector3(_direction.x, _direction.y, 0.0f);
  const Vector3 normal = Vector3(_normal.x, _normal.y, 0.0f).normalized();
  const Vector3 reflected = reflect(direction, normal);
  return Vector2(reflected.x, reflected.y);
}

Vector3
PlatformMath::reflect(const Vector3& _direction, const Vector3& _normal)
{
  const Vector3 normal = _normal.normalized();
  return _direction + (normal * -2.0f) * ((Math::dotProd(_direction, normal)));
}

float
PlatformMath::distance(const Vector2& _x, const Vector2& _y)
{
  return std::sqrt(std::pow((_y.x - _x.x), 2) +
                   std::pow((_y.y - _x.y), 2));
}

float
PlatformMath::distance(const Vector3& _x, const Vector3& _y)
{
  return std::sqrt(std::pow((_y.x - _x.x), 2) +
                   std::pow((_y.y - _x.y), 2) + 
                   std::pow((_y.z - _x.z), 2));
}

float
PlatformMath::distance(const Vector4& _x, const Vector4& _y)
{
  return std::sqrt(std::pow((_y.x - _x.x), 2) +
                   std::pow((_y.y - _x.y), 2) +
                   std::pow((_y.z - _x.z), 2) +
                   std::pow((_y.w - _x.w), 2));
}

float
PlatformMath::random()
{
  return static_cast<float>(rand());
}

int32
PlatformMath::randomInt32()
{
  return static_cast<int32>(rand());
}

float
PlatformMath::randomRange(const float& _x, const float& _y)
{
  return static_cast<float>(_x + (rand() / _y));
}

Vector2
PlatformMath::random2()
{
  return Vector2(random(), random());
}

Vector2
PlatformMath::random2Range(const float& _x, const float& _y)
{
  return Vector2(randomRange(_x, _y), randomRange(_x, _y));
}

Vector3
PlatformMath::random3()
{
  return Vector3(random(), random(), random());
}

Vector3
PlatformMath::random3Range(const float& _x, const float& _y)
{
  return Vector3(randomRange(_x, _y), randomRange(_x, _y), randomRange(_x, _y));
}

Vector4
PlatformMath::random4()
{
  return Vector4(random(), random(), random(), random());
}

Vector4
PlatformMath::random4Range(const float& _x, const float& _y)
{
  return Vector4(randomRange(_x, _y),
                 randomRange(_x, _y),
                 randomRange(_x, _y),
                 randomRange(_x, _y));
}

/**
 * Sphere
**/
bool
PlatformMath::intersectSphereSphere(const Sphere& _sphere,
                                    const Sphere& _other,
                                    CollisionInfo& _cInfo)
{
  // get the origin of each sphere
  const Vector3 O1 = _sphere.m_origin;
  const Vector3 O2 = _other.m_origin;

  const float distance = Math::distance(_sphere.m_origin, _other.m_origin);
  const float sumRaius = _sphere.m_radius + _other.m_radius;

  // if the distance between both spheres is less than the sum of both their radius.
  if (distance < sumRaius) {
    _cInfo.m_normalHit = (O1 - O2).normalized();
    _cInfo.m_penDistance = sumRaius - distance;
    return true;
  }
  return false;
}

bool
PlatformMath::intersectSpherePoint(const Sphere& _sphere,
                                   const Vector3& _other,
                                   CollisionInfo& _cInfo)
{
  const float distance = Math::distance(_sphere.m_origin, _other);
  const Vector3 vec = _sphere.m_origin - _other;

  if (distance < _sphere.m_radius) {
    _cInfo.m_normalHit = (_other - _sphere.m_origin).normalized();
    _cInfo.m_penDistance = _sphere.m_radius - vec.magnitude();
    return true;
  }
  return false;
}

/**
 * Capsule
**/
bool
PlatformMath::intersectCapsuleCapsule(Capsule& _capsule, const Capsule& _other)
{
  _capsule = _other;
  return false;
}

bool
PlatformMath::intersectCapsulePoint(const Capsule& _capsule,
                                    const Vector3& _other,
                                    CollisionInfo& _cInfo)
{
  /**
   * check if its near the 2 half spheres
  **/
  const Vector3 a = _capsule.getA();
  const Vector3 b = _capsule.getB();
  if (Math::distance(a, _other) < _capsule.m_radius) {
    Sphere topSphere = Sphere();
    topSphere.m_origin = a;
    topSphere.m_radius = _capsule.m_radius;
    return intersectSpherePoint(topSphere, _other, _cInfo);
  }

  if (Math::distance(b, _other) < _capsule.m_radius) {
    Sphere bottomSphere = Sphere();
    bottomSphere.m_origin = b;
    bottomSphere.m_radius = _capsule.m_radius;
    return intersectSpherePoint(bottomSphere, _other, _cInfo);
  }
  /**
   * if not near the 2 half spheres, check if it may be inside of the cylinder
  **/
  else {
    const Vector3 cylinderAxis = a - b;
    const Vector3 pointToBase = _other - b;

    // projection of pointToBase onto the axis of the cylinder
    float projection = Math::dotProd(pointToBase, cylinderAxis) /
                       Math::dotProd(cylinderAxis, cylinderAxis);

    projection = PlatformMath::clamp(projection, 0.0f, 1.0f);

    // gets entire axis and multiplies by the brojection gotten
    const Vector3 closestAxisToPoint = b + cylinderAxis * projection;

    // gets distance between point in axis and the _other point
    const float distAtoOther = Math::distance(closestAxisToPoint, _other);
    if (distAtoOther <= _capsule.m_radius) {
      _cInfo.m_penDistance = Math::distance(closestAxisToPoint, _other) - _capsule.m_radius;
      _cInfo.m_normalHit = (closestAxisToPoint - _other).normalized();
      return true;
    }
    return false;
  }
}

bool
PlatformMath::intersectCapsuleSphere(const Capsule& _capsule,
                                     const Sphere& _sphere,
                                     CollisionInfo& _cInfo)
{
  // collision point
  const Vector3 top = _capsule.getA(); // top sphere origin of capsule.
  const Vector3 bottom = _capsule.getB(); // bottom sphere origin of capsule.
  const Vector3 cS = _sphere.m_origin; // sphere center.
  /** d is the intersection point between the origin of the sphere and the line going through
   *  the capusle.
   */
  const Vector3 d = top + ((cS - top) * (bottom - top)) * (bottom - top);

  // a new sphere will be generated in the intersection point.
  Sphere newSphere = Sphere();
  newSphere.m_origin = d;
  newSphere.m_radius = _capsule.m_radius;
  // use the enw sphere to do a sphere on sphere collission check.
  return (intersectSphereSphere(_sphere, newSphere, _cInfo));
}

/**
 * Cube
**/

bool
PlatformMath::intersectCubeCube(const Cube& _cube, const Cube& _other)
{
  const float sizeComb = _cube.m_size + _other.m_size;
  if (Math::abs(_cube.m_origin.x - _other.m_origin.x) < sizeComb) {
    if (Math::abs(_cube.m_origin.y - _other.m_origin.y) < sizeComb) {
      if (Math::abs(_cube.m_origin.z - _other.m_origin.z) < sizeComb) {
        return true;
      }
    }
  }
  return false;
}

bool
PlatformMath::intersectCubePoint(const Cube& _cube, const Vector3& _other)
{
  const float sizeCube = _cube.m_size;
  if (Math::abs(_cube.m_origin.x - _other.x) < sizeCube) {
    if (Math::abs(_cube.m_origin.y - _other.y) < sizeCube) {
      if (Math::abs(_cube.m_origin.z - _other.z) < sizeCube) {
        return true;
      }
    }
  }
  return false;
}

bool
PlatformMath::intersectCubeSphere(const Cube& _cube, const Sphere& _sphere)
{
  // get vector between the origin of both objects.
  const Vector3 vBetween = _sphere.m_origin - _cube.m_origin;

  // identity is placeholder for cube rotation.
  const Vector3 localVBetween = vBetween * Matrix4::IDENTITY;

  // 10 is arbitrary until i find how to calculate the vector between the center and a corner.
  const Vector3 qLocal = Math::clamp(localVBetween, -10, 10);
  
  // identity is placeholder for cube rotation.
  // q is the closest point.
  const Vector3 q = _cube.m_origin + (qLocal * Matrix4::IDENTITY);

  const float distance = Math::distance(q, _sphere.m_origin);

  // if the closest point is inside of the sphere.
  if (distance < _sphere.m_radius) {
    return true;
  }
  return false;
}

float
PlatformMath::sign(const float& _val)
{
  if (_val > 0.0f) { return 1.0f; }
  if (_val == 0.0f) { return 0.0f; }
  return -1.0f;
}

Vector3
PlatformMath::sign(const Vector3& _direction)
{
  return Vector3(sign(_direction.x), sign(_direction.y), sign(_direction.z));
}

Vector3
PlatformMath::supportPointOBB(const OBB& _box, const Vector3& _dir)
{
  const Vector3 dir = _dir.normalized();
  // Vector3 dirLocal = _box.m_transform.getTransposed() * dir;
  const Vector3 dirSigns = sign(dir);
  return (_box.m_transform * Vector4((_box.m_halfSize * dirSigns), 0.0f)).xyz(); // to do: ???
}

Vector3
PlatformMath::supportPointConvex(const Vector3& _direction, const Vector<Vector3>& _points)
{
  const Vector3 direction = _direction.normalized();
  // if the points list has nothing
  if (_points.empty()) { return Vector3(0); }
  // otherwise, if there's only one element
  if (_points.size() < 2) { return _points[0]; }
  // for each point
  uint32 iMax = 0;
  for (uint32 i = 1; i < _points.size(); ++i) {
    if (Math::dotProd(_points[i], direction) >
        Math::dotProd(_points[iMax], direction)) {
      iMax = i;
    }
  }
  return _points[iMax];
}

Vector3
PlatformMath::supportPointSphere(const Vector3& _direction, const Sphere& _sphere)
{
  return _direction.normalized() * _sphere.m_radius + _sphere.m_origin;
}

Vector3
PlatformMath::supportPointCapsule(const Vector3& _direction, const Capsule& _capsule)
{
  const Vector3 direction = _direction.normalized();
  // d = a + [(c\index{s}-a)·(b-a)](b-a)
  const Vector3 tOr = _capsule.getA();
  const Vector3 bOr = _capsule.getB();
  const Vector3 closestPointOnLine = tOr + ((bOr - tOr) * Math::dotProd(_direction - tOr,
                                                                        bOr - tOr));
  return direction * _capsule.m_radius + closestPointOnLine;
}
}
