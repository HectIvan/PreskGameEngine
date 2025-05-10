/*****************************************************************************/
/**
 * @file    pkScene.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    2025/01/29
 * @brief   Scene used for the engine.
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

class PK_CORE_EXPORT Scene
{
 public:
  Scene() = default;
  virtual ~Scene() = default;

  /**
   * @brief Insert a new actor into the scene or as
   * a child of another actor.
   * @param _tranform Transform of the actor.
   * @param _pParent If the actor will be the child of another actor.
   * @return The actor created.
   */
  SPtr<Actor>
  instantiate(Matrix4 _transform = Matrix4::IDENTITY,
              SPtr<Actor> _pParent = nullptr);

  /**
   * @brief Set the active state of the current scene.
   * @param _active State of the scene.
   */
  void
  setActive(bool _active) { m_isActive = _active; }

  /**
   * @brief Get the last inserted actor of the scene.
   * @return The last actor.
   */
  FORCEINLINE SPtr<Actor>
  getLastActor() { return m_actors[m_actors.size() - 1]; }

  /**
   * @brief Get all the actors in the scene.
   * @return A vector with all the actors.
   */
  Vector<SPtr<Actor>>
  getAllActors() { return m_actors; }

  /**
   * @brief Get the actor in the index of the scene.
   * @param _index Actor position in the list.
   * @return The actor in the index.
   */
  SPtr<Actor>
  getActor(uint32 _index) { return m_actors[_index]; }

  /**
   * @brief Update all actors.
   * @param _deltaTime Time between frames.
   */
  void
  update(float _deltaTime);

 private:
  /**
   * @brief Update an actor.
   * @param _pActor Actor to update.
   * @param _deltaTime Time between frames.
   */
  void
  updateActor(SPtr<Actor> _pActor, float _deltaTime);

 public:
  Vector<SPtr<Actor>> m_actors;
  bool m_isActive;
};
}