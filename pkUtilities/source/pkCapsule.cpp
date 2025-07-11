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
  Capsule copyCap(m_center, m_direction, m_height, m_radius);
  return Math::supportPointCapsule(_direction, copyCap);
}
}