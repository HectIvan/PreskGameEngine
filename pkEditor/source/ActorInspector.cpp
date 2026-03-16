/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "ActorInspector.h"
#include "pkAssetResourceManager.h"
#include "pkCamera.h"
#include "pkModelManager.h"
#include "pkLight.h"
#include "pkLogger.h"
#include "pkMaterial.h"
#include "pkMaterialManager.h"
#include "pkModel.h"
#include "pkPath.h"
#include "pkPlatformMath.h"
#include "pkPrerequisitesCore.h"
#include "pkTextureManager.h"
#include "pkUInterface.h"
#include "pkVector3.h"
#include "pkVector4.h"
#include "pkModelCodec.h"
#include "pkUUID.h"

using pkEngineSDK::ANSICHAR;
using pkEngineSDK::Camera;
using pkEngineSDK::CameraDesc;
using pkEngineSDK::Color;
using pkEngineSDK::COMPONENT_TYPE::kCamera;
using pkEngineSDK::COMPONENT_TYPE::kLight;
using pkEngineSDK::COMPONENT_TYPE::kMaterial;
using pkEngineSDK::COMPONENT_TYPE::kModel;
using pkEngineSDK::COMPONENT_TYPE::kUnknown;
using pkEngineSDK::ModelManager;
using pkEngineSDK::g_ModelManager;
using pkEngineSDK::g_Logger;
using pkEngineSDK::g_MaterialManager;
using pkEngineSDK::g_ModelCodec;
using pkEngineSDK::g_TextureManager;
using pkEngineSDK::g_uInterface;
using pkEngineSDK::Light;
using pkEngineSDK::Material;
using pkEngineSDK::MaterialManager;
using pkEngineSDK::MaterialProps;
using pkEngineSDK::Math;
using pkEngineSDK::Matrix4;
using pkEngineSDK::Mesh;
using pkEngineSDK::Model;
using pkEngineSDK::ModelCodec;
using pkEngineSDK::Path;
using pkEngineSDK::reinterpret_pointer_cast;
using pkEngineSDK::String;
using pkEngineSDK::stringToLower;
using pkEngineSDK::Texture;
using pkEngineSDK::TextureManager;
using pkEngineSDK::to_string;
using pkEngineSDK::uint32;
using pkEngineSDK::UInterface;
using pkEngineSDK::UUID;
using pkEngineSDK::Vector;
using pkEngineSDK::Vector2;
using pkEngineSDK::Vector3;
using pkEngineSDK::Vector4;

