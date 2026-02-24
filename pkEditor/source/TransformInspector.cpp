#include "pkUInterface.h"
#include "TransformInspector.h"
#include "pkPlatformMath.h"

using pkEngineSDK::g_uInterface;
using pkEngineSDK::Math;
using pkEngineSDK::UInterface;
using pkEngineSDK::Vector3;

void
TransformInspector::inspect(SPtr<Actor>& _pActor)
{
  // get the user interface manager
  UInterface& im = g_uInterface();
  // change the position
  Vector3 newTranslation = _pActor->m_position;
  im.createText("Position");
  im.sameLine();
  if (im.createDrag3("##Position", newTranslation)) {
    _pActor->setPosition(newTranslation);
  }
  // change the rotation
  Vector3 newRotation = _pActor->m_rotation.toEuler() * Math::RAD2DEG;
  im.createText("Rotation");
  im.sameLine();
  if (im.createDrag3("##Rotation", newRotation, 1.0f)) {
    _pActor->setRotation(newRotation * Math::DEG2RAD);
  }
  // change the scale
  Vector3 newScale = _pActor->m_scale;
  im.createText("Scale   ");
  im.sameLine();
  if (im.createDrag3("##Scale", newScale)) {
    _pActor->setScale(newScale);
  }

  im.createText("");

  Vector3 right = _pActor->m_right;
  im.createText("Right   ");
  im.sameLine();
  im.createDrag3("##Right", right);

  Vector3 up = _pActor->m_up;
  im.createText("Up      ");
  im.sameLine();
  im.createDrag3("##Up", up);

  Vector3 forward = _pActor->m_forward;
  im.createText("Forward ");
  im.sameLine();
  im.createDrag3("##Forward", forward);
}