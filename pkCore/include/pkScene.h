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
   * @param _name Name of the scene.
   * @param _pParent If the actor will be the child of another actor.
   * @param _tranform Transform of the actor.
   * @return The actor created.
   */
  SPtr<Actor>
  instantiate(String _name = "",
              SPtr<Actor> _pParent = nullptr,
              Matrix4 _transform = Matrix4::IDENTITY);

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
  PKFORCEINLINE SPtr<Actor>
  getLastActor() { return m_actors[m_actors.size() - 1]; }

  /**
   * @brief Get all the actors in the scene.
   * @return A vector with all the actors.
   */
  Vector<SPtr<Actor>>
  getAllActors() const { return m_actors; }

  /**
   * @brief Get the actor in the index of the scene.
   * @param _index Actor position in the list.
   * @return The actor in the index.
   */
  SPtr<Actor>
  getActor(uint32 _index) { return m_actors[_index]; }

  /**
   * @brief Get the actor count.
   * @return The ammount of actors in this scene.
   */
  uint32
  getActorCount() { return static_cast<uint32>(m_actors.size()); }

  /**
   * @brief Find an actor by name.
   * @param _actorName Name of the actor.
   * @return Pointer to the actor.
   */
  SPtr<Actor>
  actorFind(String _actorName);

  /**
   * @brief Get an actor with a specific component.
   * @return A pointer to the actor.
   */
  template<typename T>
  SPtr<Actor>
  getActorWithComponent();

  /**
   * @brief Get a vector with all actors with a specific component.
   * @return A vector of actors.
   */
  template<typename T>
  Vector<SPtr<Actor>>
  getAllActorsWithComponent();

  /**
   * @brief Update all actors.
   * @param _deltaTime Time between frames.
   */
  void
  update(float _deltaTime);

  /**
   * @brief Clear all the data from the scene.
   */
  void
  clear();

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
  String m_name;
  bool m_isActive;
};
}