#include "pkRigidBody.h"
#include "pkPlatformMath.h"

namespace pkEngineSDK
{

Matrix3
RigidBody::getInvInertiaWorld()
{
  Matrix3 rotationMat = m_transform.getRotation().getMatrix3();
  // to do: work on this to fix it
  return (rotationMat * m_invAngularInertia) * rotationMat.getTransposed();
}

float
RigidBody::getFriction(Vector3 _dir, float _magnitude)
{
  return ((_dir * _magnitude) * m_frictionCoeff).magnitude();
}

float
RigidBody::getFriction(Vector3 _vector)
{
  return getFriction(_vector.normalized(), _vector.magnitude());
}

float
RigidBody::getFriction(RigidBody& _rb, Vector3 _dir, float _magnitude)
{
  float mergedFrictCoeff = (m_frictionCoeff + _rb.m_frictionCoeff) * 0.5f;
  return ((_dir) *mergedFrictCoeff).magnitude();
}

float
RigidBody::getElasticity(RigidBody& _rb)
{
  return (m_elasticity + _rb.getElasticity()) * 0.5f;
}

void
RigidBody::applyImpulse(const Vector3& _impulse, const Vector3& _point)
{
  if (m_inverseMass == 0.0f) { return; }

  Vector3 r = _point - getWorldPosition();

  m_linearVelocity += _impulse * m_inverseMass;
  m_angularVelocity += getInvInertiaWorld() * r.cross(_impulse);
}

void
RigidBody::applyPositionalImpulse(const Vector3& impulse, const Vector3& point) {
  if (m_inverseMass == 0.0f) { return; }
  Vector3 r = point - getWorldPosition();
  m_position += impulse * m_inverseMass;
  m_orientation += Quaternion::fromBivector(getInvInertiaWorld() * r.cross(impulse));
}
}