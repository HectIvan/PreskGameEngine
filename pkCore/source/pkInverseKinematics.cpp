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
// FABRIK
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
    // get the previous node transform.
    Matrix4 rootTransform = m_bones[m_bones.size() - 1]->actorIni->m_transform;
    actor->m_transform *= rootTransform;
    // fill the bone with data
    bone->iniPos = actor->m_transform.getTranslation3();
    bone->actorIni = actor;
    SPtr<IKBone> lastBone = getLastBone();
    bone->distance = lastBone->iniPos.distanceTo(bone->iniPos);
    // previous bone final position is the start of this new bone
    lastBone->finalPos = bone->iniPos;
    // fill the previous bone with data
    lastBone->distance = lastBone->iniPos.distanceTo(lastBone->finalPos);
  }
  // if there are no nodes in the chain
  else {
    bone->iniPos = _position;
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
  g_Logger().print(_target);
  // gte the max distance the arm can reach
  float maxDistance = 0;
  for (uint32 i = 0; i < m_bones.size(); ++i) {
    maxDistance += m_bones[i]->distance;
  }
  // distance between the target and the root
  float dist = _target.distanceTo(m_bones[0]->actorIni->getPosition3());
  // bone count
  uint32 bCount = m_bones.size();
  // root position
  Vector3 iniPos = m_bones[0]->actorIni->getPosition3();
  // if the distance calculated is more than the max distance of the arm
  if (dist > maxDistance) {
    // set the target to the max direction
    Vector3 direction = (iniPos - _target).normalized();
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
      m_bones[i]->actorIni->setPosition(target);
      target = (m_bones[i]->actorIni->getPosition3() +
                target - m_bones[i]->actorIni->getPosition3()).normalized() *
                m_bones[i]->distance;
    }

    // backward iteration

    for (uint32 i = 1; i < bCount - 1; ++i) {
      target = (m_bones[i]->actorIni->getPosition3() +
                target - m_bones[i]->actorIni->getPosition3()).normalized() *
                m_bones[i]->distance;
      m_bones[i]->actorIni->setPosition(target);
    }
  }
  // forward chain loop
  // for (uint32 i = m_bones.size() - 1; i > 0; --i) {
  //   if (i - 1 > 0) {
  //     Vector3 prevPos = m_bones[i - 1]->actorIni->getPosition3();
  //     Vector3 newPos = ((_target - prevPos).normalized()) * m_bones[i - 1]->distance;
  //     m_bones[i]->actorIni->setPosition(newPos);
  //   }
  // }
}
}