/*****************************************************************************/
/**
 * @file    pkScene.h
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    2025/01/29
 * @brief   Engine Scene.
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

  /**
   * @brief Get the last inserted actor of the scene.
   * @return The last actor.
   */
  FORCEINLINE SPtr<Actor>
  getLastActor() { return m_actors[m_actors.size() - 1]; }

  /**
   * @brief Get the actor in the index of the scene.
   * @param _index Actor position in the list.
   * @return The actor in the index.
   */
  SPtr<Actor>
  getActor(uint32 _index) { return m_actors[_index]; }

 public:
  Vector<SPtr<Actor>> m_actors;
};
}