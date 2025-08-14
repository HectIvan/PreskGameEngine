/*************************************************************************/
/**
 * @file    pkPhysicsManager.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    05/06/2025
 * @brief   Physics manager for the physics library.
 *
 * @bug     no known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkBaseManager.h"
#include "pkPhysicsDefines.h"

namespace pkEngineSDK
{

class PhysicsManager : public BaseManager
{
 public:
  PhysicsManager() = default;
  virtual ~PhysicsManager() = default;

  /**
   * @brief Update the manager on a fixed time.
   */
  void
  fixedUpdate() override;

  /**
   * @brief Sort a vector of shapes by order from left to right in the X axis.
   * @param _shapes Vector of shapes.
   * @return New vector of organized shapes.
   */
  Vector<Shape>
  sortByLeft(Vector<Shape>& _shapes) override;

  /**
   * @brief Sweep and prune algorithm for Broad Phase Collision Detection.
   * @param _colliders Vectors of shapes to use
   */
  void
  BPCDSweepAndPrune(Vector<Shape>& _colliders) override;

  /**
   * @brief Calculates a collision using Gilbert-Johnson-Keerthi (GJK).
   * @param _shape1 First shape of the collision.
   * @param _shape2 Second shape of the collision.
   * @param _attempts How many attempts will be made for the detection.
   * @return bool for when there was a collision or not.
   */
  bool
  GJK(Shape& _shape1, Shape& _shape2, uint32 _attempts) override;

  /**
   * @brief Configuration space obstacle.
   * @param _shape1 First shape to use.
   * @param _shape2 Second shape to use.
   * @param _direction Direction to use.
   * @return The support point using the algorithm.
   */
  Vector3
  supportCSO(Shape& _shape1, Shape& _shape2, Vector3& _direction) override;

  /**
   * @brief Update the dierction by using the simplex provided.
   * @param _simplex Simplex to use.
   * @param _direction Direction to use and change.
   */
  bool
  updateSimplexNDirection(Shape& _simplex, Vector3& _direction) override;

  /**
   * @brief Get if the origin is in front of the plane.
   * @param _vertex Points of the plane (from last inserted [0] to first inserted[2].
   * @param _direction
   */
  bool
  originInFrontOfPlane(Vector3 _vertex[], Vector3& _direction) override;

  /**
   * @brief Get inertia tensor from OBB object.
   * @param _obb Object Bounding Box to check.
   * @param _mass Mass of the box.
   */
  Matrix3
  getInertiaTensorOBB(OBB _obb, float _mass) override;

  /**
   * @brief Get inertia tensor from Sphere.
   * @param _sphere Sphere to check.
   * @param _mass Mass of the sphere
   */
  Matrix3
  getInertiaTensorSphere(Sphere& _sphere, float _mass) override;

  /**
   * @brief Get the effective mass between 2 rigid bodies.
   * @param _normalHit Normal vector of the collision.
   * @param _rb1 First rigid body.
   * @param _rb2 Second rigid body.
   * @param _contactPoint1 First contact point.
   * @param _contactPoint2 Second contact point.
   * @return Effective mass.
   */
  float
  getEffectiveMass(Vector3& _normalHit,
                   RigidBody& _rb1,
                   RigidBody& _rb2,
                   Vector3 _contactPoint1,
                   Vector3 _contactPoint2) override;

  /**
   * @brief Get the average elasticity from 2 rigid bodies.
   * @param _rb1 First rigid body.
   * @param _rb2 Second rigid body.
   * @return The average elasticity.
   */
  float
  getElasticity(const RigidBody& _rb1, const RigidBody& _rb2) override;

  /**
   * @brief Get the average elasticity from 2 rigid bodies.
   * @param _rb1 First rigid body.
   * @param _rb2 Second rigid body.
   * @return The average elasticity.
   */
  float
  getFriction(const RigidBody& _rb1, const RigidBody& _rb2) override;

  /**
   * @brief Detect if there was a collision.
   * @param _rb1 First rigid body to check for.
   * @param _rb2 Second rigid body to check for.
   * @param _info Info of the collision.
   */
  void
  resolveCollision(RigidBody _rb1, RigidBody _rb2, CollisionInfo _info) override;

  /**
   * @brief Get the effective mass between 2 rigid bodies.
   * @param _rb1 First rigid body.
   * @param _rb2 Second rigid body.
   * @return The effective mass.
   */
  float
  getEffectiveMassP(RigidBody& _rb1, RigidBody& _rb2) override;

  /**
   * @brief Simulate an actors physics.
   * @param _pActor What actor to simulate.
   * @param _deltatime Delta time to use for the simulation.
   */
  void
  simulateActor(SPtr<Actor>& _pActor, float _deltatime) override;

  /**
   * @brief Simulate all actors physics.
   * @param _pActor What actors to simulate.
   * @param _deltatime Delta time to use for the simulation.
   */
  void
  simulateActors(Vector<SPtr<Actor>> _pActor, float _deltatime) override;
};
}
