/************************************************************************/
/**
* @pkPlatformMath pkPlatformMath.h
* @Hector Ivan Muñoz Ceballos
* @date 06/09/2024
* @Platform Math file for the Presk Game Engine.
*
* This file will contain the math used for the engine
* 
* @bug No bug known.
*
* @HectIvan 09/09/2024
* Added comments and missing functions
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkCollisionInfo.h"
#include "pkSphere.h"
#include "pkCube.h"
#include "pkCapsule.h"
#include "pkPlane.h"
#include "pkOBB.h"
#include "pkAABB.h"

namespace pkEngineSDK 
{

class PK_UTILITY_EXPORT PlatformMath
{
 public:
  PlatformMath() = default;
  ~PlatformMath() = default;

  /**
   * @brief Get the absolute value of _x.
   * @param _x Number to calculate the absolute value.
   * @return Absolute value of the parameter.
   */
  static float
  abs(const float _x);

  /**
   * @brief Get the power of _x.
   * @param _x Number to calculate the power.
   * @param _y Number of times _x will be multiplied by itself.
   * @return Result of _x elevated by the power of _y.
   */
  static float
  pow(const float _x, const float _y);

  /**
   * @brief Get Euler elevated by _x.
   * @param _x Number to elevate Euler.
   * @return Result of Euler elevated by _x.
   */
  static float
  exp(const float _x);

  /**
   * @brief Get the square root of _x.
   * @param _x Number to calculate the square root.
   * @return Square root of _x.
   */
  static float
  sqrt(const float _x);

  /**
   * @brief Get the lowest of 2 parameters.
   * @param _x First number to compare.
   * @param _y Second number to compare.
   * @return Whichever value is the smallest.
   */
  static float
  min(const float _x, const float _y);

  /**
   * @brief Get the highest of 2 parameters.
   * @param _x First number to compare.
   * @param _y Second number to compare.
   * @return Whichever value is the largest.
   */
  static float
  max(const float _x, const float _y);

  /**
   * @brief Get the sin of _x.
   * @param _x Value to calculate sin.
   * @return sin of _x.
   */
  static float
  sin(const float _x);

  /**
   * @brief Get the asin of _x.
   * @param _x Value to calculate asin.
   * @return asin of _x.
   */
  static float
  asin(const float _x);

  /**
   * @brief Get the cos of _x.
   * @param _x Value to calculate cos.
   * @return cos of _x.
   */
  static float
  cos(const float _x);

  /**
   * @brief Get the acos of _x.
   * @param _x Value to calculate acos.
   * @return acos of _x.
   */
  static float
  acos(const float _x);

  /**
   * @brief Get the tan of _x.
   * @param _x Value to calculate tan.
   * @return tan of _x.
   */
  static float
  tan(const float _x);

  /**
   * @brief Get the atan of _x.
   * @param _x Value to calculate atan.
   * @return atan of _x.
   */
  static float
  atan(const float _x);

  /**
   * @brief clamps a float between 2 functions.
   * @param _t Float to clamp.
   * @param _x Minimum clamping value.
   * @param _y Maximum clamping value.
   * @return Clamped float.
   */
  static float
  clamp(const float _t, const float _x, const float _y);

  /**
   * @brief clamps a Vector3 between 2 functions.
   * @param _t Vector3 to clamp.
   * @param _x Minimum clamping value.
   * @param _y Maximum clamping value.
   * @return Clamped float.
   */
  static Vector3
  clamp(const Vector3 _t, const float _x, const float _y);

  /**
   * @brief Check if a float number is in range of 2 values.
   * @param _t value to check for.
   * @param _x minimum value.
   * @param _y maximum value.
   * @return If it's in range or not.
   */
  static bool
  isInRange(const float _t, float _x, float _y);

  /**
   * @brief Gets the lerped number between 2 numbers in a specific timestamp.
   * @param _x The first number.
   * @param _y The second number.
   * @param _t Timestamp.
   * @return The lerped number.
   */
  static float
  lerp(const float _x, const float _y, const float _t);

  /**
   * @brief Gets the lerped number between 2 Vector3 in a specific timestamp.
   * @param _x The first Vector.
   * @param _y The second Vector.
   * @param _t Timestamp.
   * @return The lerped Vector.
   */
  static Vector3
  lerp3(const Vector3 _x, const Vector3 _y, const float _t);

  /**
   * @brief Gets the lerped number between 2 Vector4 in a specific timestamp.
   * @param _x The first Vector.
   * @param _y The second Vector.
   * @param _t Timestamp.
   * @return The lerped Vector.
   */
  static Vector4
  lerp4(const Vector4 _x, const Vector4 _y, const float _t);

  /**
   * @brief Hooke's law of elasticity.
   * @param _elasticity Elasticity to use.
   * @param _displacement Difference in distance.
   * @return The force applied.
   */
  static float
  hookeLaw(float _elasticity, float _displacement);

  /**
   * @brief Check if the value is nan.
   * @param _x Value to check.
   */
  static bool
  isNan(float _x);

  /***************************************************************************
  * 
  * Shapes
  * 
  ***************************************************************************/

  /***************************************************************************
  * Sphere
  ***************************************************************************/

  /**
  * @brief Check if there is a collision between 2 spheres.
  * @param _sphere The reference to the sphere.
  * @param _other The reference to the other sphere.
  * @param _cInfo Collision info.
  * @return A bool confirming or denying if there is a collision.
  **/
  static bool
  intersectSphereSphere(Sphere& _sphere, const Sphere& _other, CollisionInfo& _cInfo);

  /**
   * @brief Check if there is a collision between 2 spheres.
   * @param _sphere The reference to the sphere.
   * @param _other The point that will be ckecked.
   * @param _cInfo Collision info.
   * @return A bool confirming or denying if there is a collision.
   */
  static bool
  intersectSpherePoint(Sphere& _sphere, const Vector3& _other, CollisionInfo& _cInfo);

  /***************************************************************************
  * Capsule
  ***************************************************************************/

  /**
   * @brief Check if there is a collision between 2 Capsule.
   * @param _capsule Capsule to check.
   * @param _other The reference to the other Capsule.
   * @return A bool confirming or denying if there is a collision.
   */
  static bool
  intersectCapsuleCapsule(Capsule& _capsule, const Capsule& _other);

  /**
   * @brief Check if there is a collision between a capsule and a point.
   * @param _capsule The reference to the Capsule.
   * @param _other The reference to the Point.
   * @param _cInfo Collision info.
   * @return A bool confirming or denying if there is a collision.
   */
  static bool
  intersectCapsulePoint(Capsule& _capsule, Vector3& _other, CollisionInfo& _cInfo);

  /**
   * @brief Check if there is a collision between a capsule and a sphere.
   * @param _capsule The reference to the Capsule.
   * @param _spehre The reference to the other Sphere.
   * @param _cInfo Collision info.
   * @return A bool confirming or denying if there is a collision.
   */
  static bool
  intersectCapsuleSphere(Capsule& _capsule, Sphere& _sphere, CollisionInfo& _cInfo);

  /***************************************************************************
  * Cube
  ***************************************************************************/

  /**
   * @brief Check if there is a collision between 2 Cube.
   * @param _other The reference to the cube.
   * @param _other The reference to the other Cube.
   * @return A bool confirming or denying if there is a collision.
   */
  static bool
  intersectCubeCube(Cube& _cube, const Cube& _other);

  /**
   * @brief Check for a collision between a Cube and a Point.
   * @param _cube The reference to the cube.
   * @param _other The reference to the point.
   * @return A bool confirming or denying if there is a collision.
   */
  static bool
  intersectCubePoint(Cube& _cube, const Vector3& _other);

  /**
   * @brief Check for a collision between a Cube and a Sphere.
   * @param _cube Cube to check.
   * @param _sphere Sphere to check.
   * @return Wether there was an intersection or not.
   */
  static bool
  intersectCubeSphere(Cube& _cube, Sphere& _sphere);

  /**
   * @brief Get a signed value.
   * @param _val Value to sign.
   * @return The signed value.
   */
  static float
  sign(float _val);

  /**
   * @brief Get a signed direction.
   * @param _direction Direction to use.
   * @return The new signed direction.
   */
  static Vector3
  sign3(Vector3& _direction);

  /**
   * @brief Get the support point of an Object Bounding Box.
   * @param _box Box to study.
   * @param _dir Direction to use.
   * @return The support point of the box.
   */
  static Vector3
  supportPointOBB(OBB& _box, Vector3& _dir);

  /**
   * @brief Get the support point of a collection of points from a convex shape in a direction.
   * @param _direction Direction where the support point should be.
   * @param _points Group of points of  the shape.
   */
  static Vector3
  supportPointConvex(Vector3& _direction, Vector<Vector3>& _points);

  /**
   * @brief Get the support point from a sphere.
   * @param _direction Direction to use.
   * @param _sphere Sphere of use.
   * @return The support point.
   */
  static Vector3
  supportPointSphere(Vector3& _direction, Sphere& _sphere);

  /**
   * @brief Get the support point from a capsule.
   * @param _direction Direction to use.
   * @param _capsule Capsule of use.
   * @return The support point.|
   */
  static Vector3
  supportPointCapsule(Vector3& _direction, Capsule& _capsule);

  /**
   * Static variables
   */
  static const float PI;

  static const float EULER;

  static const float SMALL_NUMBER;

  static const float RAD2DEG;

  static const float DEG2RAD;
};

using Math = PlatformMath;
}