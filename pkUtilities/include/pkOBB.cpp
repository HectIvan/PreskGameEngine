#include "pkOBB.h"
#include "pkPlatformMath.h"

namespace pkEngineSDK
{

Vector3
OBB::supportPoint(Vector3& _direction)
{
  OBB copyOBB(m_transform, m_halfSize);
  return Math::supportPointOBB(copyOBB, _direction);
}
}
