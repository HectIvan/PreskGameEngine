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
  SPtr<Actor> gActor = pk_shared_ptr_new<Actor>("", Matrix4::IDENTITY);
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
Scene::instantiate(const ANSICHAR* _name,
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
    Vector<SPtr<Actor>> children = _pParent->getChildren();
    insertActor(actor, children);
    // set the parent as the parent of the current actor.
    actor->setParent(_pParent);
  }
  // otherwise, the actor is part of the scene.
  else { insertActor(actor, m_actors); }
  // log registry.
  const String message = "Instantiated actor of name: " + String(_name);
  LOG_REGISTER(message, __FILE__, __LINE__);
  // return the actor created.
  return actor;
  
}

SPtr<Actor>
Scene::instantiate(const ANSICHAR* _name,
                   const Vector3& _position,
#if PK_ROT == PK_ROT_QUATERNION
                   const Quaternion& _rotation,
#elif PK_ROT == PK_ROT_EULER
                   const Vector3& _rotation,
#endif
                   const Vector3& _scale,
                   const SPtr<Actor>& _pParent)
{
  Matrix4 transform = Matrix4::rotation(_rotation);
  transform.setTranslation(_position);
  transform.setScale(_scale);

  return instantiate(_name, _pParent, transform);
}

void
Scene::deleteActor(const uint32& _index)
{
  m_actors[_index]->~Actor();
  m_actors[_index] = nullptr;
  m_actors.erase(m_actors.begin() + _index);
}

SPtr<Actor>
Scene::actorFind(const String& _actorName)
{
  const uint32 actorCount = getActorCount();
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
Scene::update(const float& _deltaTime) const
{
  const uint32 actorCount = getActorCount();
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
  const uint32 actorCount = toUint32(m_actors.size());
  for (uint32 i = 0; i < actorCount; ++i) {
    m_actors[i]->clear();
  }
  m_actors.clear();
  m_name = "";
  m_isActive = false;
}

void
Scene::updateActor(const SPtr<Actor>& _pActor, const float& _deltaTime) const
{
  _pActor->update(_deltaTime);
  const uint32 childCount = _pActor->getChildCount();
  for (uint32 i = 0; i < childCount; ++i) {
    updateActor(_pActor->getChild(i), _deltaTime);
  }
}
}