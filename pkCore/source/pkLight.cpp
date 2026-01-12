#include "pkLight.h"
#include "pkActor.h"

namespace pkEngineSDK
{
void
Light::update(Actor& _owner)
{
  m_position = _owner.getPosition3();
  m_transform = _owner.m_transform;
}
}
