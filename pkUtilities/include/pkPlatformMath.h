/*****************************************************************************/
/**
 * @file    pkPlatformMath.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    06/09/2024
 * @brief   Basic math used for the game engine
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
#include "pkAABB.h"
#include "pkCapsule.h"
#include "pkCollisionInfo.h"
#include "pkCube.h"
#include "pkOBB.h"
#include "pkPlane.h"
#include "pkPrerequisitesUtilities.h"
#include "pkSphere.h"


namespace pkEngineSDK 
{

struct Color;
struct FColor;
class Vector2;
class Vector4;
class Quaternion;

namespace PK_ROT_TYPE
{
  enum E {
    kDegrees = 0,
    kRadians
  };
}

class PK_UTILITY_EXPORT PlatformMath
{
 public:
  PlatformMath() = default;
  ~PlatformMath() = default;

  /**
   * @brief Get the absolute value of _x.
   */
  PK_NODISCARD static int32
  abs(const int32& _x);

  /**
   * @brief Get the absolute value of _x.
   */
  PK_NODISCARD static float
  abs(const float& _x);

  /**
   * @brief Get the power of _x.
   * @param _x Number to calculate the power.
   * @param _y Number of times _x will be multiplied by itself.
   * @return Result of _x elevated by the power of _y.
   */
  PK_NODISCARD static float
  pow(const float& _x, const float& _y);

  /**
   * @brief Get Euler elevated by _x.
   */
  PK_NODISCARD static float
  exp(const float& _x);

  /**
   * @brief Get the square root of _x.
   */
  PK_NODISCARD static float
  sqrt(const float& _x);

  /**
   * @brief Compute the floating-point remainder of a division operation x / y
   */
  PK_NODISCARD static float
  fmod(const float& _x, const float& _y);

  /**
   * @brief Floor a float number down to its closest integer.
   */
  PK_NODISCARD static int32
  floor(const float& _x);

  /**
   * @brief Get the lowest of 2 parameters.
   * @param _x First number to compare.
   * @param _y Second number to compare.
   * @return Whichever value is the smallest.
   */
  PK_NODISCARD static uint32
  min(const uint32& _x, const uint32& _y);

  /**
   * @brief Get the lowest of 2 parameters.
   * @param _x First number to compare.
   * @param _y Second number to compare.
   * @return Whichever value is the smallest.
   */
  PK_NODISCARD static int32
  min(const int32& _x, const int32& _y);

  /**
   * @brief Get the lowest of 2 parameters.
   * @param _x First number to compare.
   * @param _y Second number to compare.
   * @return Whichever value is the smallest.
   */
  PK_NODISCARD static float
  min(const float& _x, const float& _y);

  /**
   * @brief Get the maximum value between 2 uint32.
   * @param _x First uint32 to compare.
   * @param _y Second uint32 to compare.
   */
  PK_NODISCARD static uint32
  max(const uint32& _x, const uint32& _y);

  /**
   * @brief Get the highest of 2 parameters.
   * @param _x First number to compare.
   * @param _y Second number to compare.
   * @return Whichever value is the largest.
   */
  PK_NODISCARD static float
  max(const float& _x, const float& _y);

  /**
   * @brief Get the maximum value between 2 floats.
   * @param _x First float to compare.
   * @param _y Second float to compare.
   */
  PK_NODISCARD static float
  maxf(const float& _x, const float& _y);

  /**
   * @brief Get the sin of _x.
   * @param _x Value to calculate sin.
   * @return sin of _x.
   */
  PK_NODISCARD static float
  sin(const float& _x);

  /**
   * @brief Get the sin of _x.
   * @param _x Value to calculate sin.
   * @return sin of _x.
   */
  PK_NODISCARD static float
  sinf(const float& _x);

  /**
   * @brief Get the asin of _x.
   * @param _x Value to calculate asin.
   * @return asin of _x.
   */
  PK_NODISCARD static float
  asin(const float& _x);

  /**
   * @brief Get the cos of _x.
   * @param _x Value to calculate cos.
   * @return cos of _x.
   */
  PK_NODISCARD static float
  cos(const float& _x);

  /**
   * @brief Get the acos of _x.
   * @param _x Value to calculate acos.
   * @return acos of _x.
   */
  PK_NODISCARD static float
  acos(const float& _x);

  /**
   * @brief Get the tan of _x.
   * @param _x Value to calculate tan.
   * @return tan of _x.
   */
  PK_NODISCARD static float
  tan(const float& _x);

  /**
   * @brief Get the atan of _x.
   * @param _x Value to calculate atan.
   * @return atan of _x.
   */
  PK_NODISCARD static float
  atan(const float& _x);

  /**
   * @brief Get the atan2 of _x.
   * @param _x Value to calculate atan2.
   * @param _y Second value to calculate atan2.
   * @return atan2 of _x and _y.
   */
  PK_NODISCARD static float
  atan2(const float& _x, const float& _y);

  /**
   * @brief Check if the value is nan.
   */
  PK_NODISCARD static bool
  isNan(const float& _x);

  /**
   * @brief Check if the vector is nan.
   */
  PK_NODISCARD static bool
  isNan(const Vector2& _x);

  /**
   * @brief Check if the vector is nan.
   */
  PK_NODISCARD static bool
  isNan(const Vector3& _x);

  /**
   * @brief Check if the vector is nan.
   */
  PK_NODISCARD static bool
  isNan(const Vector4& _x);

  /**
   * @brief Check if the quaternion is nan.
   */
  PK_NODISCARD static bool
  isNan(const Quaternion& _x);

  /**
   * @brief check if a number is inf.
   */
  PK_NODISCARD static bool
  isInf(const float& _x);

  /**
   * @brief check if a Vector is inf.
   */
  PK_NODISCARD static bool
  isInf(const Vector2& _x);

  /**
   * @brief check if a Vector is inf.
   */
  PK_NODISCARD static bool
  isInf(const Vector3& _x);

  /**
   * @brief check if a Vector is inf.
   */
  PK_NODISCARD static bool
  isInf(const Vector4& _x);

  /**
   * @brief check if a Quaternion is inf.
   */
  PK_NODISCARD static bool
  isInf(const Quaternion& _x);

  /**
   * @brief get the base 2 log of the number.
   * @param _x Value to calculate the log.
   */
  PK_NODISCARD static float
  log2(const float& _x);

  /**
   * @brief clamps a float between 2 functions.
   * @param _t Float to clamp.
   * @param _x Minimum clamping value.
   * @param _y Maximum clamping value.
   * @return Clamped float.
   */
  PK_NODISCARD static float
  clamp(const float& _t, const float& _x, const float& _y);

  /**
   * @brief clamps a Vector2 between 2 values.
   * @param _t Vector to clamp.
   * @param _x Minimum clamping value.
   * @param _y Maximum clamping value.
   * @return Clamped float.
   */
  PK_NODISCARD static Vector2
  clamp(const Vector2& _t, const float& _x, const float& _y);

  /**
   * @brief clamps a Vector3 between 2 values.
   * @param _t Vector to clamp.
   * @param _x Minimum clamping value.
   * @param _y Maximum clamping value.
   * @return Clamped float.
   */
  PK_NODISCARD static Vector3
  clamp(const Vector3& _t, const float& _x, const float& _y);

  /**
   * @brief Clamp a Vector4 between 2 values.
   * @param _t Vector to clamp.
   * @param _x Minimum clamping value.
   * @param _y Maximum clamping value.
   * @return Clamped float.
   */
  PK_NODISCARD static Vector4
  clamp(const Vector4& _t, const float& _x, const float& _y);

  /**
   * @brief Check if a float number is in range of 2 values.
   * @param _t value to check for.
   * @param _x minimum value.
   * @param _y maximum value.
   * @return If it's in range or not.
   */
  PK_NODISCARD static bool
  isInRange(const float& _t, const float& _x, const float& _y);

  /**
   * @brief Check if an uint32 number is in range of 2 values.
   * @param _t value to check for.
   * @param _x minimum value.
   * @param _y maximum value.
   * @return If it's in range or not.
   */
  PK_NODISCARD static bool
  isInRange(const uint32& _t, const uint32& _x, const uint32& _y);

  /**
   * @brief Lerp a number in a specific timestamp.
   * @param _x The first number.
   * @param _y The second number.
   * @param _t Timestamp.
   * @return The lerped number.
   */
  PK_NODISCARD static float
  lerp(const float& _x, const float& _y, const float& _t);

  /**
   * @brief Lerp a Vector in a specific timestamp.
   * @param _x The first Vector.
   * @param _y The second Vector.
   * @param _t Timestamp.
   * @return The lerped Vector.
   */
  PK_NODISCARD static Vector2
  lerp(const Vector2& _x, const Vector2& _y, const float& _t);

  /**
   * @brief Lerp a Vector in a specific timestamp.
   * @param _x First Vector.
   * @param _y Second Vector.
   * @param _t Timestamp.
   * @return The lerped Vector.
   */
  PK_NODISCARD static Vector3
  lerp(const Vector3& _x, const Vector3& _y, const float& _t);

  /**
   * @brief Lerp a Vector in a specific timestamp.
   * @param _x First Vector.
   * @param _y Second Vector.
   * @param _t Timestamp.
   * @return The lerped Vector.
   */
  PK_NODISCARD static Vector4
  lerp(const Vector4& _x, const Vector4& _y, const float& _t);

  /**
   * @brief Lerp an FColor in a specific timestamp.
   * @param _x FColor to lerp.
   * @param _y Target FColor.
   * @return Lerped FColor.
   */
  PK_NODISCARD static FColor
  lerp(const FColor& _x, const FColor& _y, const float& _t);

  /**
   * @brief Lerp a Color in a specific timestamp.
   * @param _x Color to lerp.
   * @param _y Target Color.
   * @return Lerped Color.
   */
  PK_NODISCARD static Color
  lerp(const Color& _x, const Color& _y, const float& _t);

  /**
   * @brief Hooke's law of elasticity.
   * @param _elasticity Elasticity to use.
   * @param _displacement Difference in distance.
   * @return The force applied.
   */
  PK_NODISCARD static float
  hookeLaw(const float& _elasticity, const float& _displacement);

  /**
   * @brief Dot product between 2 vectors.
   * @param _x Target.
   * @param _y Other.
   */
  PK_NODISCARD static float
  dotProd(const Vector2& _x, const Vector2& _y);

  /**
   * @brief Dot product between 2 vectors.
   * @param _x Target.
   * @param _y Other.
   */
  PK_NODISCARD static float
  dotProd(const Vector3& _x, const Vector3& _y);

  /**
   * @brief Dot product between 2 vectors.
   * @param _x Target.
   * @param _y Other.
   */
  PK_NODISCARD static float
  dotProd(const Vector4& _x, const Vector4& _y);

  /**
   * @brief Cross product between 2 vectors.
   */
  PK_NODISCARD static Vector3
  cross(const Vector3& _x, const Vector3& _y);

  /**
   * @brief Cross product between 2 vectors.
   */
  PK_NODISCARD static Vector4
  cross(const Vector4& _x, const Vector4& _y);

  /**
   * @brief Reflect a vector on a normal.
   * @param _direction Direction to reflect.
   * @param _normal Normal upon which to reflect.
   * @return Reflected vector.
   */
  PK_NODISCARD static Vector2
  reflect(const Vector2& _direction, const Vector3& _normal);

  /**
   * @brief Reflect a vector on a normal.
   * @param _direction Direction to reflect.
   * @param _normal Normal upon which to reflect.
   * @return Reflected vector.
   */
  PK_NODISCARD static Vector3
  reflect(const Vector3& _direction, const Vector3& _normal);

  /**
   * @brief Distance between 2 positions.
   * @param _x First distance.
   * @param _y Second distance.
   * @return Distance.
   */
  PK_NODISCARD static float
  distance(const Vector2& _x, const Vector2& _y);

  /**
   * @brief Distance between 2 positions.
   * @param _x First distance.
   * @param _y Second distance.
   * @return Distance.
   */
  PK_NODISCARD static float
  distance(const Vector3& _x, const Vector3& _y);

  /**
   * @brief Distance between 2 positions.
   * @param _x First distance.
   * @param _y Second distance.
   * @return Distance.
   */
  PK_NODISCARD static float
  distance(const Vector4& _x, const Vector4& _y);

  /**
   * @brief Get a random number.
   */
  PK_NODISCARD static float
  random();

  /**
   * @brief Get a random number.
   */
  PK_NODISCARD static int32
  randomInt32();

  /**
   * @brief get a random number in range.
   * @param _x Minimum value.
   * @param _y Maximum value.
   * @return Random number.
   */
  PK_NODISCARD static float
  randomRange(const float& _x, const float& _y);

  /**
   * @brief Get a random Vector2.
   */
  PK_NODISCARD static Vector2
  random2();

  /**
   * @brief Get a random Vector2 in range.
   */
  PK_NODISCARD static Vector2
  random2Range(const float& _x, const float& _y);

  /**
   * @brief Get a random Vector3.
   */
  PK_NODISCARD static Vector3
  random3();

  /**
   * @brief Get a random Vector3.
   */
  PK_NODISCARD static Vector3
  random3Range(const float& _x, const float& _y);

  /**
   * @brief Get a random Vector4.
   */
  PK_NODISCARD static Vector4
  random4();

  /**
   * @brief Get a random Vector4.
   */
  PK_NODISCARD static Vector4
  random4Range(const float& _x, const float& _y);

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
  intersectSphereSphere(const Sphere& _sphere,
                        const Sphere& _other,
                        CollisionInfo& _cInfo);

  /**
   * @brief Check if there is a collision between 2 spheres.
   * @param _sphere The reference to the sphere.
   * @param _other The point that will be ckecked.
   * @param _cInfo Collision info.
   * @return A bool confirming or denying if there is a collision.
   */
  static bool
  intersectSpherePoint(const Sphere& _sphere,
                       const Vector3& _other,
                       CollisionInfo& _cInfo);

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
  intersectCapsulePoint(const Capsule& _capsule, const Vector3& _other, CollisionInfo& _cInfo);

  /**
   * @brief Check if there is a collision between a capsule and a sphere.
   * @param _capsule The reference to the Capsule.
   * @param _spehre The reference to the other Sphere.
   * @param _cInfo Collision info.
   * @return A bool confirming or denying if there is a collision.
   */
  static bool
  intersectCapsuleSphere(const Capsule& _capsule, const Sphere& _sphere, CollisionInfo& _cInfo);

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
  intersectCubeCube(const Cube& _cube, const Cube& _other);

  /**
   * @brief Check for a collision between a Cube and a Point.
   * @param _cube The reference to the cube.
   * @param _other The reference to the point.
   * @return A bool confirming or denying if there is a collision.
   */
  static bool
  intersectCubePoint(const Cube& _cube, const Vector3& _other);

  /**
   * @brief Check for a collision between a Cube and a Sphere.
   * @param _cube Cube to check.
   * @param _sphere Sphere to check.
   * @return Wether there was an intersection or not.
   */
  static bool
  intersectCubeSphere(const Cube& _cube, const Sphere& _sphere);

  /**
   * @brief Get a signed value.
   * @param _val Value to sign.
   * @return The signed value.
   */
  static float
  sign(const float& _val);

  /**
   * @brief Get a signed direction.
   * @param _direction Direction to use.
   * @return The new signed direction.
   */
  static Vector3
  sign(const Vector3& _direction);

  /**
   * @brief Get the support point of an Object Bounding Box.
   * @param _box Box to study.
   * @param _dir Direction to use.
   * @return The support point of the box.
   */
  static Vector3
  supportPointOBB(const OBB& _box, const Vector3& _dir);

  /**
   * @brief Get the support point of a collection of points from a convex shape in a direction.
   * @param _direction Direction where the support point should be.
   * @param _points Group of points of  the shape.
   */
  static Vector3
  supportPointConvex(const Vector3& _direction, const Vector<Vector3>& _points);

  /**
   * @brief Get the support point from a sphere.
   * @param _direction Direction to use.
   * @param _sphere Sphere of use.
   * @return The support point.
   */
  static Vector3
  supportPointSphere(const Vector3& _direction, const Sphere& _sphere);

  /**
   * @brief Get the support point from a capsule.
   * @param _direction Direction to use.
   * @param _capsule Capsule of use.
   * @return The support point.
   */
  static Vector3
  supportPointCapsule(const Vector3& _direction, const Capsule& _capsule);

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
