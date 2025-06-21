/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "cmath"
#include "pkPlatformMath.h"
#include "pkVector4.h"
#include "pkMatrix4.h"

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

Vector3
PlatformMath::clamp(const Vector3 _t, const float _x, const float _y)
{
  Vector3 vec = _t;
  vec.x = clamp(vec.x, _x, _y);
  vec.y = clamp(vec.y, _x, _y);
  vec.z = clamp(vec.z, _x, _y);
  return vec;
}

bool
PlatformMath::isInRange(const float _t, float _x, float _y)
{
  if (_t >= _x && _t <= _y) {
    return true;
  }
  return false;
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

float
PlatformMath::hookeLaw(float _elasticity, float _displacement)
{
  // @source: https://www.britannica.com/science/Hookes-law
  // F = KX
  return _elasticity * _displacement;
}

bool
PlatformMath::isNan(float _x)
{
  return std::isnan(_x);
}

/**
 * Sphere
**/
bool
PlatformMath::intersectSphereSphere(Sphere& _sphere,
                                    const Sphere& _other,
                                    CollisionInfo& _cInfo)
{
  // get the origin of each sphere
  Vector3 O1 = _sphere.m_origin;
  Vector3 O2 = _other.m_origin;

  float distance = _sphere.m_origin.distanceTo(_other.m_origin);
  float sumRaius = _sphere.m_radius + _other.m_radius;

  // if the distance between both spheres is less than the sum of both their radius.
  if (distance < sumRaius) {
    _cInfo.m_normalHit = (O1 - O2).normalized();
    _cInfo.m_penDistance = sumRaius - distance;
    return true;
  }
  return false;
}

bool
PlatformMath::intersectSpherePoint(Sphere& _sphere,
                                   const Vector3& _other,
                                   CollisionInfo& _cInfo)
{
  float distance = _sphere.m_origin.distanceTo(_other);
  Vector3 vec = _sphere.m_origin - _other;

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
  _capsule = _capsule;
  _capsule = _other;
  return false;
}

bool
PlatformMath::intersectCapsulePoint(Capsule& _capsule, Vector3& _other, CollisionInfo& _cInfo)
{
  /**
   * check if its near the 2 half spheres
  **/
  if (_capsule.getA().distanceTo(_other) < _capsule.m_radius) {
    Sphere topSphere = Sphere();
    topSphere.m_origin = _capsule.getA();
    topSphere.m_radius = _capsule.m_radius;
    return intersectSpherePoint(topSphere, _other, _cInfo);
  }

  if (_capsule.getB().distanceTo(_other) < _capsule.m_radius) {
    Sphere bottomSphere = Sphere();
    bottomSphere.m_origin = _capsule.getB();
    bottomSphere.m_radius = _capsule.m_radius;
    return intersectSpherePoint(bottomSphere, _other, _cInfo);
  }
  /**
   * if not near the 2 half spheres, check if it may be inside of the cylinder
  **/
  else {
    Vector3 cylinderAxis = _capsule.getA() - _capsule.getB();
    Vector3 pointToBase = _other - _capsule.getB();

    // projection of pointToBase onto the axis of the cylinder
    float projection = pointToBase.dotProd(cylinderAxis) /
      cylinderAxis.dotProd(cylinderAxis);

    projection = PlatformMath::clamp(projection, 0.0f, 1.0f);

    // gets entire axis and multiplies by the brojection gotten
    Vector3 closestAxisToPoint = _capsule.getB() + cylinderAxis * projection;

    // gets distance between point in axis and the _other point
    float distAtoOther = closestAxisToPoint.distanceTo(_other);
    if (distAtoOther <= _capsule.m_radius) {
      _cInfo.m_penDistance = closestAxisToPoint.distanceTo(_other) - _capsule.m_radius;
      _cInfo.m_normalHit = (closestAxisToPoint - _other).normalized();
      return true;
    }
    return false;
  }
}

bool
PlatformMath::intersectCapsuleSphere(Capsule& _capsule, Sphere& _sphere, CollisionInfo& _cInfo)
{
  // collision point
  Vector3 top = _capsule.getA(); // top sphere origin of capsule.
  Vector3 bottom = _capsule.getB(); // bottom sphere origin of capsule.
  Vector3 cS = _sphere.m_origin; // sphere center.
  /** d is the intersection point between the origin of the sphere and the line going through
   *  the capusle.
   */
  Vector3 d = top + ((cS - top) * (bottom - top)) * (bottom - top);

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
PlatformMath::intersectCubeSphere(Cube& _cube, Sphere& _sphere)
{
  // get vector between the origin of both objects.
  Vector3 vBetween = _sphere.m_origin - _cube.m_origin;

  // identity is placeholder for cube rotation.
  Matrix4 TRot = Matrix4::IDENTITY;
  Vector3 localVBetween = vBetween * TRot;

  // 10 is arbitrary until i find how to calculate the vector between the center and a corner.
  Vector3 qLocal = Math::clamp(localVBetween, -10, 10);
  
  // identity is placeholder for cube rotation.
  // q is the closest point.
  Vector3 q = _cube.m_origin + (qLocal * Matrix4::IDENTITY);

  float distance = q.distanceTo(_sphere.m_origin);

  // if the closest point is inside of the sphere.
  if (distance < _sphere.m_radius) {
    return true;
  }
  return false;
}

float
PlatformMath::sign(float _val)
{
  if (_val > 0.0f) { return 1.0f; }
  if (_val == 0.0f) { return 0.0f; }
  return -1.0f;
}

Vector3
PlatformMath::sign3(Vector3& _direction)
{
  return Vector3(sign(_direction.x), sign(_direction.y), sign(_direction.z));
}

Vector3 PlatformMath::supportPointOBB(OBB& _box, Vector3& _dir)
{
  Vector3 dir = _dir.normalized();
  Vector3 dirLocal = _box.m_transform.getTransposed() * dir;
  Vector3 dirSigns = sign3(dir);
  return _box.m_transform * (_box.m_halfSize * dirSigns);
}

Vector3
PlatformMath::supportPointConvex(Vector3 _direction, Vector<Vector3>& _points)
{
  Vector3 direction = _direction.normalized();
  // if the points list has nothing
  if (_points.empty()) { return Vector3(0); }
  // otherwise, if there's only one element
  if (_points.size() < 2) { return _points[0]; }
  // for each point
  uint32 iMax = 0;
  for (uint32 i = 1; i < _points.size(); ++i) {
    if (_points[i].dotProd(direction) >
        _points[iMax].dotProd(direction)) {
      iMax = i;
    }
  }
  return _points[iMax];
}

Vector3
PlatformMath::supportPointSphere(Vector3& _direction, Sphere& _sphere)
{
  Vector3 direction = _direction.normalized();
  return direction * _sphere.m_radius + _sphere.m_origin;
}

Vector3
PlatformMath::supportPointCapsule(Vector3& _direction, Capsule& _capsule)
{
  Vector3 direction = _direction.normalized();
  // d = a + [(c\index{s}-a)·(b-a)](b-a)
  Vector3 tOr = _capsule.getA();
  Vector3 bOr = _capsule.getB();
  Vector3 closestPointOnLine = tOr + ((bOr - tOr) * (_direction - tOr).dotProd(bOr - tOr));
  return direction * _capsule.m_radius + closestPointOnLine;
}
}
