/*****************************************************************************/
/**
 * @file    pkInverseKinematics.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    20/03/2025
 * @brief
 *
 * @bug    No known bugs.
 */
/*****************************************************************************/
#include "pkInverseKinematics.h"
#include "pkLogger.h"

// Algorithms to make:
// 
// FABRIK (done)
// CCD

namespace pkEngineSDK
{
void
InverseKinematics::insertNodeLocal(Vector3 _position, SPtr<Actor> _pActor)
{
  // create the actor and set its transform to an identity.
  SPtr<Actor> actor = make_shared<Actor>();

  // if there is no actor sent to the function.
  if (!_pActor) {
    actor->m_transform = Matrix4::IDENTITY;
  }
  else { // if there is a desired actor.
    actor = _pActor;
  }
  actor->setPosition(_position);

  // create the bone
  SPtr<IKBone> bone = make_shared<IKBone>();

  // if there are nodes in the chain.
  if (!m_bones.empty()) {
    // fill the bone with data 
    bone->actorIni = actor;
    SPtr<IKBone> lastBone = getLastBone();
    // fill the previous bone with data
    lastBone->distance = lastBone->actorIni->getPosition3().distanceTo(_position);
  }
  // if there are no nodes in the chain
  else {
    bone->actorIni = actor;
  }
  // m_nodes.push_back(actor);
  m_bones.push_back(bone);
}

void
InverseKinematics::insertNodeGlobal(Vector3 _position, SPtr<Actor> _pActor)
{
  // create the actor and set its transform to an identity.
  SPtr<Actor> actor = make_shared<Actor>();

  // if there is no actor sent to the function.
  if (!_pActor) {
    actor->m_transform = Matrix4::IDENTITY;
    actor->setPosition(_position);
  }
  else { // if there is a desired actor.
    actor = _pActor;
  }

  // insert to the chain
  m_nodes.push_back(actor);
}

void
InverseKinematics::deleteNode(uint32 _index)
{
  // check if index is in range.
  if (_index >= 0 && _index + 1 <= m_nodes.size()) {
    m_nodes.erase(m_nodes.begin() + _index);
  }
}

void
InverseKinematics::deleteBone(uint32 _index)
{
  // check if index is in range.
  if (_index >= 0 && _index + 1 <= m_bones.size()) {
    m_bones.erase(m_bones.begin() + _index);
  }
}

void
InverseKinematics::deleteLastNode()
{
  if (!m_nodes.empty()) {
    m_nodes.pop_back();
  }
}

void
InverseKinematics::deleteLastBone()
{
  if (!m_bones.empty()) {
    m_bones.pop_back();
  }
}

void
InverseKinematics::fabrik(Vector3 _target)
{
  Vector3 rootPos = m_bones[0]->actorIni->getPosition3();
  // gte the max distance the arm can reach
  float maxDistance = 0;
  for (uint32 i = 0; i < m_bones.size(); ++i) {
    maxDistance += m_bones[i]->distance;
  }
  // distance between the target and the root
  float dist = _target.distanceTo(m_bones[0]->actorIni->getPosition3());
  // bone count
  uint32 bCount = static_cast<uint32>(m_bones.size());
  // root position
  Vector3 iniPos = m_bones[0]->actorIni->getPosition3();
  // if the distance calculated is more than the max distance of the arm
  if (dist > maxDistance) {
    // set the target to the max direction
    Vector3 direction = (_target - iniPos).normalized();
    // set all joints to one direction
    for (uint32 i = 1; i < m_bones.size(); ++i) {
      Vector3 prevPos = m_bones[i - 1]->actorIni->getPosition3();
      Vector3 newPos = prevPos + direction * m_bones[i - 1]->distance;
      m_bones[i]->actorIni->setPosition(newPos);
    }
  }
  // if the target is inside the range of the arm
  else {
    // forward iteration
    Vector3 target = _target;
    for (uint32 i = bCount - 1; i > 0; --i)
    {
      // direction from this bone new position to the previous bone
      m_bones[i]->actorIni->setPosition(target);
      Vector3 directionNorm = (m_bones[i - 1]->actorIni->getPosition3() - target).normalized();
      // distance from this bone to the previous bone
      dist = m_bones[i - 1]->distance;
      // set the new target for the next bone move
      target =  m_bones[i]->actorIni->getPosition3() + directionNorm * dist;
    }
    // backward iteration
    m_bones[0]->actorIni->setPosition(rootPos);
    for (uint32 i = 1; i < bCount; ++i)
    {
      Vector3 nextPos = m_bones[i]->actorIni->getPosition3();
      Vector3 prevPos = m_bones[i - 1]->actorIni->getPosition3();
      Vector3 directionNorm = (nextPos - prevPos).normalized();
      // distance from this bone to the previous bone
      dist = m_bones[i - 1]->distance;
      // set the new target for the next bone move
      target = prevPos + directionNorm * dist;
      // direction from this bone new position to the previous bone
      m_bones[i]->actorIni->setPosition(target);
    }
  }
}
}