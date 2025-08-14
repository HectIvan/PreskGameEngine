/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkCapsule.h"
#include "pkMath.h"

namespace pkEngineSDK {
Vector3
Capsule::supportPoint(Vector3& _direction)
{
  Vector3 direction = _direction.normalized();
  // d = a + [(c\index{s}-a)·(b-a)](b-a)
  Vector3 tOr = getA();
  Vector3 bOr = getB();
  Vector3 closestPointOnLine = tOr + ((bOr - tOr) * (_direction - tOr).dotProd(bOr - tOr));
  return direction * m_radius + closestPointOnLine;
}
}