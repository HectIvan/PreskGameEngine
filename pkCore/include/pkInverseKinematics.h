/*****************************************************************************/
/**
 * @file    pkInverseKinematics.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    20/03/2025
 * @brief
 *
 * @bug    No known bugs.
 */
/*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkActor.h"
#include "pkPrerequisitesCore.h"

namespace pkEngineSDK
{

class InverseKinematics
{
 public:
  InverseKinematics() = default;
  virtual ~InverseKinematics() = default;

  /**
   * @brief Insert a new node to the inverse kinematic chain.
   * @param _position Position in which the node will be inserted at in
   *                  local space.
   * @param _pActor Actor to insert (Optional).
   */
  void
  insertNodeLocal(Vector3 _position, SPtr<Actor> _pActor = nullptr);

  /**
   * @brief Insert a new node to the inverse kinematic chain.
   * @param _position Position in which the node will be inserted at in
   *                  global space.
   * @param _pActor Actor to insert (Optional).
   */
  void
  insertNodeGlobal(Vector3 _position, SPtr<Actor> _pActor = nullptr);

  /**
   * @brief delete a specific node.
   * @param _index Index of the node to delete
   */
  FORCEINLINE void
  deleteNode(uint32 _index) { m_nodes.erase(m_nodes.begin() + _index); };

  /**
   * @brief delete the last node in the chain.
   */
  FORCEINLINE void
  deleteLastNode() { m_nodes.pop_back(); }

  /**
   * @brief Set a node to a desired actor.
   * @param _pActor The desired actor.
   * @param _index Position of the node
   */
  FORCEINLINE void
  setNode(SPtr<Actor> _pActor, uint32 _index) { m_nodes[_index] = _pActor; }

 public:
  Vector<SPtr<Actor>> m_nodes;
};
}