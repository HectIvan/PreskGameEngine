#include "pkShape.h"
#include "pkPlatformMath.h"

namespace pkEngineSDK 
{
Vector3
Shape::getSupportPoint(Vector3& _direction)
{
  return Math::supportPointConvex(_direction, m_vertex);
}
}