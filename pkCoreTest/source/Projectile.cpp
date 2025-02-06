#include "Projectile.h"

void Projectile::start()
{
  // parameter assignation.
  m_speed = 40.0f;
  m_maxSpeed = 40.0f;
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
  if (pos.x < -_width * 0.5f ||
    pos.x > _width * 0.5f) {
    m_direction.x *= -1.0f;
    m_direction *= m_bounceFactor;

    if (pos.x < -_width * 0.5f) {
      m_actor->setPosition(-_width * 0.5f, pos.y, pos.z);
    }
    if (pos.x > _width * 0.5f) {
      m_actor->setPosition(_width * 0.5f, pos.y, pos.z);
    }
  }
  if (pos.y < -_height * 0.5f ||
    pos.y > _height * 0.5f) {
    m_direction.y *= -1.0f;
    m_direction *= m_bounceFactor;
    if (pos.y < -_height * 0.5f) {
      m_actor->setPosition(pos.x, -_height * 0.5f, pos.z);
    }
    if (pos.y > _height * 0.5f) {
      m_actor->setPosition(pos.x, _height * 0.5f, pos.z);
    }
  }
}

void
Projectile::gravity(float _deltaTime)
{
  m_direction.y += m_gravity * _deltaTime;
}
