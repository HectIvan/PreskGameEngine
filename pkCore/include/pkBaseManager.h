#pragma once

#include "pkModule.h"
#include "pkPrerequisitesCore.h"

namespace pkEngineSDK
{

class RigidBody;
class Shape;
class OBB;
class Sphere;
class Actor;
class CollisionInfo;

class PK_CORE_EXPORT BaseManager : public Module<BaseManager>
{
 public:
  BaseManager() = default;
  virtual ~BaseManager() = default;

  /**
   * @brief Update the manager on a fixed time.
   */
  virtual void
  fixedUpdate() = 0;

  /**
   * @brief Sort a vector of shapes by order from left to right in the X axis.
   * @param _shapes Vector of shapes.
   * @return New vector of organized shapes.
   */
  virtual Vector<Shape>
  sortByLeft(Vector<Shape>& _shapes) = 0;

  /**
   * @brief Sweep and prune algorithm for Broad Phase Collision Detection.
   * @param _colliders Vectors of shapes to use
   */
  virtual void
  BPCDSweepAndPrune(Vector<Shape>& _colliders) = 0;

  /**
   * @brief Calculates a collision using Gilbert-Johnson-Keerthi (GJK).
   * @param _shape1 First shape of the collision.
   * @param _shape2 Second shape of the collision.
   * @param _attempts How many attempts will be made for the detection.
   * @return bool for when there was a collision or not.
   */
  virtual bool
  GJK(Shape& _shape1, Shape& _shape2, uint32 _attempts) = 0;

  /**
   * @brief Configuration space obstacle.
   * @param _shape1 First shape to use.
   * @param _shape2 Second shape to use.
   * @param _direction Direction to use.
   * @return The support point using the algorithm.
   */
  virtual Vector3
  supportCSO(Shape& _shape1, Shape& _shape2, Vector3& _direction) = 0;

  /**
   * @brief Update the dierction by using the simplex provided.
   * @param _simplex Simplex to use.
   * @param _direction Direction to use and change.
   */
  virtual bool
  updateSimplexNDirection(Shape& _simplex, Vector3& _direction) = 0;

  /**
   * @brief Get if the origin is in front of the plane.
   * @param _vertex Points of the plane (from last inserted [0] to first inserted[2].
   * @param _direction
   */
  virtual bool
  originInFrontOfPlane(Vector3 _vertex[], Vector3& _direction) = 0;

  /**
   * @brief Get inertia tensor from OBB object.
   * @param _obb Object Bounding Box to check.
   * @param _mass Mass of the box.
   */
  virtual Matrix3
  getInertiaTensorOBB(OBB _obb, float _mass) = 0;

  /**
   * @brief Get inertia tensor from Sphere.
   * @param _sphere Sphere to check.
   * @param _mass Mass of the sphere
   */
  virtual Matrix3
  getInertiaTensorSphere(Sphere& _sphere, float _mass) = 0;

  /**
   * @brief Get the effective mass between 2 rigid bodies.
   * @param _normalHit Normal vector of the collision.
   * @param _rb1 First rigid body.
   * @param _rb2 Second rigid body.
   * @param _contactPoint1 First contact point.
   * @param _contactPoint2 Second contact point.
   * @return Effective mass.
   */
  virtual float
  getEffectiveMass(Vector3& _normalHit,
                   RigidBody& _rb1,
                   RigidBody& _rb2,
                   Vector3 _contactPoint1,
                   Vector3 _contactPoint2) = 0;

  /**
   * @brief Get the average elasticity from 2 rigid bodies.
   * @param _rb1 First rigid body.
   * @param _rb2 Second rigid body.
   * @return The average elasticity.
   */
  virtual float
  getElasticity(const RigidBody& _rb1, const RigidBody& _rb2) = 0;

  /**
   * @brief Get the average elasticity from 2 rigid bodies.
   * @param _rb1 First rigid body.
   * @param _rb2 Second rigid body.
   * @return The average elasticity.
   */
  virtual float
  getFriction(const RigidBody& _rb1, const RigidBody& _rb2) = 0;

  /**
   * @brief Detect if there was a collision.
   * @param _rb1 First rigid body to check for.
   * @param _rb2 Second rigid body to check for.
   * @param _info Info of the collision.
   */
  virtual void
  resolveCollision(RigidBody _rb1, RigidBody _rb2, CollisionInfo _info) = 0;

  /**
   * @brief Get the effective mass between 2 rigid bodies.
   * @param _rb1 First rigid body.
   * @param _rb2 Second rigid body.
   * @return The effective mass.
   */
  virtual float
  getEffectiveMassP(RigidBody& _rb1, RigidBody& _rb2) = 0;

  /**
   * @brief Simulate an actors physics.
   * @param _pActor What actor to simulate.
   * @param _deltatime Delta time to use for the simulation.
   */
  virtual void
  simulateActor(SPtr<Actor>& _pActor, float _deltatime) = 0;
};

PK_CORE_EXPORT BaseManager&
g_BaseManager();
}
