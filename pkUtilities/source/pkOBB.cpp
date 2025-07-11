#include "pkOBB.h"

namespace pkEngineSDK
{

Vector3
OBB::supportPoint(Vector3& _direction)
{
  // Get the rotation from the matrix
  Vector3 right = Vector3(m_transform.matrix[0][0],
                          m_transform.matrix[0][1],
                          m_transform.matrix[0][2]);
  Vector3 up = Vector3(m_transform.matrix[1][0],
                       m_transform.matrix[1][1],
                       m_transform.matrix[1][2]);
  Vector3 forward = Vector3(m_transform.matrix[2][0],
                            m_transform.matrix[2][1],
                            m_transform.matrix[2][2]);

  // Normalize the direction in case its not already normalized
  Vector3 dir = _direction.normalized();

  // check if it needs to go in a positive or negative on each axis
  // OBB center
  Vector3 result = m_transform.getTranslation3();
  result += right * (dir.dotProd(right) >= 0 ? m_halfSize.x : -m_halfSize.x);
  result += up * (dir.dotProd(up) >= 0 ? m_halfSize.y : -m_halfSize.y);
  result += forward * (dir.dotProd(forward) >= 0 ? m_halfSize.z : -m_halfSize.z);

  return result;
}
}