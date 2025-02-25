#include "Spring.h"
#include "pkPlatformMath.h"

using pkEngineSDK::Math;

void
Spring::applyForce(float _force, Vector3 _direction)
{
  // m_length += hookeLaw(_force);
}

void
Spring::move(float _deltaTime, Vector3 _direction)
{
  Vector3 weightPos = m_weight->m_transform.getTranslation3();
  Vector3 anchorPos = m_anchor->m_transform.getTranslation3();

  Vector3 direction = (anchorPos - weightPos).normalized();

 //  weightPos += hookeLaw();

  m_weight->m_transform.setTranslation(weightPos + direction * _deltaTime);
}

void
Spring::gravity(float _gravity, float _deltaTime)
{
}

float
Spring::hookeLaw(float _force)
{
  // X = F/K
  // X is the distance traveled
  // F is the force applied
  // K is the elasticity constant
  return _force / m_elasticity;
}