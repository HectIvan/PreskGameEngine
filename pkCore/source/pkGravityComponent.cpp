/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkGravityComponent.h"
#include "pkActor.h"

namespace pkEngineSDK
{
void
GravityComponent::update(Actor& _owner)
{
}

void
GravityComponent::applyGravity(SPtr<Actor> _pActor, float _deltaTime)
{
  applyGravity(_pActor->m_transform, _deltaTime);
}
void
GravityComponent::applyGravity(Matrix4& _transform, float _deltaTime)
{
  if (m_active) {
    Vector3 pos = _transform.getTranslation3();
    pos += m_gravityDirection *= m_force * _deltaTime;
    _transform.setTranslation(pos);
  }
}
}