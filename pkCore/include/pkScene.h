/*****************************************************************************/
/**
 * @file    pkScriptManager.h
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    2025/01/29
 * @brief
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkActor.h"
#include "pkPrerequisitesCore.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT Scene
{
 public:
  Scene() = default;
  virtual ~Scene() = default;

  /**
   * @brief Insert a new actor into the scene or as
   * a child of another actor.
   * @param _tranform Transform of the actor.
   * @param _pParent If the actor will be the child of another actor.
   */
  void
  instantiate(Matrix4 _transform = Matrix4::IDENTITY,
              SPtr<Actor> _pParent = nullptr);

 public:
  Vector<SPtr<Actor>> m_actors;
};
}