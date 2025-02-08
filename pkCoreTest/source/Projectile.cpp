#include "Projectile.h"

void Projectile::start()
{
  // parameter assignation.
  m_speed = 40.0f;
  m_maxSpeed = 40.0f;
  m_radius = 1.0f;
  m_bounceFactor = 0.9f;
  m_direction = Vector2(0);
  m_gravity = 1.0f;
  m_fired = false;
  m_actor->setActive(false);
}

void
Projectile::move(float _deltaTime, Vector3 _direction)
{
  m_actor->move(_direction * _deltaTime);
}

void
Projectile::fire(Vector3 _startPos, Vector2 _direction)
{
  m_fired = true;
  m_actor->setPosition(_startPos);
  m_direction = _direction;
  m_actor->setActive(true);
}

void
Projectile::projTimer(float _deltaTime)
{
  m_lifeTime += 1.0f * _deltaTime;
  if (m_lifeTime > m_lifeTimer) {
    m_lifeTime = 0.0f;
    m_actor->setActive(false);
  }
}

void
Projectile::screenBounce(float _width, float _height)
{
  Vector3 pos = m_actor->m_transform.getTranslation3();
  float left = -_width * 0.5f + m_radius;
  float right = _width * 0.5f - m_radius;
  float top = -_height * 0.5f + m_radius;
  float bottom = _height * 0.5f - m_radius;
  if (pos.x < left  ||
    pos.x > right) {
    m_direction.x *= -1.0f;
    m_direction *= m_bounceFactor;

    if (pos.x < left) {
      m_actor->setPosition(left, pos.y, pos.z);
    }
    if (pos.x > right) {
      m_actor->setPosition(right, pos.y, pos.z);
    }
  }
  if (pos.y < top ||
      pos.y > bottom) {
    m_direction.y *= -1.0f;
    m_direction *= m_bounceFactor;
    if (pos.y < top) {
      m_actor->setPosition(pos.x, top, pos.z);
    }
    if (pos.y > bottom) {
      m_actor->setPosition(pos.x,bottom, pos.z);
    }
  }
}

void
Projectile::gravity(float _deltaTime)
{
  m_direction.y += m_gravity * _deltaTime;
}
