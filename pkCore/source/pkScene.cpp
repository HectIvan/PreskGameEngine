/*****************************************************************************/
/**
 * @file    pkScene.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    2025/01/29
 * @brief   Scene used for the engine.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

#include "pkScene.h"
#include "pkTimeManager.h"
#include "pkLogger.h"

namespace pkEngineSDK
{

/**
 * @brief Create an actor.
 * @return An actor pointer.
 */
SPtr<Actor>
createActor()
{
  SPtr<Actor> gActor = make_shared<Actor>();
  gActor->setTransform();
  gActor->setName("");
  return gActor;
}

/**
 * @brief Insert an actor into the actors vector.
 * @param _pActor Actort to insert.
 * @param _vector Vector where the actor will be inserted.
 */
void
insertActor(const SPtr<Actor>& _pActor, Vector<SPtr<Actor>>& _vector)
{
  _vector.push_back(_pActor);
}

SPtr<Actor>
Scene::instantiate(const String& _name,
                   const SPtr<Actor>& _pParent,
                   const Matrix4& _transform)
{
  // insert the actor into the vector of actors.
  SPtr<Actor> actor = createActor();
  // search for other actors with the same name in the same scene.
  uint32 index = 0;
  bool found;
  do {
    found = false;
    // if the search name is not first, append a number to the search number
    String searchName = _name;
    if (index != 0) {
      searchName += to_string(index);
    }
    if (actorFind(searchName)) {
      found = true;
      ++index;
    }
  } while (found);
  // if the name is not unique in the scene
  String newName = _name;
  if (index != 0) {
    newName += to_string(index);
  }
  // set the actor name.
  actor->setName(newName);
  // set the actor transform.
  actor->setTransform(_transform);
  // if the parent is not a nullptr (there is a parent that will have this actor).
  if (_pParent) {
    // insert the current actor to the children vector of the parent.
    insertActor(actor, _pParent->m_children);
    // set the parent as the parent of the current actor.
    actor->m_parent = _pParent;
  }
  // otherwise, the actor is part of the scene.
  else { insertActor(actor, m_actors); }
  // log registry.
  const String message = "Instantiated actor of name: " + _name;
  LOG_REGISTER(message, __FILE__, __LINE__);
  // return the actor created.
  return actor;
  
}

SPtr<Actor>
Scene::actorFind(const String& _actorName)
{
  const uint32 actorCount = static_cast<uint32>(getAllActors().size());
  // for each actor in the list
  for (uint32 i = 0; i < actorCount; ++i) {
    // check if the name is the one we're looking for
    const SPtr<Actor> actor = getActor(i);
    if (actor->getName() == _actorName) {
      return actor;
    }
  }
  // if no actor fits the name.
  return nullptr;
}

void
Scene::update(const float& _deltaTime)
{
  const uint32 actorCount = static_cast<uint32>(getAllActors().size());
  for (uint32 i = 0; i < actorCount; ++i) {
    const SPtr<Actor> actor = getActor(i);
    if (actor->isActive()) {
      updateActor(actor, _deltaTime);
    }
  }
}

void
Scene::clear()
{
  const uint32 actorCount = static_cast<uint32>(m_actors.size());
  for (uint32 i = 0; i < actorCount; ++i) {
    m_actors[i]->clear();
  }
  m_actors.clear();
  m_name = "";
  m_isActive = false;
}

void
Scene::updateActor(const SPtr<Actor>& _pActor, const float& _deltaTime)
{
  _pActor->update(_deltaTime);
  const uint32 childCount = static_cast<uint32>(_pActor->m_children.size());
  for (uint32 i = 0; i < childCount; ++i) {
    updateActor(_pActor->m_children[i], _deltaTime);
  }
}
}