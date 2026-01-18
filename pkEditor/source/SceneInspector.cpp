/*****************************************************************************/
/**
 * @file    SceneInspector.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    17/01/2026
 * @brief   Inspector to show a scene graph in a window.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkModel.h"
#include "pkModelManager.h"
#include "pkUInterface.h"
#include "SceneInspector.h"

using pkEngineSDK::Color;
using pkEngineSDK::g_uInterface;
using pkEngineSDK::g_ModelManager;
using pkEngineSDK::UInterface;
using pkEngineSDK::UUID;
using pkEngineSDK::Model;
using pkEngineSDK::ModelManager;
using pkEngineSDK::to_string;

void
SceneInspector::setScene(const SPtr<Scene>& _pScene)
{
  m_pScene = _pScene;
}

void
SceneInspector::createSceneGraphWindow()
{
  UInterface& im = g_uInterface();
  ModelManager& modelMan = g_ModelManager();

  if (im.createButton("+")) {
    m_pScene->instantiate("Actor");
    m_sActorIndex = m_pScene->getActorCount() - 1;
    m_pSelectedActor = m_pScene->getActor(m_sActorIndex);
  }
  if (im.beginDragDropTarget()) {
    const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
    if (id) {
      SPtr<Model> model = modelMan.createModel(*id);
      if (model) {
        SPtr<Actor> newActor = m_pScene->instantiate(model->getName());
        newActor->addComponent(model);
        m_pSelectedActor = newActor;
        m_sActorIndex = m_pScene->getActorCount() - 1;
      }
    }
    im.endDragDropTarget();
  }
  if (m_pSelectedActor) {
    im.sameLine();
    if (im.createButton("Delete")) {
      m_pSelectedActor->~Actor();
      m_pSelectedActor = nullptr;
      m_pScene->m_actors.erase(m_pScene->m_actors.begin() + m_sActorIndex);
      m_sActorIndex = 0;
    }
    im.sameLine();
    if (im.createButton("^")) {
      m_pSelectedActor = nullptr;
      m_sActorIndex = 0;
    }
  }
  // iterate through all actors in the scene.
  const uint32 actorCount = m_pScene->getActorCount();
  for (uint32 i = 0; i < actorCount; ++i) {
    SPtr<Actor> currentActor = m_pScene->getActor(i);
    Color baseColor = Color(255, 10);
    // highlight selected actor.
    if (m_sActorIndex == i && m_pSelectedActor) {
      baseColor = Color(255, 50);
    }
    // create button for the actor.
    if (im.createButton(currentActor->getName(),
                        baseColor,
                        Color(50, 50),
                        Color(100, 50),
                        true)) {
      m_pSelectedActor = currentActor;
      m_sActorIndex = i;
    }
  }
}
