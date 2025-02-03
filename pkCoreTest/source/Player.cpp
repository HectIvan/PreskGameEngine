#include "Player.h"

void Player::start()
{
  // parameter assignation.
  m_speed = 30.0f;
  m_maxSpeed = 30.0f;
  m_bounceFactor = 0.9f;
  m_direction = Vector2(0);
  m_gravity = 1.0f;
  m_fired = false;
  m_actor->setActive(false);
}

void
Player::move(float _deltaTime, Vector3 _direction)
{
  m_actor->move(_direction * _deltaTime);
}

void
Player::fire(Vector3 _startPos, Vector2 _direction)
{
  m_fired = true;
  m_actor->setPosition(_startPos);
  m_direction = _direction;
  m_actor->setActive(true);
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
