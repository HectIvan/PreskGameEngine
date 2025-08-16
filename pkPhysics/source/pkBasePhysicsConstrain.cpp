#include "pkBasePhysicsConstrain.h"
#include "pkRigidBody.h"
#include "pkVector3.h"
#include "pkQuaternion.h"

namespace pkEngineSDK
{

void
BasePhysicsConstrain::init()
{

}

void
BasePhysicsConstrain::preSolve(float _dt)
{
  Vector3 pos1 = m_rb1->getWorldPosition();
  Vector3 pos2 = m_rb2->getWorldPosition();

  Matrix3 skew1 = Matrix3::getSkewSymetric(pos1);
  Matrix3 skew2 = Matrix3::getSkewSymetric(pos2);

  Matrix3 firstMass = Matrix3::IDENTITY * m_rb1->getInverseMass();
  Matrix3 secondMass = Matrix3::IDENTITY * m_rb2->getInverseMass();
  Matrix3 effectiveMass = firstMass + secondMass +
                          skew1 * m_rb1->getInvInertiaWorld() * skew1.getTransposed() + 
                          skew2 * m_rb2->getInvInertiaWorld() * skew2.getTransposed();

  Quaternion rot1 = Quaternion();
  Quaternion rot2 = Quaternion();

  // Quaternion rot1 = m_rb1->m_transform.getLocalRotation();
  // Quaternion rot2 = m_rb2->m_transform.getLocalRotation();

  // Vector3 lambda1 = effectiveMass * -(pos1 + rot1 * m_r1 - pos2 - rot2 * m_r2);

  // m_rb1->applyPositionalImpulse(lambda, pos1 + rot1 * m_r1);
  // m_rb2->applyPositionalImpulse(-lambda, pos2 + rot2 * m_r2);
}

void
BasePhysicsConstrain::solve(float _dt)
{

}

void
BasePhysicsConstrain::postSolve(float _dt)
{

}
}
