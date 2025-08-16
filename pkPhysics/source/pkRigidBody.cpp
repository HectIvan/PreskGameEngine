#include "pkRigidBody.h"

namespace pkEngineSDK
{

Matrix3
RigidBody::getInvInertiaWorld()
{
  Matrix3 rotationMat = m_transform.getRotation().getMatrix3();
  return (rotationMat * m_invAngularInertia) * rotationMat.getTransposed();
}

void
RigidBody::applyImpulse(const Vector3& _impulse, const Vector3& _point)
{
  float invMass = getInverseMass();
  if (invMass == 0.0f) { return; }

  Vector3 r = _point - getWorldPosition();

  m_linearVelocity += _impulse * invMass;
  m_angularVelocity += getInvInertiaWorld() * r.cross(_impulse);
}

float
RigidBody::getElasticity() const
{
  if (m_physMat) {
    return m_physMat->getElasticity();
  }
  return 0.0f;
}

void
RigidBody::setElasticity(const float _elasticity)
{
  if (m_physMat) {
    m_physMat->setElasticity(_elasticity);
  }
}

float
RigidBody::getFrictionCoefficient() const
{
  if (m_physMat) {
    return m_physMat->getFriction();
  }
  return 0.0f;
}

void
RigidBody::setFrictionCoef(const float _friction)
{
  if (m_physMat) {
    m_physMat->setFriction(_friction);
  }
}

void
RigidBody::applyPositionalImpulse(const Vector3& _impulse, const Vector3& _point)
{
  float invMass = getInverseMass();
  if (invMass == 0.0f) { return; }

  Vector3 r = _point - getWorldPosition();

  m_position += _impulse * invMass;
  Vector3 pseudoVector = getInvInertiaWorld() * r.cross(_impulse);
  Quaternion deltaRot = Quaternion::axisAngle(pseudoVector, pseudoVector.magnitude());
  m_orientation = (deltaRot * m_orientation).normalized();
}

}