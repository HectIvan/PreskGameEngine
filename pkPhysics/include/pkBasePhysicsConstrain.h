#pragma once
#include "pkPrerequisitesCore.h"
#include "pkRigidBody.h"

namespace pkEngineSDK
{

class BasePhysicsConstrain
{
 public:
  BasePhysicsConstrain() = default;
  virtual ~BasePhysicsConstrain() = default;

  /**
   * @brief 
   */
  void
  init(SPtr<RigidBody> _rb1, SPtr<RigidBody> _rb2); // with rigid bodies

  /**
   * @brief 
   * @param _dt
   */
  void
  preSolve(float _dt);

  /**
   * @brief 
   * @param _dt
   */
  void
  solve(float _dt);

  /**
   * @brief 
   * @param _dt
   */
  void
  postSolve(float _dt);

  /**
   * @brief 
   * @return 
   */
  float
  getError();

 public:
  SPtr<RigidBody> m_rb1;
  SPtr<RigidBody> m_rb2;

  Vector3 m_joint1; // relative position of the first rigid body
  Vector3 m_joint2; // relative position of the second rigid body
};
}
