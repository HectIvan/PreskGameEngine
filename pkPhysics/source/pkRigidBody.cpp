#include "pkRigidBody.h"

namespace pkEngineSDK
{

Matrix3
RigidBody::getInvInertiaWorld()
{
  Matrix3 rotationMat = m_transform.getRotation().getMatrix3();
  // to do: work on this to fix it
  return (rotationMat * m_invAngularInertia) * rotationMat.getTransposed();
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