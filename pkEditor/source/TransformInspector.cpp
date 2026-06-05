#include "pkUInterface.h"
#include "TransformInspector.h"
#include "pkPlatformMath.h"
#include "pkLogger.h"

using pkEngineSDK::g_uInterface;
using pkEngineSDK::Math;
using pkEngineSDK::UInterface;
using pkEngineSDK::Vector3;
using pkEngineSDK::g_Logger;

void
TransformInspector::inspect(SPtr<Actor>& _pActor)
{
  // get the user interface manager
  UInterface& im = g_uInterface();
  // change the position
  Vector3 newTranslation = _pActor->m_position;
  im.createText("Position");
  im.sameLine();
  if (im.createDrag("##Position", newTranslation)) {
    _pActor->setPosition(newTranslation);
  }
  // change the rotation
#if PK_ROT == PK_ROT_QUATERNION
  Vector3 newRotation = _pActor->m_rotation.toEuler() * Math::RAD2DEG;
#endif

#if PK_ROT == PK_ROT_EULER
  Vector3 newRotation = _pActor->m_rotation * Math::RAD2DEG;
#endif
  im.createText("Rotation");
  im.sameLine();
  if (im.createDrag("##Rotation", newRotation, 1.0f)) {
    _pActor->setRotation(newRotation * Math::DEG2RAD);
  }
  // change the scale
  Vector3 newScale = _pActor->m_scale;
  im.createText("Scale   ");
  im.sameLine();
  if (im.createDrag("##Scale", newScale)) {
    _pActor->setScale(newScale);
  }

  im.createText("------------------Directions------------------");

  Vector3 right = _pActor->m_right;
  im.createText("Right   ");
  im.sameLine();
  im.createDrag("##Right", right);

  Vector3 up = _pActor->m_up;
  im.createText("Up      ");
  im.sameLine();
  im.createDrag("##Up", up);

  Vector3 forward = _pActor->m_forward;
  im.createText("Forward ");
  im.sameLine();
  im.createDrag("##Forward", forward);
}