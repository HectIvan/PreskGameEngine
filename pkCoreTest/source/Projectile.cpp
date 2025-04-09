#include "Projectile.h"
#include "pkLogger.h"
#include "pkPlatformMath.h"
#include "pkSphere.h"

using pkEngineSDK::Logger;
using pkEngineSDK::Sphere;
using pkEngineSDK::Math;

void
Projectile::start()
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
Projectile::moveVerlet(Vector3 _dir, float _force)
{
  m_actor->moveVerlet(_dir, _force);
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
Projectile::obstacleBounce(Vector3 _obstaclePos, float _obstacleRadius)
{
  // Final position.
  // Difference = |((R1 + R2) - (P1 - P2).magnitude)|
  // P1 += normal * Difference
  //
  // P1 Projectile position.
  // R1 Projectile radius.
  // R2 Obstacle radius
  // Pi Intersection point.
  // Get the projectile position
  Vector3 projectilePos = Vector3(m_actor->m_transform.getTranslation3());

  // Create spheres of obstacle and projectile
  Sphere projSphere(projectilePos, m_radius);
  Sphere obsSphere(_obstaclePos, _obstacleRadius);

  // check for a collission
  if (Math::intersectSphereSphere(projSphere, obsSphere)) {
    // get the reflected vector
    Vector3 normal = (projectilePos - _obstaclePos).normalized();
    Vector3 dir = Vector3(m_direction.x, m_direction.y, 0.0f);
    Vector3 reflect = Vector3::reflect(dir, normal);
    m_direction = Vector2(reflect.x, reflect.y);

    /**
    * Position reset
    */
    float difference = Math::abs((_obstacleRadius + m_radius) -
                                 (projectilePos - _obstaclePos).magnitude());
    projectilePos += normal * difference;
    m_actor->m_transform.setTranslation(projectilePos);
  }
}

void
Projectile::gravity(float _deltaTime)
{
  m_direction.y += m_gravity * _deltaTime;
}
