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
  gActor->setTransform(Matrix4::IDENTITY);
  gActor->m_name = "";
  return gActor;
}

/**
 * @brief Insert an actor into the actors vector.
 * @param _pActor Actort to insert.
 * @param _vector Vector where the actor will be inserted.
 */
void
insertActor(SPtr<Actor> _pActor, Vector<SPtr<Actor>>& _vector)
{
  _vector.push_back(_pActor);
}

SPtr<Actor>
Scene::instantiate(String _name,
                   SPtr<Actor> _pParent,
                   Matrix4 _transform)
{
  // insert the actor into the vector of actors
  SPtr<Actor> actor = createActor();
  // set the actor name.
  actor->m_name = _name;
  // set the actor transform
  actor->setTransform(_transform);
  // if the parent is not a nullptr (there is a parent that will have this actor)
  if (_pParent) {
    // insert the current actor to the children vector of the parent
    insertActor(actor, _pParent->m_children);
    // set the parent as the parent of the current actor.
    actor->m_parent = _pParent;
  }
  // otherwise, the actor is part of the scene
  else { insertActor(actor, m_actors); }
  // return the actor created.
  return actor;
}

SPtr<Actor>
Scene::actorFind(String _actorName)
{
  // for each actor in the list
  for (uint32 i = 0; i < getAllActors().size(); ++i) {
    // check if the name is the one we're looking for
    if (getActor(i)->m_name == _actorName) {
      return getActor(i);
    }
  }
  // if no actor fits the name.
  return nullptr;
}

template<typename T>
SPtr<Actor>
Scene::getActorWithComponent()
{
  // check each actor
  for (uint32 i = 0; i < getAllActors().size(); ++i) {
    // check if the data type return is not null
    SPtr<T> check = getActor(i)->getComponent<T>();
    if (check) {
      // if its not null, return the final value
      return getActor(i);
    }
  }
}

template<typename T>
Vector<SPtr<Actor>>
Scene::getAllActorsWithComponent()
{
  // actor list
  Vector<SPtr<Actor>> list;
  // check each actor
  for (uint32 i = 0; i < getAllActors().size(); ++i) {
    // check if the data type return is not null
    SPtr<T> check = getActor(i)->getComponent<T>();
    if (check) {
      // if its not null, return the final value
      list.push_back(getActor(i));
    }
  }
  return list;
}

void
Scene::update(float _deltaTime)
{
  for (uint32 i = 0; i < getAllActors().size(); ++i) {
    if (getActor(i)->m_active) {
      SPtr<Actor> actor = getActor(i);
      updateActor(actor, _deltaTime);
    }
  }
}

void
Scene::clear()
{
  for (uint32 i = 0; i < m_actors.size(); ++i) {
    m_actors[i]->clear();
  }
  m_actors.clear();
  m_isActive = false;
}

void
Scene::updateActor(SPtr<Actor> _pActor, float _deltaTime)
{
  _pActor->update(_deltaTime);
  for (uint32 i = 0; i < _pActor->m_children.size(); ++i) {
    updateActor(_pActor->m_children[i], _deltaTime);
  }
}
}