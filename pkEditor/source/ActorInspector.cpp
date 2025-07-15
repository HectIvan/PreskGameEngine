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
#include "pkPlatformMath.h";
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
  // change the position
  Vector3 newTranslation = _pActor->getPosition3();
  im.createDrag3("Position", newTranslation);
  // change the rotation
  _pActor->setPosition(newTranslation);
  Vector3 newRotation = Vector3(0.0f);
  im.createDrag3("Rotation",newRotation);
  //            _pActor->setRotation(newRotation);
  // change the scale
  Vector3 newScale = _pActor->getScale();
  im.createDrag3("Scale", newScale);
  _pActor->setScale(newScale);
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
    // initialize the camera with the new parameters
    if (isChanged) {
      cDesc.eye = cam->m_eye.xyz();
      cDesc.at = cam->m_at.xyz();
      cDesc.up = Vector3::UP;
      cam->init(cDesc);
    }
    im.createText("Projection");
    break;
  }
  case kLight:
  {
    SPtr<Light> light = reinterpret_pointer_cast<Light>(_pComponent);
    im.createText("Light");
    im.colorEdit("Color", light->m_lightColor);
    im.createDrag3("Direction", light->m_lightDir, 0.1f);
    if (light->m_lightDir.magnitude() > Math::SMALL_NUMBER) {
      light->m_lightDir.normalize();
    }
    im.createDrag3("Position", light->m_lightPos, 0.1f);

    im.createDragF("Spot Exponent", light->m_spotExponent, 0.1f, 0.0f);
    im.createDragF("Spot Cutoff", light->m_spotCutoff, 0.01f);
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