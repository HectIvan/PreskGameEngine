#include "Player.h"

void
Player::move(float _deltaTime, Vector3 _direction)
{
  m_gameObject->move(_direction * _deltaTime);
}
