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
  init(); // with physics objects

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
};
}
