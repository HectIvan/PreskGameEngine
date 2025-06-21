/*************************************************************************/
/**
 * @file    pkPhysicsManager.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    05/06/2025
 * @brief   Physics manager for the physics library.
 *
 * @bug    no knon bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkModule.h"
#include "pkShape.h"

namespace pkEngineSDK
{

class PhysicsManager : public Module<PhysicsManager>
{
 public:
  PhysicsManager() = default;
  virtual ~PhysicsManager() = default;

  /**
   * @brief Update the manager on a fixed time.
   */
  void
  fixedUpdate();

  /**
   * @brief Sort a vector of shapes by order from left to right in the X axis.
   * @param _shapes Vector of shapes.
   * @return New vector of organized shapes.
   */
  Vector<Shape>
  sortByLeft(Vector<Shape>& _shapes);

  /**
   * @brief Sweep and prune algorithm for Broad Phase Collision Detection.
   * @param _colliders Vectors of shapes to use
   */
  void
  BPCDSweepAndPrune(Vector<Shape>& _colliders);

  /**
   * @brief Calculates a collision using Gilbert-Johnson-Keerthi (GJK).
   * @param _shape1 First shape of the collision.
   * @param _shape2 Second shape of the collision.
   * @param _attempts How many attempts will be made for the detection.
   * @return bool for when there was a collision or not.
   */
  bool
  GJK(Shape& _shape1, Shape& _shape2, uint32 _attempts);

  /**
   * @brief Configuration space obstacle.
   * @param _shape1 First shape to use.
   * @param _shape2 Second shape to use.
   * @param _direction Direction to use.
   * @return The support point using the algorithm.
   */
  Vector3
  supportCSO(Shape& _shape1, Shape& _shape2, Vector3& _direction);

  /**
   * @brief Update the dierction by using the simplex provided.
   * @param _simplex Simplex to use.
   * @param _direction Direction to use and change.
   */
  bool
  updateSimplexNDirection(Shape& _simplex, Vector3& _direction);

  /**
   * @brief Get if the origin is in front of the plane.
   * @param _vertex Points of the plane (from last inserted [0] to first inserted[2].
   * @param _direction
   */
  bool
  originInFrontOfPlane(Vector3 _vertex[], Vector3& _direction);
};
PhysicsManager&
g_physicsManager()
{
  return PhysicsManager::instance();
}
}
