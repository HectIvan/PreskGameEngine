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
#include "pkLight.h";
#include "pkUInterface.h"
#include "pkVector3.h"

using pkEngineSDK::Camera;
using pkEngineSDK::COMPONENT_TYPE::kCamera;
using pkEngineSDK::COMPONENT_TYPE::kLight;
using pkEngineSDK::COMPONENT_TYPE::kMaterial;
using pkEngineSDK::COMPONENT_TYPE::kModel;
using pkEngineSDK::COMPONENT_TYPE::kUnknown;
using pkEngineSDK::g_uInterface;
using pkEngineSDK::Light;
using pkEngineSDK::reinterpret_pointer_cast;
using pkEngineSDK::uint32;
using pkEngineSDK::UInterface;
using pkEngineSDK::Vector3;

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
    im.createSliderVector3("Eye", cam->m_eye.xyz(), -9999999.9f, 9999999.9f);
    break;
  }
  case kLight:
  {
    SPtr<Light> light = reinterpret_pointer_cast<Light>(_pComponent);
    im.createText("Light");
    im.createSliderVector3("Color", light->LightColor, 0.0f, 255.0f);
    im.createSliderVector3("Direction", light->LightDir, -1.0f, 1.0f);
    im.createSliderVector3("Position", light->LightPos, -9999999.9f, 9999999.9f);
    break;
  }
  case kMaterial:
  {
    im.createText("Material");
    break;
  }
  case kModel:
  {
    im.createText("Model");
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