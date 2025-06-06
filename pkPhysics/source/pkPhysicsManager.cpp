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
// to do: make a simplex structure, currently using a simple shape
bool
PhysicsManager::GJK(Shape& _shape1, Shape& _shape2, uint32 _attempts)
{
  // create the simplex
  Shape simplex;
  // get a random direction
  Vector3 direction = Vector3::random();
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
}
