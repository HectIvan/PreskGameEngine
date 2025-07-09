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
#include "pkModel.h"
#include "pkUInterface.h"
#include "pkVector3.h"
#include "pkVector4.h"
#include "pkPrerequisitesCore.h"

using pkEngineSDK::Camera;
using pkEngineSDK::COMPONENT_TYPE::kCamera;
using pkEngineSDK::COMPONENT_TYPE::kLight;
using pkEngineSDK::COMPONENT_TYPE::kMaterial;
using pkEngineSDK::COMPONENT_TYPE::kModel;
using pkEngineSDK::COMPONENT_TYPE::kUnknown;
using pkEngineSDK::g_uInterface;
using pkEngineSDK::Light;
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
  // generate the transform matrix in interface
  Vector3 newTranslation = im.createSliderVector3("Position",
                                                  _pActor->getPosition3(),
                                                  -2147483648.0f,
                                                  2147483647.0f);
  _pActor->setPosition(newTranslation);
  Vector3 newRotation = im.createSliderVector3("Rotation",
                                               Vector3(0),
                                               -2147483648.0f,
                                               2147483647.0f);
  // pistol->m_transform.setRotation(newRotation);
  Vector3 newScale = im.createSliderVector3("Scale",
                                            _pActor->getScale(),
                                            -2147483648.0f,
                                            2147483647.0f);
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
    SPtr<Camera> cam = reinterpret_pointer_cast<Camera>(_pComponent);
    im.createText("Camera");
    im.createInputVector4Clamp("Eye", cam->m_eye, -9999999.9f, 9999999.9f);
    break;
  }
  case kLight:
  {
    SPtr<Light> light = reinterpret_pointer_cast<Light>(_pComponent);
    im.createText("Light");
    im.createInputVector3Clamp("Color", light->LightColor, 0.0f, 1.0f);
    im.createInputVector3Clamp("Direction", light->LightDir, -1.0f, 1.0f);
    im.createInputVector3Clamp("Position", light->LightPos, -9999999.9f, 9999999.9f);
    im.createInputF("Spot Exponent", light->SpotExponent, 1.0f, 1.0f);
    im.createInputFClamp("Shadow Intensity", light->shadowIntensity, 0.0f, 1.0f, 0.05f, 0.1f);
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
    im.createText(model->getName());
    for (uint32 i = 0; i < model->getMeshes().size(); ++i) {
      String name = "  " + model->getMeshes()[i]->getName();
      im.createText(name.c_str());
    }
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