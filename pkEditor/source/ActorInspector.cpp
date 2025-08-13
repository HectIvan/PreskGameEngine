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
#include "pkLight.h"
#include "pkMaterial.h"
#include "pkModel.h"
#include "pkLogger.h"
#include "pkPath.h"
#include "pkPlatformMath.h"
#include "pkPrerequisitesCore.h"
#include "pkTextureManager.h"
#include "pkUInterface.h"
#include "pkVector3.h"
#include "pkVector4.h"

using pkEngineSDK::Camera;
using pkEngineSDK::CameraDesc;
using pkEngineSDK::Color;
using pkEngineSDK::COMPONENT_TYPE::kCamera;
using pkEngineSDK::COMPONENT_TYPE::kLight;
using pkEngineSDK::COMPONENT_TYPE::kMaterial;
using pkEngineSDK::COMPONENT_TYPE::kModel;
using pkEngineSDK::COMPONENT_TYPE::kUnknown;
using pkEngineSDK::g_TextureManager;
using pkEngineSDK::g_uInterface;
using pkEngineSDK::Light;
using pkEngineSDK::Material;
using pkEngineSDK::Math;
using pkEngineSDK::Matrix4;
using pkEngineSDK::Mesh;
using pkEngineSDK::Model;
using pkEngineSDK::Path;
using pkEngineSDK::reinterpret_pointer_cast;
using pkEngineSDK::String;
using pkEngineSDK::Texture;
using pkEngineSDK::TextureManager;
using pkEngineSDK::uint32;
using pkEngineSDK::UInterface;
using pkEngineSDK::Vector;
using pkEngineSDK::Vector2;
using pkEngineSDK::Vector3;
using pkEngineSDK::Vector4;

using pkEngineSDK::g_Logger;

ActorInspector::ActorInspector(SPtr<Actor> _pActor)
{
  m_actor = _pActor;
}
/*
void
buttonForTexture(String _name, String _tooltip, SPtr<Texture>& _pTexture, Window& _window)
{
  TextureManager& tm = g_TextureManager().instance();
  UInterface& im = g_uInterface().instance();
  // create the buttons
  if (im.createButtonImage(_name.c_str(), _pTexture)) {
    // opened window to set diffuse texture
    Path path(_window.openFileFromExplorer());
    if (path.toString() != "") {
      SPtr<Texture> texture = tm.loadTexture(path);
      meshMat->setDiffuse(texture);
    }
  }
  // hover tooltip.
  if (im.isItemHovered()) {
    im.setTooltip("Diffuse Texture");
  }
}*/

void
ActorInspector::Inspect()
{
  // get the user interface manager
  UInterface& im = g_uInterface().instance();
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
                                      float& _imgTexSize)
{
  Vector2 texSize = Vector2(_imgTexSize, _imgTexSize);
  // get the user interface manager
  UInterface& im = g_uInterface().instance();
  TextureManager& tm = g_TextureManager().instance();
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
      // model section
      SPtr<Model> model = reinterpret_pointer_cast<Model>(_pComponent);
      Vector<SPtr<Mesh>> meshes = model->getMeshes();
      im.sameLine();
      im.createText("Search: ");
      im.sameLine();
      im.createInputText("##Search", &_searchMesh);
      im.createText("Meshes: ");
      for (uint32 i = 0; i < meshes.size(); ++i) {
        // get mesh
        SPtr<Mesh> mesh = meshes[i];
        String name = mesh->getName();
        // if there is no search filter
        if (name.find(_searchMesh.c_str()) != String::npos) {
          if (im.collapsingHeader(name.c_str())) {
            SPtr<Material> material = mesh->material;
            im.pushID(i);
            im.createCheckBox("Active ", mesh->getActive());
            // get material
            SPtr<Material> meshMat = mesh->material;
            // get the textures
            SPtr<Texture> diffuse = meshMat->m_diffuse;
            SPtr<Texture> normal = meshMat->m_normal;
            SPtr<Texture> occlusion = meshMat->m_occlusion;
            SPtr<Texture> rough = meshMat->m_roughness;
            SPtr<Texture> metallic = meshMat->m_metallic;
            SPtr<Texture> emissive = meshMat->m_emissive;

            // get the names of the texture and material
            String matName = meshMat->getNameS();
            String difName = diffuse->getName().toString() + matName + "diff";
            String norName = normal->getName().toString() + matName + "norm";
            String occName = occlusion->getName().toString() + matName + "occ";
            String roughName = rough->getName().toString() + matName + "rough";
            String metName = metallic->getName().toString() + matName + "metal";
            String emissName = emissive->getName().toString() + matName + "Emiss";

            // create the buttons
            if (im.createButtonImage(difName.c_str(), diffuse, texSize)) {
              // opened window to set diffuse texture
              Path path(_window.openFileFromExplorer());
              if (path.toString() != "") {
                SPtr<Texture> texture = tm.loadTexture(path);
                meshMat->setDiffuse(texture);
              }
            }
            // hover tooltip.
            if (im.isItemHovered()) {
              im.setTooltip("Diffuse Texture");
            }
            im.sameLine();
            if (im.createButtonImage(norName.c_str(), normal, texSize)) {
              // opened window to set normal texture
              Path path(_window.openFileFromExplorer());
              if (path.toString() != "") {
                SPtr<Texture> texture = tm.loadTexture(path);
                meshMat->setNormal(texture);
              }
            }
            // hover tooltip.
            if (im.isItemHovered()) {
              im.setTooltip("Normal Texture");
            }
            im.sameLine();
            if (im.createButtonImage(occName.c_str(), occlusion, texSize)) {
              // opened window to set occlusion texture
              Path path(_window.openFileFromExplorer());
              if (path.toString() != "") {
                SPtr<Texture> texture = tm.loadTexture(path);
                meshMat->setOcclusion(texture);
              }
            }
            // hover tooltip.
            if (im.isItemHovered()) {
              im.setTooltip("Ambient Occlusion Texture");
            }
            im.sameLine();
            if (im.createButtonImage(roughName.c_str(), rough, texSize)) {
              // opened window to set rough texture
              Path path(_window.openFileFromExplorer());
              if (path.toString() != "") {
                SPtr<Texture> texture = tm.loadTexture(path);
                meshMat->setRoughness(texture);
              }
            }
            // hover tooltip.
            if (im.isItemHovered()) {
              im.setTooltip("Roughness Texture");
            }
            im.sameLine();
            if (im.createButtonImage(metName.c_str(), metallic, texSize)) {
              // opened window to set metallic texture
              Path path(_window.openFileFromExplorer());
              if (path.toString() != "") {
                SPtr<Texture> texture = tm.loadTexture(path);
                meshMat->setMetallic(texture);
              }
            }
            // hover tooltip.
            if (im.isItemHovered()) {
              im.setTooltip("Metallic Texture");
            }
            im.sameLine();
            if (im.createButtonImage(emissName.c_str(), emissive, texSize)) {
              // opened window to set metallic texture
              Path path(_window.openFileFromExplorer());
              if (path.toString() != "") {
                SPtr<Texture> texture = tm.loadTexture(path);
                meshMat->setEmissive(texture);
              }
            }
            // hover tooltip.
            if (im.isItemHovered()) {
              im.setTooltip("Emissive Texture");
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