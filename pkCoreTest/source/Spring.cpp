#include "Spring.h"
#include "pkPlatformMath.h"

using pkEngineSDK::Math;

void
Spring::move(float _deltaTime)
{
  // get the positions
  Vector3 weightPos = m_weight->m_transform.getTranslation3();
  Vector3 anchorPos = m_anchor->m_transform.getTranslation3();
  
  // get the directional vector between the 2 points
  Vector3 direction = (weightPos - anchorPos);
  
  // get the length of the direction
  float length = direction.magnitude();
  
  // force to be applied
  float force = Math::hookeLaw(m_elasticity, length - m_length) * _deltaTime;
  
  // as the force decreases, so does the influence of the stretch
  Vector2 newPos = Vector2(weightPos.x, weightPos.y) - force;
  // set the new position
  m_weight->m_transform.setTranslation(Vector3(newPos.x, newPos.y, 0.0f));
}