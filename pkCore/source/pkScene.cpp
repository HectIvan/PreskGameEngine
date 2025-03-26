/*****************************************************************************/
/**
 * @file    pkScriptManager.cpp
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    2025/01/29
 * @brief
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

#include "pkScene.h"

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
Scene::instantiate(Matrix4 _transform, SPtr<Actor> _pParent)
{
  // insert the game object into the vector of game objects
  SPtr<Actor> gObject = createActor();
  // set the gameObject transform
  gObject->setTransform(_transform);
  // if the parent is not a nullptr (there is a parent that will have this game object)
  if (_pParent) {
    // insert the current gameObject to the children vector of the parent
    insertActor(gObject, _pParent->m_children);
    // set the parent as the parent of the current game Object
    gObject->m_parent = _pParent;
  }
  // otherwise, the object is part of the scene
  else { insertActor(gObject, m_actors); }
  // return the object created.
  return gObject;
}
PK_CORE_EXPORT Scene&
g_sceneManager()
{
  return Scene::instance();
}
}