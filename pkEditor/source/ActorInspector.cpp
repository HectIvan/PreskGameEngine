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
#include "pkPath.h"
#include "pkPlatformMath.h"
#include "pkPrerequisitesCore.h"
#include "pkUInterface.h"
#include "pkVector3.h"
#include "pkVector4.h"
#include "pkLogger.h"
#include "pkRigidBody.h"

using pkEngineSDK::Camera;
using pkEngineSDK::CameraDesc;
using pkEngineSDK::COMPONENT_TYPE::kCamera;
using pkEngineSDK::COMPONENT_TYPE::kLight;
using pkEngineSDK::COMPONENT_TYPE::kMaterial;
using pkEngineSDK::COMPONENT_TYPE::kModel;
using pkEngineSDK::COMPONENT_TYPE::kRigidBody;
using pkEngineSDK::COMPONENT_TYPE::kUnknown;
using pkEngineSDK::g_uInterface;
using pkEngineSDK::Light;
using pkEngineSDK::Material;
using pkEngineSDK::Math;
using pkEngineSDK::Matrix4;
using pkEngineSDK::Mesh;
using pkEngineSDK::Model;
using pkEngineSDK::reinterpret_pointer_cast;
using pkEngineSDK::RigidBody;
using pkEngineSDK::String;
using pkEngineSDK::uint32;
using pkEngineSDK::UInterface;
using pkEngineSDK::Vector3;
using pkEngineSDK::Vector4;

using pkEngineSDK::g_Logger;

ActorInspector::ActorInspector(SPtr<Actor> _pActor)
{
  Inspect(_pActor);
}

void
ActorInspector::Inspect(SPtr<Actor>& _pActor)
{
  // get the user interface manager
  UInterface& im = g_uInterface().instance();
  String name = _pActor->getName();
  im.createText("Name:   ");
  im.sameLine();
  if (im.createInputText("##Name", &name)) {
    _pActor->setName(name);
  }
  // activity checkbox
  im.sameLine();
  im.createCheckBox("|", _pActor->isActive());
  // change the position
  Vector3 newTranslation = _pActor->m_position;
  im.createText("Position");
  im.sameLine();
  im.createDrag3("##Position", newTranslation);
  _pActor->setPosition(newTranslation);
  // change the rotation
  Vector3 newRotation = _pActor->m_rotation;
  im.createText("Rotation");
  im.sameLine();
  im.createDrag3("##Rotation",newRotation, 1.0f);
  _pActor->setRotation(newRotation);
  // change the scale
  Vector3 newScale = _pActor->m_scale;
  im.createText("Scale   ");
  im.sameLine();
  im.createDrag3("##Scale", newScale);
  _pActor->setScale(newScale);
}

void
ActorInspector::createComponentWindow(SPtr<Component>& _pComponent, Matrix4 _transform)
{
  // get the user interface manager
  UInterface& im = g_uInterface().instance();
  // Component activity
  im.createCheckBox("Active ", _pComponent->isActive());
  // for each type of component
  switch (_pComponent->getType())
  {
  case kCamera:
  {
    bool isChanged = false;
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
    // bool selected = false;
    // im.beginCombo("Projections", "Profile: ");
    // im.selectable("Perspective", &selected);
    // im.selectable("Orthographic", &selected);
    // im.endCombo();

    // initialize the camera with the new parameters
    if (isChanged) {
      cDesc.eye = cam->m_eye.xyz();
      cDesc.at = cam->m_at.xyz();
      cDesc.up = Vector3::UP;
      cam->init(cDesc);
    }
    break;
  }
  case kLight:
  {
    SPtr<Light> light = reinterpret_pointer_cast<Light>(_pComponent);
    im.createText("Light");
    // Light color
    im.createText("Color             ");
    im.sameLine();
    im.colorEdit("##Color", light->m_color);
    // Light direction
    im.createText("Direction         ");
    im.sameLine();
    Vector4 dir4 = _transform * Vector4(light->m_direction, 0.0f);
    Vector3 dir = dir4.xyz().normalized();
    im.createDrag3("##Direction", dir, 0.0f);
    // spot exponent
    im.createText("Spot Exponent     ");
    im.sameLine();
    im.createDragF("##Spot Exponent", light->m_spotExponent, 0.1f, 0.0f);
    // Spot cutoff
    im.createText("Spot Cutoff       ");
    im.sameLine();
    im.createDragF("##Spot Cutoff", light->m_spotCutoff, 0.01f, 0.0f, 180.0f);
    // shadow intensity
    im.createText("Shadow Intensity  ");
    im.sameLine();
    im.createDragF("##Shadow Intensity", light->m_shadowIntensity, 0.05f, 0.0f, 1.0f);
    // Specular Intensity
    im.createText("Specular Intensity");
    im.sameLine();
    im.createDragF("##Specular Intensity", light->m_specIntensity, 0.05f, 0.0f, 1.0f);
    break;
  }
  case kMaterial:
  {
    im.createText("Material");
    break;
  }
  case kModel:
  {
    SPtr<Model> model = reinterpret_pointer_cast<Model>(_pComponent);
    im.beginChild("Meshes:");
    im.createText("Meshes: ");
    for (uint32 i = 0; i < model->getMeshes().size(); ++i) {
      // get mesh
      SPtr<Mesh> mesh = model->getMeshes()[i];
      String name = "  " + mesh->getName();
      SPtr<Material> material = mesh->material;
      im.createText(name.c_str());
      // get material
      // SPtr<Material> meshMat = mesh->material;
      im.createButton("Diffuse");
      im.sameLine();
      im.createButton("Normal");
      im.sameLine();
      im.createButton("AO");
      im.sameLine();
      im.createButton("Height");
      im.sameLine();
      im.createButton("Metallic");
    }
    im.endChild();
    break;
  }
  case kRigidBody:
  {
    SPtr<RigidBody> rb = reinterpret_pointer_cast<RigidBody>(_pComponent);
    im.beginChild("Rigid Body");
    im.createText("Gravity           ");
    im.sameLine();
    im.createDragF("##Gravity", rb->m_gravity);
    im.createText("Drag              ");
    im.sameLine();
    im.createDragF("##Drag", rb->m_drag);
    if (rb->m_physMat) {
      im.createText("Physics Material");
      // friction
      im.createText("Friction          ");
      im.sameLine();
      float friction = rb->getFrictionCoefficient();
      im.createDragF("##Friction", friction, 0.1f, 0.0f, 1.0f);
      rb->setFrictionCoef(friction);
      // elasticity
      im.createText("Elasticity        ");
      im.sameLine();
      float elasticity = rb->getElasticity();
      im.createDragF("##Elasticity", elasticity, 0.1f, 0.0f, 1.0f);
      rb->setElasticity(elasticity);
    }
    im.endChild();
    break;
  }
  case kUnknown:
  {
    im.createText("???");
    break;
  }
  default:
    break;
  }
}