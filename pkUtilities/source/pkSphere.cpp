/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkMath.h"
#include "pkSphere.h"

namespace pkEngineSDK {


Vector3
Sphere::supportPoint(Vector3& _direction)
{
  Vector3 direction = _direction.normalized();
  return direction * m_radius + m_origin;
}
}