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
  if (!m_nodes.empty()) {
    // get the previous node transform.
    Matrix4 rootTransform = m_nodes[m_nodes.size() - 1]->m_transform;
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
  // gte the max distance the arm can reach
  float maxDistance = 0;
  for (uint32 i = 0; i < m_bones.size(); ++i) {
    maxDistance += m_bones[i]->distance;
  }
  // distance between the target and the root
  float dist = _target.distanceTo(m_bones[0]->iniPos);
  // if the distance calculated is more than the max distance of the arm
  if (dist > maxDistance) {
    // set the target to the max direction
    Vector3 dir = (m_bones[0]->iniPos - getLastBone()->iniPos).normalized();
    dir *= maxDistance;
    _target = dir;
    getLastBone()->actorIni->m_transform.setTranslation(dir);
  }

  // loop backwards on the chain
  for (uint32 i = m_bones.size() - 1; i < 0; ++i) {
    m_bones[i]->finalPos = _target;

  }

  m_bones[4]->finalPos = _target;
  fabrik
}
}