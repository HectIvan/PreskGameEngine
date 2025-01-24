#include "Player.h"

void
Player::move(float _deltaTime, Vector3 _direction)
{
  m_gameObject->move(_direction * _deltaTime);
}

void
Player::screenBounce(float _width, float _height)
{
  Vector3 direction = m_gameObject->transform.getTranslation3();
  if (direction.x < -_width * 0.5f ||
      direction.x > _width * 0.5f) {
    m_direction.x *= -1.0f;
  }
  if (direction.y < -_height * 0.5f ||
     direction.y > _height * 0.5f) {
    m_direction.y *= -1.0f;
  }
}
