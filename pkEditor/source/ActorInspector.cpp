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

using pkEngineSDK::Camera;
using pkEngineSDK::CameraDesc;
using pkEngineSDK::COMPONENT_TYPE::kCamera;
using pkEngineSDK::COMPONENT_TYPE::kLight;
using pkEngineSDK::COMPONENT_TYPE::kMaterial;
using pkEngineSDK::COMPONENT_TYPE::kModel;
using pkEngineSDK::COMPONENT_TYPE::kUnknown;
using pkEngineSDK::g_uInterface;
using pkEngineSDK::Light;
using pkEngineSDK::Material;
using pkEngineSDK::Math;
using pkEngineSDK::Matrix4;
using pkEngineSDK::Mesh;
using pkEngineSDK::Model;
using pkEngineSDK::reinterpret_pointer_cast;
using pkEngineSDK::String;
using pkEngineSDK::uint32;
using pkEngineSDK::UInterface;
using pkEngineSDK::Vector3;
using pkEngineSDK::Vector4;

ActorInspector::ActorInspector(SPtr<Actor> _pActor)
{
  Inspect(_pActor);
}

void
ActorInspector::Inspect(SPtr<Actor>& _pActor)
{
  // get the user interface manager
  UInterface& im = g_uInterface().instance();
  Matrix4 transform = Matrix4::IDENTITY;
  // change the position
  Vector3 newTranslation = _pActor->m_position;
  im.createDrag3("Position", newTranslation);
  transform.setTranslation(newTranslation);
  // change the rotation
  Vector3 newRotation = _pActor->m_rotation;
  im.createDrag3("Rotation",newRotation, 1.0f);
  newRotation *= Math::DEG2RAD;
  transform.setRotation(Matrix4::rotation(newRotation));
  // change the scale
  Vector3 newScale = _pActor->m_scale;
  im.createDrag3("Scale", newScale);
  transform.setScale(newScale);

  _pActor->m_rotation = newRotation;
  _pActor->m_position = newTranslation;
  _pActor->m_scale = newScale;

  _pActor->m_transform = transform;
}

void
ActorInspector::createComponentWindow(SPtr<Component>& _pComponent)
{
  // get the user interface manager
  UInterface& im = g_uInterface().instance();
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
    if (im.createDragF("Half FOV", cDesc.halfFOV, 1.0f)) {
      isChanged = true;
    }
    if (im.createDragF("Near", cDesc.nearZ, 1.0f)) {
      isChanged = true;
    }
    if (im.createDragF("Far", cDesc.farZ, 1.0f)) {
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
    im.colorEdit("Color", light->m_color);
    im.createDrag3("Direction", light->m_direction, 0.1f);
    if (light->m_direction.magnitude() > Math::SMALL_NUMBER) {
      light->m_direction.normalize();
    }
    im.createDrag3("Position", light->m_position, 0.1f);

    im.createDragF("Spot Exponent", light->m_spotExponent, 0.1f, 0.0f);
    im.createDragF("Spot Cutoff", light->m_spotCutoff, 0.01f, 0.0f, 1.0f);
    im.createDragF("Shadow Intensity", light->m_shadowIntensity, 0.05f, 0.0f, 1.0f);
    im.createDragF("Specular Intensity", light->m_specIntensity, 0.05f, 0.0f, 1.0f);
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
    for (uint32 i = 0; i < model->getMeshes().size(); ++i) {
      // get mesh
      SPtr<Mesh> mesh = model->getMeshes()[i];
      String name = "  " + mesh->getName();
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
  case kUnknown:
  {
    im.createText("???");
    break;
  }
  default:
    break;
  }
}