/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "ActorInspector.h"

/*********************************************/
/**
* Includes
**/
/*********************************************/
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

ActorInspector::ActorInspector(SPtr<Actor> _pActor)
{
  m_actor = _pActor;
}
/*
void
buttonForTexture(String _name, String _tooltip, SPtr<Texture>& _pTexture, Window& _window)
{
  TextureManager& tm = g_TextureManager();
  UInterface& im = g_uInterface();
  // create the buttons
  if (im.createButtonImage(_name.c_str(), _pTexture)) {
    // opened window to set albedo texture
    Path path(_window.openFileFromExplorer());
    if (path.toString() != "") {
      SPtr<Texture> texture = tm.loadTexture(path);
      meshMat->setAlbedo(texture);
    }
  }
  // hover tooltip.
  if (im.isItemHovered()) {
    im.setTooltip("Albedo Texture");
  }
}*/

void
ActorInspector::Inspect()
{
  // get the user interface manager
  UInterface& im = g_uInterface();
  MaterialManager& matMan = g_MaterialManager();
  // change the position
  Vector3 newTranslation = m_actor->m_position;
  im.createText("Position");
  im.sameLine();
  if (im.createDrag3("##Position", newTranslation)) {
    m_actor->setPosition(newTranslation);
    // m_actor->setPositionLocal(newTranslation);
  }
  // change the rotation
  Vector3 newRotation = m_actor->m_rotation;
  im.createText("Rotation");
  im.sameLine();
  im.createDrag3("##Rotation",newRotation, 1.0f);
  m_actor->setRotation(newRotation);
  // change the scale
  Vector3 newScale = m_actor->m_scale;
  im.createText("Scale   ");
  im.sameLine();
  im.createDrag3("##Scale", newScale);
  m_actor->setScale(newScale);
}

void
ActorInspector::createComponentWindow(SPtr<Component>& _pComponent,
                                      Window& _window,
                                      String& _searchMesh,
                                      float& _imgTexSize,
                                      SPtr<Material>& _pMaterialInspect)
{
  Vector2 texSize = Vector2(_imgTexSize, _imgTexSize);
  // get the user interface manager
  UInterface& im = g_uInterface();
  MaterialManager& matMan = g_MaterialManager();
  ModelCodec& modelCod = g_ModelCodec();
  // for each type of component
  im.PushStyleColor(Color(100, 100, 0, 125), Color(150, 150, 0, 125), Color(200, 200, 0, 125));
  switch (_pComponent->getType()) 
  {
  case kCamera:
  {
    bool isChanged = false;
    if (im.collapsingHeader("Camera")) {
      // Component activity
      im.createCheckBox("Active ", _pComponent->isActive());
      // camera sections
      SPtr<Camera> cam = reinterpret_pointer_cast<Camera>(_pComponent);
      CameraDesc cDesc(cam->m_descriptor);
      im.createText("Camera");
      // parameter change
      im.createText("Half FOV");
      im.sameLine();
      if (im.createDragF("##HFOV", cDesc.halfFOV, 1.0f)) {
        isChanged = true;
      }
      im.createText("Near    ");
      im.sameLine();
      if (im.createDragF("##Near", cDesc.nearZ, 1.0f)) {
        isChanged = true;
      }
      im.createText("Far     ");
      im.sameLine();
      if (im.createDragF("##Far", cDesc.farZ, 1.0f)) { 
        isChanged = true;
      }
      // initialize the camera with the new parameters
      if (isChanged) {
        cDesc.eye = cam->m_eye.xyz();
        cDesc.at = cam->m_at.xyz();
        cDesc.up = Vector3::UP;
        cam->init(cDesc);
      }
    }
    break;
  }
  case kLight:
  {
    if (im.collapsingHeader("Light")) {
      // Component activity
      im.createCheckBox("Active ", _pComponent->isActive());
      // light section
      SPtr<Light> light = reinterpret_pointer_cast<Light>(_pComponent);
      // Light color
      im.colorEdit("Color", light->m_color);
      // Light direction
      Vector4 dir4 = m_actor->m_transform * Vector4(light->m_direction, 0.0f);
      Vector3 dir = dir4.xyz().normalized();
      im.createDrag3("Direction", dir, 0.0f);
      // spot exponent
      im.createDragF("Spot Exponent", light->m_spotExponent, 0.1f, 0.0f, 1.0f);
      // Spot cutoff
      im.createDragF("Spot Cutoff", light->m_spotCutoff, 0.01f, 0.0f, 180.0f);
      // shadow intensity
      im.createDragF("Shadow Intensity", light->m_shadowIntensity, 0.05f, 0.0f, 1.0f);
      // Specular Intensity
      im.createDragF("Specular Intensity", light->m_specIntensity, 0.05f, 0.0f, 1.0f);
    }
    break;
  }
  case kMaterial:
  {
    if (im.collapsingHeader("Material")) {
      // Component activity
      im.createCheckBox("Active ", _pComponent->isActive());
    }
    break;
  }
  case kModel:
  { // to do: the ammount of code here is getting a bit ridiculous.
    if (im.collapsingHeader("Model")) {
      // Component activity
      im.createCheckBox("Active ", _pComponent->isActive());
      im.sameLine();
      SPtr<Model> model = reinterpret_pointer_cast<Model>(_pComponent);
      if (im.createButton("Save")) {
        const Path resourcePath = Path("resources/" + String(model->getName()) + ".pkm");
        modelCod.createResourceFromModel(model, resourcePath);
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
      im.createInputText("##Search", &_searchMesh);
      for (uint32 i = 0; i < meshes.size(); ++i) {
        // get mesh
        SPtr<Mesh> mesh = meshes[i];
        String name = mesh->getName();
        const String searchMeshLower = stringToLower(_searchMesh);
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
            im.pushID(i);
            im.createCheckBox("Active ", mesh->getActive());
            // Mesh material data
            if (!mesh->material) {
              im.pushID(i);
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
              // if (im.beginDragDropSource()) {
              //   const String dragText = "Dragging " + matNameString;
              //   im.createText(dragText.c_str());
              //   const UUID* textureID = mesh->material->getID();
              //   im.setDragDropPayload("RESOURCE_PAYLOAD", textureID, sizeof(UUID));
              //   im.endDragDropSource();
              // }
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
  im.popStyleColor(3);
}