#include "Player.h"

void
Player::move(float _deltaTime, Vector3 _direction)
{
  m_actor->move(_direction * _deltaTime);
}

void
Player::screenBounce(float _width, float _height)
{
  Vector3 direction = m_actor->m_transform.getTranslation3();
  if (direction.x < -_width * 0.5f ||
      direction.x > _width * 0.5f) {
    m_direction.x *= -1.0f;
    m_direction *= m_bounceFactor;
  }
  if (direction.y < -_height * 0.5f ||
     direction.y > _height * 0.5f) {
    m_direction.y *= -1.0f;
    m_direction *= m_bounceFactor;
  }
}

void
Player::gravity(float _deltaTime)
{
  m_direction.y += m_gravity * _deltaTime;
}
