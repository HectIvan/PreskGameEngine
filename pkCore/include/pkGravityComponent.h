/************************************************************************/
/**
* @pkGravityComponent pkGravityComponent.h
* @Hector Ivan Muñoz Ceballos
* @date 15/03/2025
* @Gravity component for the engine, this component will bring the object down
* with a specified force and in a direction, this force can be changed manually.
*
* @bug No current function.
*
* @HectIvan 15/03/20245
* File Creation.
*/
/************************************************************************/
#pragma once

#include "pkComponent.h"

namespace pkEngineSDK
{

class Actor;
class Transform;

class GravityComponent : public Component
{
 public:
  GravityComponent() = default;
  virtual ~GravityComponent() = default;

  /**
   * @brief Update the component.
   * @param _owner Actor that owns the component.
   */
  void
  update(Actor& _owner) override;

  /**
   * @brief Set wether the component will affect the Actor
   */
  void
  isActive(bool _active) { m_active = _active; }

  /**
   * @brief Apply the gravity to the actor.
   * @param _pActor Actor to which the gravity will be applied.
   * @param _deltaTime Time between frames.
   */
  void
  applyGravity(SPtr<Actor> _pActor, float _delta);

  /**
   * @brief Apply the gravity to the transform matrix.
   * @param _transform Transform to which the gravity will be applied.
   * @param _deltaTime Time between frames.
   */
  void
  applyGravity(Matrix4& _transform, float _deltaTime);

  bool m_active;
  float m_force;
  Vector3 m_gravityDirection;
};
}
