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
    actor->setPosition(_position);
  }
  else { // if there is a desired actor.
    actor = _pActor;
  }

  // if there are nodes in the chain.
  if (!m_nodes.empty())
  {
    // get the previous node transform.
    Matrix4 rootTransform = m_nodes[m_nodes.size() - 1]->m_transform;
    actor->m_transform *= rootTransform;
  }
  m_nodes.push_back(actor);
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
}