void
ActorInspector::inspectComponents(SPtr<Actor> _pActor, SPtr<Material>& _pMaterialInspect)
{
  // get the user interface manager
  UInterface& im = g_uInterface();
  MaterialManager& matMan = g_MaterialManager();
  ModelCodec& modelCod = g_ModelCodec();
  // for each type of component
  // non constant in case a component is removed in runtime.
  uint32 compCount = _pActor->getComponentCount();
  for (uint32 i = 0; i < compCount; ++i) {
    im.pushID(i);
    SPtr<Component> pComponent = _pActor->getComponent(i);
    // remove the component from the list and update the component count.
    if (im.createButton("X")) {
      _pActor->removeComponent(i);
      --compCount;
    }
    if (im.isItemHovered()) {
      im.setTooltip("Remove Component.");
    }
    switch (pComponent->getType())
    {
    case kCamera:
    {
      bool isChanged = false;
      if (im.collapsingHeader("Camera")) {
        // Component activity
        im.createCheckBox("Active ", pComponent->isActive());
        // camera sections
        SPtr<Camera> cam = reinterpret_pointer_cast<Camera>(pComponent);
        CameraDesc cDesc(cam->getDescriptor());
        im.createText("Camera");
        // parameter change
        im.createText("Half FOV");
        im.sameLine();
        if (im.createDrag("##HFOV", cDesc.halfFOV, 1.0f)) {
          isChanged = true;
        }
        im.createText("Near    ");
        im.sameLine();
        if (im.createDrag("##Near", cDesc.nearZ, 1.0f)) {
          isChanged = true;
        }
        im.createText("Far     ");
        im.sameLine();
        if (im.createDrag("##Far", cDesc.farZ, 1.0f)) {
          isChanged = true;
        }
        // initialize the camera with the new parameters
        if (isChanged) {
          cDesc.eye = cam->getEye().xyz();
          cDesc.forward = cam->getForward();
          cDesc.right = Vector3::RIGHT;
          cam->init(cDesc);
        }
      }
      break;
    }
    case kLight:
    {
      if (im.collapsingHeader("Light")) {
        // Component activity
        im.createCheckBox("Active ", pComponent->isActive());
        // light section
        SPtr<Light> light = reinterpret_pointer_cast<Light>(pComponent);
        // Light color
        im.colorEdit("Color", light->m_color);
        // Light direction
        Vector4 dir4 = _pActor->m_transform * Vector4(light->m_direction, 0.0f);
        Vector3 dir = dir4.xyz().normalized();
        im.createDrag("Direction", dir, 0.0f);
        // spot exponent
        im.createDrag("Spot Exponent", light->m_spotExponent, 0.1f, 0.0f, 1.0f);
        // Spot cutoff
        im.createDrag("Spot Cutoff", light->m_spotCutoff, 0.01f, 0.0f, 180.0f);
        // shadow intensity
        im.createDrag("Shadow Intensity", light->m_shadowIntensity, 0.05f, 0.0f, 1.0f);
        // Specular Intensity
        im.createDrag("Specular Intensity", light->m_specIntensity, 0.05f, 0.0f, 1.0f);
      }
      break;
    }
    case kModel:
    { // to do: the ammount of code here is getting a bit ridiculous.
      const ANSICHAR* name = pComponent->getName();
      if (im.collapsingHeader(name)) {
        // Component activity
        im.createCheckBox("Active ", pComponent->isActive());
        im.sameLine();
        SPtr<Model> model = reinterpret_pointer_cast<Model>(pComponent);
        if (im.createButton("Save")) {
          const Path resourcePath = Path(PK_RESOURCE_FOLDER + String(model->getName()) + ".pkm");
          modelCod.createResourceFromModel(model, resourcePath);
        }
        if (im.isItemHovered()) {
          im.setTooltip("Save model as a resource.");
        }
        
        // model section
        Vector<SPtr<Mesh>> meshes = model->getMeshes();
        // display total model vertex count.
        uint32 modelDataCount = static_cast<uint32>(model->vertex.size());
        String countModel = "Vertex Count: " + to_string(modelDataCount);
        im.createText(countModel.c_str());
        // display total model index count.
        modelDataCount = static_cast<uint32>(model->index.size());
        countModel = "Index Count: " + to_string(modelDataCount);
        im.createText(countModel.c_str());

        // search specific mesh
        im.createText("Meshes: ");
        im.createText("Search: ");
        im.sameLine();
        im.createInputText("##Search", &m_searchMesh);
        for (uint32 j = 0; j < meshes.size(); ++j) {
          // get mesh
          SPtr<Mesh> mesh = meshes[j];
          String name = mesh->getName();
          const String searchMeshLower = stringToLower(m_searchMesh);
          const String meshNameLower = stringToLower(name);
          // if there is no search filter
          if (meshNameLower.find(searchMeshLower.c_str()) != String::npos) {
            if (im.collapsingHeader(name.c_str())) {
              // Mesh geometry data
              im.createText("Vertex Count: ");
              im.sameLine();
              String vertexCount = to_string(mesh->vertexVector.size());
              im.createText(vertexCount.c_str());
              im.createText("Index Count: ");
              im.sameLine();
              String indexCount = to_string(mesh->indexVector.size());
              im.createText(indexCount.c_str());
              im.pushID(j);
              im.createCheckBox("Active ", mesh->getActive());
              // Mesh material data
              if (!mesh->material) {
                im.pushID(j);
                if (im.createButton("<Create new material>")) {
                  mesh->material = matMan.newMaterial();
                }
                im.popID();
              }
              else {
                const ANSICHAR* matName = mesh->material->getName();
                const String matNameString = String(matName);
                if (im.selectable2("##Inspect", Vector2(50))) {
                  _pMaterialInspect = mesh->material;
                }
                if (im.beginDragDropTarget()) {
                  const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
                  if (id) {
                    SPtr<Material> ddMat = matMan.loadMaterial(*id);
                    mesh->material = ddMat;
                  }
                  im.endDragDropTarget();
                }
                if (im.isItemHovered()) {
                  const String tooltip = "Inspect " + matNameString + ".";
                  im.setTooltip(tooltip.c_str());
                }
                im.sameLine();
                im.createText(matName);
              }
              im.popID();
            }
          }
        }
      }
      break;
    }
    case kUnknown:
    {
      if (im.collapsingHeader("???")) {

      }
      return;
    }
    default:
      break;
    }
    im.popID();
  }
}