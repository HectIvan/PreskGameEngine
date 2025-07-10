#include "pkAABB.h"

namespace pkEngineSDK
{

Vector3
AABB::supportPoint(Vector3& _direction)
{
  // in case direction is not normalized
  Vector3 dir = _direction.normalized();

  // get the AABB center
  Vector3 center = m_transform.getTranslation3();

  // get the support point using the center and size of the box
  Vector3 support;
  support.x = dir.x >= 0 ? center.x + m_halfSize.x : center.x - m_halfSize.x;
  support.y = dir.y >= 0 ? center.y + m_halfSize.y : center.y - m_halfSize.y;
  support.z = dir.z >= 0 ? center.z + m_halfSize.z : center.z - m_halfSize.z;

  return support;
}
}