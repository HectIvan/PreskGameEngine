/*****************************************************************************/
/**
 * @file    pkActorCodec.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    05/12/2025
 * @brief   Actor codec for the game engine.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkActorCodec.h"
#include "pkActor.h"
#include "pkActorResource.h"
#include "pkLogger.h"
#include "pkModelCodec.h"
#include "pkLight.h"
#include "pkUUID.h"

namespace pkEngineSDK
{

SPtr<BaseResource>
ActorCodec::createResource(const SPtr<Actor>& _pActor)
{
  PK_ASSERT(_pActor);
  Logger& log = g_Logger();
  ModelCodec& modelCodec = g_ModelCodec();

  // try to create a file for the actor.
  const String actorName = _pActor->getName();
  ofstream file(actorName + ".pka", ios::out | ios::binary);

  if (!file.is_open()) {
    const String msg = "Failed to create actor resource file: " + actorName + ".pka";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    return nullptr;
  }

  // get the base actor data and save it.
  const SIZE_T v3Size = sizeof(Vector3);

  const Vector3 position = _pActor->m_position;
  const Vector3 rotation = _pActor->m_rotation;
  const Vector3 scale = _pActor->m_scale;
  const bool isActive = _pActor->isActive();
  const uint32 componentCount = static_cast<uint32>(_pActor->m_components.size());

  SPtr<ActorResource> actorRes = make_shared<ActorResource>();
  actorRes->m_position = position;
  actorRes->m_rotation = rotation;
  actorRes->m_scale = scale;
  actorRes->m_isActive = isActive;
  
  const String id = UUID::generateRandomUUIDFromString(actorName);
  actorRes->writeBaseHeader(file, id, "", actorName + ".pka");

  file.write(reinterpret_cast<const ANSICHAR*>(&position), v3Size);
  file.write(reinterpret_cast<const ANSICHAR*>(&rotation), v3Size);
  file.write(reinterpret_cast<const ANSICHAR*>(&scale), v3Size);
  file.write(reinterpret_cast<const ANSICHAR*>(&isActive), sizeof(bool));
  file.write(reinterpret_cast<const ANSICHAR*>(&componentCount), sizeof(uint32));

  // if the actor has a parent, save its ID.
  if (_pActor->m_parent) {
    const String parentID = UUID::generateRandomUUIDFromString(_pActor->m_parent->getName());
    const SIZE_T idSize = parentID.length();
    file.write(reinterpret_cast<const ANSICHAR*>(true), sizeof(bool));
    file.write(reinterpret_cast<const ANSICHAR*>(&idSize), sizeof(SIZE_T));
    file.write(reinterpret_cast<const ANSICHAR*>(&parentID), idSize);
  }
  else {
    const SIZE_T idSize = UUID::PK_DEFAULT_UUID.length();
    file.write(reinterpret_cast<const ANSICHAR*>(false), sizeof(bool));
    file.write(reinterpret_cast<const ANSICHAR*>(&idSize), sizeof(SIZE_T));
    file.write(reinterpret_cast<const ANSICHAR*>(&UUID::PK_DEFAULT_UUID), sizeof(ANSICHAR));
  }

  // iterate through the components and save them.
  for (uint32 i = 0; i < componentCount; ++i) {
    SPtr<Component> pComp = _pActor->m_components[i];

    const COMPONENT_TYPE::E compType = pComp->getType();
    file.write(reinterpret_cast<const ANSICHAR*>(&compType), sizeof(COMPONENT_TYPE::E));

    // if the component type is unknown, skip it.
    if (compType == COMPONENT_TYPE::kUnknown) {
      continue;
    }
    // if the component is a model, save it as a pkm.
    if (compType == COMPONENT_TYPE::kModel) {
      auto model = reinterpret_pointer_cast<Model>(pComp);
      const Path resourcePath = Path("resources/" + String(model->getName()) + ".pkm");
      SPtr<BaseResource> modelRes = modelCodec.createResourceFromModel(model, resourcePath);

      // save the resource id.
      const SIZE_T idSize = modelRes->m_id.length();
      file.write(reinterpret_cast<const ANSICHAR*>(idSize), sizeof(SIZE_T));
      file.write(reinterpret_cast<const ANSICHAR*>(modelRes->m_id.c_str()), idSize);
    }
    // if the component is a light, save it in the actor itself.
    if (compType == COMPONENT_TYPE::kLight) {
      auto light = reinterpret_pointer_cast<Light>(pComp);
    }
    // if the component is a camera, save it in the actor itself.
    if (compType == COMPONENT_TYPE::kCamera) {
      
    }
  }
  file.close();

  // recursive saving of children.
  const uint32 childCount = static_cast<uint32>(_pActor->m_children.size());
  for (uint32 i = 0; i < childCount; ++i) {
    createResource(_pActor->m_children[i]);
  }


  return actorRes;
}
}
