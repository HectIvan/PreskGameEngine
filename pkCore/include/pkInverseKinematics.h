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
#include "pkVector3.h"
#include "pkPrerequisitesCore.h"

namespace pkEngineSDK
{

struct IKBone
{
  Vector3 iniPos;
  SPtr<Actor> actorIni = nullptr;
  Vector3 finalPos;
  float distance;
};

class PK_CORE_EXPORT InverseKinematics
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
  void
  deleteNode(uint32 _index);

  /**
   * @brief delete a specific bone.
   * @param _index Index of the bone to delete
   */
  void
  deleteBone(uint32 _index);

  /**
   * @brief delete the last node in the chain.
   */
  void
  deleteLastNode();

  /**
   * @brief delete the last bone in the chain.
   */
  void
  deleteLastBone();

  /**
   * @brief Set a node to a desired actor.
   * @param _pActor The desired actor.
   * @param _index Position of the node
   */
  FORCEINLINE void
  setNode(SPtr<Actor> _pActor, uint32 _index) { m_nodes[_index] = _pActor; }

  /**
   * @brief Set a bone actor to a desired actor.
   * @param _pActor The desired actor.
   * @param _index Position of the bone
   */
  FORCEINLINE void
  setBone(SPtr<Actor> _pActor, uint32 _index) { m_bones[_index]->actorIni = _pActor; }

  /**
   * @brief Get the last node of the list.
   * @return The last node.
   */
  FORCEINLINE SPtr<Actor>
  getLastNode() { return m_nodes[m_nodes.size() - 1]; }

  /**
   * @brief Get the last bone of the list.
   * @return The last bone.
   */
  FORCEINLINE SPtr<IKBone>
  getLastBone() { return m_bones[m_bones.size() - 1]; }

  /**
   * @brief
   */
  void
  fabrik(Vector3 _target);

 public:
  Vector<SPtr<Actor>> m_nodes;

  Vector<SPtr<IKBone>> m_bones;
};
}