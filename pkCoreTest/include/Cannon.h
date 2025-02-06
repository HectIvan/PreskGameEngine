#pragma once
#include "pkActor.h"
#include "pkPrerequisitesCore.h"
#include "pkMatrix4.h"

using pkEngineSDK::Matrix4;
using pkEngineSDK::Actor;
using pkEngineSDK::SPtr;

class Cannon
{
 public:
  Cannon() = default;
  virtual ~Cannon() = default;

  void
  start()
  {
    m_actor->m_transform = Matrix4::IDENTITY;
  }

  SPtr<Actor> m_actor;
};