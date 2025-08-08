/*************************************************************************/
/**
 * @file    pkPhysicsManager.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    05/06/2025
 * @brief   Physics manager for the physics library.
 *
 * @bug    no knon bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPhysicsManager.h"
#include "pkPlatformMath.h"
#include "pkCollisionInfo.h"

namespace pkEngineSDK
{

Vector<Shape>
PhysicsManager::sortByLeft(Vector<Shape>& _shapes)
{
  Vector<Shape> returnShapes = _shapes;
  // sort in ascending order using the x axis
  // std::sort(returnShapes.begin(),
  //           returnShapes.end(),
  //           [](const Vector3& a, const Vector3& b) {
  //             return a.x < b.x;
  //           });
  return returnShapes;
}

void
PhysicsManager::BPCDSweepAndPrune(Vector<Shape>& _colliders)
{
  // temporary shape vector
  Vector<Shape> shapes = sortByLeft(_colliders);
  // check for objects
  for (int32 i = 0; i < _colliders.size(); ++i) {
    const Shape coll1 = _colliders[i];
    // for each collider in front of the brevious one
    for (int32 j = i + 1; j < _colliders.size(); ++j) {
      const Shape coll2 = _colliders[j];
      // get the vertex of both objects
      Vector<Vector3> points1 = coll1.m_vertex;
      Vector<Vector3> points2 = coll2.m_vertex;
      // get the most right of the first collision and the most left of the second collision.
      Vector3 coll1right = Math::supportPointConvex(Vector3::RIGHT, points1);
      Vector3 coll2Left = Math::supportPointConvex(Vector3::LEFT, points2);
      // if the left and right of both objects do not intersect, there is no collision.
      if (coll2Left.x > coll1right.x) { break; }
      // else, a collision is possible.
    }
  }
}

// to do: make a simplex structure, currently using a simple shape
bool
PhysicsManager::GJK(Shape& _shape1, Shape& _shape2, uint32 _attempts)
{
  // create the simplex
  Shape simplex;
  // get a random direction
  Vector3 direction = Vector3::RIGHT;
  // get the first support point between the 2 shapes and the direction
  Vector3 supportPoint = supportCSO(_shape1, _shape2, direction);

  // insert the first simplex point
  simplex.m_vertex.push_back(supportPoint);

  // get the next direction
  direction = supportPoint.normalized() * -1.0f;

  // check loop
  uint32 count = 0;
  while (count < _attempts) {
    // get the support point of both shapes using the new direction
    Vector3 newPoint = supportCSO(_shape1, _shape2, direction);
    // if the origin is not reachable
    if (newPoint.dotProd(direction) <= 0.0f) {
      return false;
    }
    // insert the next simplex point
    simplex.m_vertex.push_back(newPoint);
    // if origin is inside of simplex
    if (updateSimplexNDirection(simplex, direction)) {
      return true;
    }
    ++count;
  }
}
Vector3
PhysicsManager::supportCSO(Shape& _shape1, Shape& _shape2, Vector3& _direction)
{
  // get support point of one shape and get the support point of the second shape using the
  // inverse direction
  Vector3 invDir = _direction * -1.0f;
  return (Math::supportPointConvex(_direction, _shape1.m_vertex) -
          Math::supportPointConvex(invDir, _shape2.m_vertex));
}

bool
PhysicsManager::updateSimplexNDirection(Shape& _simplex, Vector3& _direction)
{
  switch (_simplex.m_vertex.size()) {
  // get the next direction using the line
  case 2: {
    Vector3 AB = _simplex.m_vertex[1] - _simplex.m_vertex[0];
    Vector3 AO = _simplex.m_vertex[0] * -1.0f;

    Vector3 ABPerp = AO.cross(AO.cross(AB));
    _direction = ABPerp.normalized();
    return false;
  }
  // get the next direction to check for by using the current triangle
  case 3: {
    Vector3 AB = _simplex.m_vertex[1] - _simplex.m_vertex[0];
    Vector3 AC = _simplex.m_vertex[2] - _simplex.m_vertex[0];
    Vector3 AO = _simplex.m_vertex[0] * -1.0f;
    // get triangle normal
    return false;
  }
  // check for the origin within all 4 sides
  case 4: {
    Vector3 array[4] = { _simplex.m_vertex[3], _simplex.m_vertex[2],
                         _simplex.m_vertex[1], _simplex.m_vertex[0]};
    Vector3 arr1[3] = { array[2], array[1], array[0] };
    Vector3 arr2[3] = { array[2], array[3], array[0] };
    Vector3 arr3[3] = { array[1], array[3], array[0] };
    if (originInFrontOfPlane(arr1, _direction)) { return false; }
    else if (originInFrontOfPlane(arr2, _direction)) { return false; }
    else if (originInFrontOfPlane(arr3, _direction)) { return false; }
    else { return true; }
  }
  }
}
bool
PhysicsManager::originInFrontOfPlane(Vector3 _vertex[], Vector3& _direction)
{
  // check if the direction is in front or behind the plane
  Vector3 p1p2 = _vertex[1] - _vertex[0];
  Vector3 p1p3 = _vertex[2] - _vertex[0];

  Vector3 dir = p1p3 * p1p2;

  if (dir.dotProd(_vertex[0] * -1.0f) > 0.0f) {
    _direction = dir;
  }
  return dir.dotProd(_vertex[0] * -1.0f) > 0 ? true : false;
}

Matrix3
PhysicsManager::getInertiaTensorOBB(OBB _obb, float _mass)
{
  Matrix3 result = Matrix3::IDENTITY;

  float hx2 = _obb.m_halfSize.x * _obb.m_halfSize.x;
  float hy2 = _obb.m_halfSize.y * _obb.m_halfSize.y;
  float hz2 = _obb.m_halfSize.z * _obb.m_halfSize.z;

  result.matrix[0][0] = (1.0f / 3.0f) * _mass * (hy2 + hz2);
  result.matrix[1][1] = (1.0f / 3.0f) * _mass * (hz2 + hx2);
  result.matrix[2][2] = (1.0f / 3.0f) * _mass * (hx2 + hy2);

  return _obb.m_transform.getRotation().getMatrix3() *
         result *
         _obb.m_transform.getRotation().getTransposed().getMatrix3();
}

Matrix3
PhysicsManager::getInertiaTensorSphere(Sphere& _sphere, float _mass)
{
  Matrix3 inertia = Matrix3::IDENTITY;

  inertia.matrix[0][0] = (_mass * (_sphere.m_radius * _sphere.m_radius)) * 0.4f;
  inertia.matrix[1][1] = inertia.matrix[0][0];
  inertia.matrix[2][2] = inertia.matrix[0][0];

  return inertia;
}

float
PhysicsManager::getEffectiveMass(Vector3& _normalHit,
                                 RigidBody& _rb1,
                                 RigidBody& _rb2,
                                 Vector3 _contactPoint1,
                                 Vector3 _contactPoint2)
{
  Vector3 angularEffect = _rb1.getInvInertiaWorld() *
                          (_contactPoint1.cross(_normalHit)).cross(_contactPoint1)
                          + _rb2.getInvInertiaWorld() *
                          (_contactPoint2.cross(_normalHit)).cross(_contactPoint2);

  return _rb1.m_inverseMass + _rb2.m_inverseMass + angularEffect.dotProd(_normalHit);
}

float
PhysicsManager::getElasticity(const RigidBody& _rb1, const RigidBody& _rb2)
{
  return (_rb1.getElasticity() + _rb2.getElasticity()) * 0.5f;
}

float
PhysicsManager::getFriction(const RigidBody& _rb1, const RigidBody& _rb2)
{
  return (_rb1.getFrictionCoefficient() + _rb2.getFrictionCoefficient()) * 0.5f;
}

void
PhysicsManager::resolveCollision(RigidBody _rb1, RigidBody _rb2, CollisionInfo _info)
{
  /* ----- Restitution ----- */
  Vector3 r1 = _info.m_contactPoint1 - _rb1.getWorldPosition();
  Vector3 r2 = _info.m_contactPoint2 - _rb2.getWorldPosition();

  Vector3 v_rel = _rb1.m_linearVelocity + _rb1.m_angularVelocity.cross(r1)
                  - (_rb2.m_linearVelocity + _rb2.m_angularVelocity.cross(r2));

  float v_alongNormal = v_rel.dotProd(_info.m_normalHit);
  if (v_alongNormal > 0) { return; }  // do not resolve if they separate

  float impulseNormal = -(1 + getElasticity(_rb1, _rb2)) * v_alongNormal;
  impulseNormal /= getEffectiveMass(_info.m_normalHit, _rb1, _rb2, r1, r2);

  Vector3 j_normal = _info.m_normalHit * impulseNormal;

  _rb1.applyImpulse(j_normal, _info.m_contactPoint1);
  Vector3 j_inNormal = j_normal * -1.0f;
  _rb2.applyImpulse(j_inNormal, _info.m_contactPoint2);

  /* ----- Recalculare velocity ----- */
  v_rel = _rb1.m_linearVelocity + _rb1.m_angularVelocity.cross(r1)
          - (_rb2.m_linearVelocity + _rb2.m_angularVelocity.cross(r2));

  v_alongNormal = v_rel.dotProd(_info.m_normalHit);

  /* ----- Friction (Coulomb model) ----- */
  Vector3 v_tan = v_rel - (_info.m_normalHit * v_alongNormal);
  float v_tanMagnitude = v_tan.magnitudeSquare();
  // if the magnitude of the tangent is incredibly small, there is no friction
  if (v_tanMagnitude <= Math::SMALL_NUMBER) { return; }

  // normalize
  Vector3 d_tan = v_tan / Math::sqrt(v_tanMagnitude);

  // calculate the ideal impulse
  float impulseIdeal = v_rel.dotProd(d_tan);
  impulseIdeal /= getEffectiveMass(d_tan, _rb1, _rb2, r1, r2);

  float maxFriction = getFriction(_rb1, _rb2) * impulseNormal;
  float impulseTan = Math::clamp(impulseIdeal, -maxFriction, maxFriction);

  Vector3 j_tan = d_tan * impulseTan;

  // apply said impulse to the rigid bodies
  _rb1.applyImpulse(j_tan, _info.m_contactPoint1);
  Vector3 j_invTan = j_tan * -1.0f;
  _rb2.applyImpulse(j_invTan, _info.m_contactPoint2);
}

float
PhysicsManager::getEffectiveMassP(RigidBody& _rb1, RigidBody& _rb2)
{
  Matrix3 skew1 = Matrix3::getSkewSymetric();
  Matrix3 skew2 = 
  float firstMass = _rb1.m_inverseMass * Matrix3::IDENTITY;
  float secondMass = _rb2.m_inverseMass * Matrix3::IDENTITY;
}
}
