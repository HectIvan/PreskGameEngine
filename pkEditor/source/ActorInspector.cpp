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
#include "pkGPUResourceManager.h"
#include "pkLight.h"
#include "pkLogger.h"
#include "pkMaterial.h"
#include "pkModel.h"
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
using pkEngineSDK::GPUResourceManager;
using pkEngineSDK::g_GPUResourceManager;
using pkEngineSDK::g_TextureManager;
using pkEngineSDK::g_uInterface;
using pkEngineSDK::Light;
using pkEngineSDK::Material;
using pkEngineSDK::MaterialProps;
using pkEngineSDK::Math;
using pkEngineSDK::Matrix4;
using pkEngineSDK::Mesh;
using pkEngineSDK::Model;
using pkEngineSDK::Path;
using pkEngineSDK::reinterpret_pointer_cast;
using pkEngineSDK::String;
using pkEngineSDK::Texture;
using pkEngineSDK::TextureManager;
using pkEngineSDK::to_string;
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
  TextureManager& tm = g_TextureManager();
  UInterface& im = g_uInterface();
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
  UInterface& im = g_uInterface();
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
  UInterface& im = g_uInterface();
  TextureManager& tm = g_TextureManager();
  GPUResourceManager& GPUResourceMan = g_GPUResourceManager();
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
        // if there is no search filter
        if (name.find(_searchMesh.c_str()) != String::npos) {
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
            SPtr<Material> material = mesh->material;
            if (material->getName() != GPUResourceMan.m_defaultMaterial->getName()) {
              im.createText("Material Name: ");
              im.sameLine();
              im.createInputText("##MaterialName: ", &material->m_name);
              // get material
              SPtr<Material> meshMat = mesh->material;
              MaterialProps& matProps = meshMat->m_properties;
              String matName = meshMat->getNameS();
              // get the textures
              SPtr<Texture> diffuse = meshMat->m_diffuse;
              SPtr<Texture> normal = meshMat->m_normal;
              SPtr<Texture> oclussion = meshMat->m_oclussion;
              SPtr<Texture> rough = meshMat->m_roughness;
              SPtr<Texture> metallic = meshMat->m_metallic;
              SPtr<Texture> emissive = meshMat->m_emissive;

              /***************************************************************/
              /*------------------------diffuse button-----------------------*/
              /***************************************************************/
              if (diffuse) {
                const String difName = diffuse->getNameString() + "diff";
                if (im.createButtonImage(difName.c_str(), diffuse, texSize)) {
                  // opened window to set diffuse texture
                  const Path path(_window.openFileFromExplorer());
                  if (path.toString() != "") {
                    // SPtr<Texture> texture = tm.loadTexture(path);
                    // meshMat->setDiffuse(texture);
                  }
                }
                if (im.beginDragDropSource()) {
                  const String dragText = "Dragging " + difName;
                  im.createText(dragText.c_str());
                  const String textureID = diffuse->getID();
                  const char* data = textureID.c_str();
                  im.setDragDropPayload("RESOURCE_PAYLOAD", data, strlen(data) + 1);
                  im.endDragDropSource();
                }
                if (im.beginDragDropTarget()) {
                  const char* id = reinterpret_cast<const char*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
                  if (id) {
                    SPtr<Texture> texture = tm.loadTexture(id);
                    meshMat->setDiffuse(texture);
                  }
                  im.endDragDropTarget();
                }
                // hover tooltip.
                if (im.isItemHovered()) {
                  im.setTooltip("Diffuse Texture");
                }
                im.sameLine();
                im.colorEdit("Color Multiply", matProps.ColorMultiply);
              }
              else {
                if (im.createButton("##DiffuseButton")) {

                }
                if (im.beginDragDropTarget()) {
                  const char* id = reinterpret_cast<const char*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
                  if (id) {
                    SPtr<Texture> texture = tm.loadTexture(id);
                    meshMat->setDiffuse(texture);
                  }
                  im.endDragDropTarget();
                }
                if (im.isItemHovered()) {
                  im.setTooltip("Warning!!! Diffuse texture is null.");
                }
              }
              
              /***************************************************************/
              /*------------------------Normal button------------------------*/
              /***************************************************************/

              if (normal) {
                const String norName = normal->getNameString() + "norm";
                if (im.createButtonImage(norName.c_str(), normal, texSize)) {
                  // opened window to set normal texture
                  const Path path(_window.openFileFromExplorer());
                  if (path.toString() != "") {
                    // SPtr<Texture> texture = tm.loadTexture(path);
                    // meshMat->setNormal(texture);
                  }
                }
                if (im.beginDragDropSource()) {
                  const String dragText = "Dragging " + norName;
                  im.createText(dragText.c_str());
                  const String textureID = normal->getID();
                  const char* data = textureID.c_str();
                  im.setDragDropPayload("RESOURCE_PAYLOAD", data, strlen(data) + 1);
                  im.endDragDropSource();
                }
                if (im.beginDragDropTarget()) {
                  const char* id = reinterpret_cast<const char*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
                  if (id) {
                    SPtr<Texture> texture = tm.loadTexture(id);
                    meshMat->setNormal(texture);
                  }
                  im.endDragDropTarget();
                }
                // hover tooltip.
                if (im.isItemHovered()) {
                  im.setTooltip("Normal Texture");
                }
              }
              else {
                if (im.createButton("##NormalButton")) {

                }
                if (im.beginDragDropTarget()) {
                  const char* id = reinterpret_cast<const char*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
                  if (id) {
                    SPtr<Texture> texture = tm.loadTexture(id);
                    meshMat->setNormal(texture);
                  }
                  im.endDragDropTarget();
                }
                if (im.isItemHovered()) {
                  im.setTooltip("Warning!!! Normal texture is null.");
                }
              }
              
              /***************************************************************/
              /*----------------------oclussion button-----------------------*/
              /***************************************************************/

              if (oclussion) {
                const String occName = oclussion->getNameString() + "ao";
                if (im.createButtonImage(occName.c_str(), oclussion, texSize)) {
                  // opened window to set occlusion texture
                  const Path path(_window.openFileFromExplorer());
                  if (path.toString() != "") {

                    // SPtr<Texture> texture = tm.loadTexture(path);
                    // meshMat->setOcclusion(texture);
                  }
                }
                if (im.beginDragDropSource()) {
                  const String dragText = "Dragging " + occName;
                  im.createText(dragText.c_str());
                  const String textureID = oclussion->getID();
                  const char* data = textureID.c_str();
                  im.setDragDropPayload("RESOURCE_PAYLOAD", data, strlen(data) + 1);
                  im.endDragDropSource();
                }
                if (im.beginDragDropTarget()) {
                  const char* id = reinterpret_cast<const char*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
                  if (id) {
                    SPtr<Texture> texture = tm.loadTexture(id);
                    meshMat->setOclussion(texture);
                  }
                  im.endDragDropTarget();
                }
                // hover tooltip.
                if (im.isItemHovered()) {
                  im.setTooltip("Ambient Occlusion Texture");
                }
              }
              else {
                if (im.createButton("##OclussionButton")) {

                }
                if (im.beginDragDropTarget()) {
                  const char* id = reinterpret_cast<const char*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
                  if (id) {
                    SPtr<Texture> texture = tm.loadTexture(id);
                    meshMat->setOclussion(texture);
                  }
                  im.endDragDropTarget();
                }
                if (im.isItemHovered()) {
                  im.setTooltip("Warning!!! Oclussion texture is null.");
                }
              }
              
              /***************************************************************/
              /*-----------------------roughness button----------------------*/
              /***************************************************************/

              if (rough) {
                const String roughName = rough->getNameString() + "rough";
                if (im.createButtonImage(roughName.c_str(), rough, texSize)) {
                  // opened window to set rough texture
                  const Path path(_window.openFileFromExplorer());
                  if (path.toString() != "") {
                    // SPtr<Texture> texture = tm.loadTexture(path);
                    // meshMat->setRoughness(texture);
                  }
                }
                if (im.beginDragDropSource()) {
                  const String dragText = "Dragging " + roughName;
                  im.createText(dragText.c_str());
                  const String textureID = rough->getID();
                  const char* data = textureID.c_str();
                  im.setDragDropPayload("RESOURCE_PAYLOAD", data, strlen(data) + 1);
                  im.endDragDropSource();
                }
                if (im.beginDragDropTarget()) {
                  const char* id = reinterpret_cast<const char*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
                  if (id) {
                    SPtr<Texture> texture = tm.loadTexture(id);
                    meshMat->setRoughness(texture);
                  }
                  im.endDragDropTarget();
                }
                // hover tooltip.
                if (im.isItemHovered()) {
                  im.setTooltip("Roughness Texture");
                }
                im.sameLine();
                im.createDragF("Roughness Strength", matProps.roughnessMultiply, 0.01f, 0.0f, 1.0f);
              }
              else {
                if (im.createButton("##RoughnessButton")) {

                }
                if (im.beginDragDropTarget()) {
                  const char* id = reinterpret_cast<const char*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
                  if (id) {
                    SPtr<Texture> texture = tm.loadTexture(id);
                    meshMat->setRoughness(texture);
                  }
                  im.endDragDropTarget();
                }
                if (im.isItemHovered()) {
                  im.setTooltip("Warning!!! Roughness texture is null.");
                }
              }
              
              /***************************************************************/
              /*-----------------------metallic button-----------------------*/
              /***************************************************************/

              if (metallic) {
                const String metName = metallic->getNameString() + "metal";
                if (im.createButtonImage(metName.c_str(), metallic, texSize)) {
                  // opened window to set metallic texture
                  const Path path(_window.openFileFromExplorer());
                  if (path.toString() != "") {
                    // SPtr<Texture> texture = tm.loadTexture(path);
                    // meshMat->setMetallic(texture);
                  }
                }
                if (im.beginDragDropSource()) {
                  const String dragText = "Dragging " + metName;
                  im.createText(dragText.c_str());
                  const String textureID = metallic->getID();
                  const char* data = textureID.c_str();
                  im.setDragDropPayload("RESOURCE_PAYLOAD", data, strlen(data) + 1);
                  im.endDragDropSource();
                }
                if (im.beginDragDropTarget()) {
                  const char* id = reinterpret_cast<const char*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
                  if (id) {
                    SPtr<Texture> texture = tm.loadTexture(id);
                    meshMat->setMetallic(texture);
                  }
                  im.endDragDropTarget();
                }
                // hover tooltip.
                if (im.isItemHovered()) {
                  im.setTooltip("Metallic Texture");
                }
                im.sameLine();
                im.createDragF("Metallic Strength", matProps.metallicMultiply, 0.01f, 0.0f, 1.0f);
              }
              else {
                if (im.createButton("##MetallicButton")) {

                }
                if (im.beginDragDropTarget()) {
                  const char* id = reinterpret_cast<const char*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
                  if (id) {
                    SPtr<Texture> texture = tm.loadTexture(id);
                    meshMat->setMetallic(texture);
                  }
                  im.endDragDropTarget();
                }
                if (im.isItemHovered()) {
                  im.setTooltip("Warning!!! Metallic texture is null.");
                }
              }
              
              /***************************************************************/
              /*-----------------------emissive button-----------------------*/
              /***************************************************************/

              if (emissive) {
                const String emissName = emissive->getNameString() + "emissive";
                if (im.createButtonImage(emissName.c_str(), emissive, texSize)) {
                  // opened window to set metallic texture
                  const Path path(_window.openFileFromExplorer());
                  if (path.toString() != "") {
                    // SPtr<Texture> texture = tm.loadTexture(path);
                    // meshMat->setEmissive(texture);
                  }
                }
                if (im.beginDragDropSource()) {
                  const String dragText = "Dragging " + emissName;
                  im.createText(dragText.c_str());
                  const String textureID = emissive->getID();
                  const char* data = textureID.c_str();
                  im.setDragDropPayload("RESOURCE_PAYLOAD", data, strlen(data) + 1);
                  im.endDragDropSource();
                }
                if (im.beginDragDropTarget()) {
                  const char* id = reinterpret_cast<const char*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
                  if (id) {
                    SPtr<Texture> texture = tm.loadTexture(id);
                    meshMat->setEmissive(texture);
                  }
                  im.endDragDropTarget();
                }
                // hover tooltip.
                if (im.isItemHovered()) {
                  im.setTooltip("Emissive Texture");
                }
                im.sameLine();
                im.colorEdit("Emissive Color", matProps.EmissiveMultiply);
              }
              else {
                if (im.createButton("##EmissiveButton")) {

                }
                if (im.beginDragDropTarget()) {
                  const char* id = reinterpret_cast<const char*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
                  if (id) {
                    SPtr<Texture> texture = tm.loadTexture(id);
                    meshMat->setEmissive(texture);
                  }
                  im.endDragDropTarget();
                }
                if (im.isItemHovered()) {
                  im.setTooltip("Warning!!! Emissive texture is null.");
                }
              }
            }
            else {
              im.pushID(i);
              if (im.createButton("<Create new material>")) {
                mesh->material = GPUResourceMan.newMaterial();
              }
              im.popID();
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