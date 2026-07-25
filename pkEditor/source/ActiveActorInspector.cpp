/*****************************************************************************/
/**
 * @file    ActiveActorInspector.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    11/03/2026
 * @brief   Inspector for the currently selected actor.
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "ActiveActorInspector.h"
#include "pkModelCodec.h"
#include "pkModelManager.h"
#include "pkModelResource.h"
#include "pkModel.h"
#include "pkPath.h"
#include "pkUInterface.h"
#include "pkUUID.h"
#include "TransformInspector.h"

using pkEngineSDK::BaseResource;
using pkEngineSDK::Component;
using pkEngineSDK::g_uInterface;
using pkEngineSDK::g_ModelCodec;
using pkEngineSDK::g_ModelManager;
using pkEngineSDK::make_shared;
using pkEngineSDK::int32;
using pkEngineSDK::Model;
using pkEngineSDK::ModelCodec;
using pkEngineSDK::ModelManager;
using pkEngineSDK::ModelResource;
using pkEngineSDK::Path;
using pkEngineSDK::PK_TREENODE_FLAGS::kPK_DefaultOpen;
using pkEngineSDK::String;
using pkEngineSDK::toUint32;
using pkEngineSDK::uint32;
using pkEngineSDK::UInterface;
using pkEngineSDK::UUID;
using pkEngineSDK::Vector;

void
ActiveActorInspector::init(Window& _window, SPtr<Actor>& _pActor)
{
  UInterface& im = g_uInterface();
  ModelManager& modelMan = g_ModelManager();
  ModelCodec& modelCod = g_ModelCodec();
  // transform window
  if (im.collapsingHeader("Transform", kPK_DefaultOpen)) {
    String name = _pActor->getName();
    im.createText("Name:   ");
    im.sameLine();
    if (im.createInputText("##Name", &name)) {
      _pActor->setName(name);
    }
    // activity checkbox
    im.sameLine();
    im.createCheckBox("##ActiveActor", _pActor->isActive());
    // inspect actor transform matrix
    TransformInspector::inspect(_pActor);
  }
  // ---- Components window ---- //
  if (im.collapsingHeader("Components Window", kPK_DefaultOpen)) {
    // to do: change this to a more efficient option
    const Vector<String> options = { "model", "light", "camera" };
    int32 val = -1;
    if (im.beginCombo("Components", val, options)) {
      if (im.beginDragDropTarget()) {
        const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
        if (id) {
          SPtr<Model> loadedModel = modelMan.createModel(*id);
          modelMan.insertModel(*id, loadedModel);
        }
        im.endDragDropTarget();
      }
      // if a model component is to be added.
      if (val == 0) {
        Vector<Path> path = _window.openFileFromExplorer("Model Files",
                                                         modelCod.getPossibleExtensions());
        const uint32 fileCount = toUint32(path.size());
        for (uint32 i = 0; i < fileCount; ++i) {
          Path file = path[i];
          if (file.toString() != "") {
            SPtr<BaseResource> resource = make_shared<ModelResource>();
            resource->softLoad(file);
            const SPtr<Model> model = modelMan.createModel(resource->m_id);
            modelMan.insertModel(resource->m_id, model);
            _pActor->addComponent(model);
          }
        }
      }
    }
    if (im.beginDragDropTarget()) {
      UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
      if (id) {
        const SPtr<Model> model = modelMan.createModel(*id);
        modelMan.insertModel(*id, model);
        _pActor->addComponent(model);
      }
      im.endDragDropTarget();
    }
  }
}