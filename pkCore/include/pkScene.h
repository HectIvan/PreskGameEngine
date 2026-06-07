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

namespace pkEngineSDK
{

class PK_CORE_EXPORT Scene
{
 public:
  Scene() = default;
  virtual ~Scene() = default;

  /**
   * @brief Check if the scene is active.
   */
  PKFORCEINLINE bool
  isActive() const { return m_isActive; }

  /**
   * @brief Set if the scene is active.
   */
  PKFORCEINLINE void
  setActive(const bool& _isActive) { m_isActive = _isActive; }

  /**
   * @brief Get the name of the scene.
   */
  PKFORCEINLINE const String&
  getName() const { return m_name; }

  /**
   * @brief Set the name of the scene.
   */
  PKFORCEINLINE void
  setName(const String& _name) { m_name = _name; }

  /**
   * @brief Insert a new actor into the scene or as
   * a child of another actor.
   * @param _name Name of the scene.
   * @param _pParent If the actor will be the child of another actor.
   * @param _tranform Transform of the actor.
   * @return The actor created.
   */
  SPtr<Actor>
  instantiate(const String& _name = "",
              const SPtr<Actor>& _pParent = nullptr,
              const Matrix4& _transform = Matrix4::IDENTITY);

  /**
   * @brief Get the last inserted actor of the scene.
   * @return The last actor.
   */
  PKFORCEINLINE SPtr<Actor>
  getLastActor() const { return m_actors[m_actors.size() - 1]; }

  /**
   * @brief Get all the actors in the scene.
   * @return A vector with all the actors.
   */
  PKFORCEINLINE Vector<SPtr<Actor>>
  getAllActors() const { return m_actors; }

  /**
   * @brief Get the actor in the index of the scene.
   * @param _index Actor position in the list.
   * @return The actor in the index.
   */
  PKFORCEINLINE SPtr<Actor>
  getActor(const uint32& _index) const { return m_actors[_index]; }

  /**
   * @brief Delete an actor from the scene.
   * @param _index Actor position in the list.
   */
  void
  deleteActor(const uint32& _index);

  /**
   * @brief Get the last index of the actors vector.
   */
  PKFORCEINLINE uint32
  getLastIndex() const { return static_cast<uint32>(m_actors.size() - 1); }

  /**
   * @brief Get the actor count.
   * @return The ammount of actors in this scene.
   */
  PKFORCEINLINE uint32
  getActorCount() const { return static_cast<uint32>(m_actors.size()); }

  /**
   * @brief Find an actor by name.
   * @param _actorName Name of the actor.
   * @return Pointer to the actor.
   */
  SPtr<Actor>
  actorFind(const String& _actorName);

  /**
   * @brief Get an actor with a specific component.
   * @return A pointer to the actor.
   */
  template<typename T>
  SPtr<Actor>
  getActorWithComponent()
  {
    // check each actor
    const uint32 actorCount = getActorCount();
    for (uint32 i = 0; i < actorCount; ++i) {
      // check if the data type return is not null
      const SPtr<Actor> actor = getActor(i);
      const SPtr<T> check = actor->getComponent<T>();
      if (check) { // if its not null, return the final value
        return actor;
      }
    }
    return nullptr;
  }

  /**
   * @brief Get a vector with all actors with a specific component.
   * @return A vector of actors.
   */
  template<typename T>
  Vector<SPtr<Actor>>
  getAllActorsWithComponent()
  {
    // actor list
    Vector<SPtr<Actor>> list;
    const uint32 actorCount = getActorCount();
    // check each actor
    for (uint32 i = 0; i < actorCount; ++i) {
      // check if the data type return is not null
      const SPtr<Actor> actor = getActor(i);
      const SPtr<T> check = actor->getComponent<T>();
      if (check) {
        // if its not null, return the final value
        list.push_back(actor);
      }
    }
    return list;
  }

  /**
   * @brief Update all actors.
   * @param _deltaTime Time between frames.
   */
  void
  update(const float& _deltaTime) const;

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
  updateActor(const SPtr<Actor>& _pActor, const float& _deltaTime) const;

 private:
  Vector<SPtr<Actor>> m_actors;
  String m_name;
  bool m_isActive;
};
}