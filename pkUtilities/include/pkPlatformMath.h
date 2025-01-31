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
#include "pkPrerequisitesUtilities.h"
#include "pkSphere.h"
#include "pkCube.h"
#include "pkCapsule.h"
#include "pkPlane.h"

namespace pkEngineSDK 
{

class PK_UTILITY_EXPORT PlatformMath
{
 public:

  PlatformMath() = default;
  ~PlatformMath() = default;

  /**
  * absolute of a value.
  *
  * This function returns the absolute value of _x.
  *
  * @param _x
  * number to calculate the absolute value.
  *
  * @return
  * absolute value of the parameter.
  **/
  static float
  abs(const float _x);

  /**
  * power of a value.
  *
  * This function returns the power of _x.
  *
  * @param _x
  * number to calculate the power.
  *
  * @param _y
  * number of times _x will be multiplied by itself.
  *
  * @return
  * result of _x elevated by the power of _y.
  **/
  static float
  pow(const float _x, const float _y);

  /**
  * Euler elevated by _x.
  *
  * This function returns Euler elevated by _x.
  *
  * @param _x
  * number to elevate Euler.
  * 
  * @return
  * result of Euler elevated by _x.
  **/
  static float
  exp(const float _x);

  /**
  * square root.
  *
  * This function returns the square root of _x.
  *
  * @param _x
  * number to calculate the square root.
  *
  * @return
  * square root of _x.
  **/
  static float
  sqrt(const float _x);

  /**
  * minimum between the 2.
  *
  * This function returns the lowest of 2 parameters.
  *
  * @param _x
  * first number to compare.
  * 
  * @param _y
  * second number to compare
  *
  * @return
  * whichever value is the smallest.
  **/
  static float
  min(const float _x, const float _y);

  /**
  * maximum between the 2.
  *
  * This function returns the highest of 2 parameters.
  *
  * @param _x
  * first number to compare.
  *
  * @param _y
  * second number to compare
  *
  * @return
  * whichever value is the largest.
  **/
  static float
  max(const float _x, const float _y);

  /**
  * sin of _x.
  *
  * This function returns the sin of _x.
  *
  * @param _x
  * value to calculate sin.
  *
  * @return
  * sin of _x.
  **/
  static float
  sin(const float _x);

  /**
  * asin of _x.
  *
  * This function returns the asin of _x.
  *
  * @param _x
  * value to calculate asin.
  *
  * @return
  * asin of _x.
  **/
  static float
  asin(const float _x);

  /**
  * cos of _x.
  *
  * This function returns the cos of _x.
  *
  * @param _x
  * value to calculate cos.
  *
  * @return
  * cos of _x.
  **/
  static float
  cos(const float _x);

  /**
  * acos of _x.
  *
  * This function returns the acos of _x.
  *
  * @param _x
  * value to calculate acos.
  *
  * @return
  * acos of _x.
  **/
  static float
  acos(const float _x);

  /**
  * tan of _x.
  *
  * This function returns the tan of _x.
  *
  * @param _x
  * value to calculate tan.
  *
  * @return
  * tan of _x.
  **/
  static float
  tan(const float _x);

  /**
  * atan of _x.
  *
  * This function returns the atan of _x.
  *
  * @param _x
  * value to calculate atan.
  *
  * @return
  * atan of _x.
  **/
  static float
  atan(const float _x);

  /**
  * atan of float.
  *
  * This function clamps a float between 2 functions.
  *
  * @param _t
  * Float to clamp.
  *
  * @param _x
  * Minimum clamping value.
  * 
  * @param _y
  * Maximum clamping value.
  *
  * @return
  * Clamped float.
  **/
  static float
  clamp(const float _t, const float _x, const float _y);

  /**
  * Lerp between 2 numbers.
  * 
  * Gets the lerped number between 2 numbers in a specific timestamp.
  * 
  * @param _x
  * The first number.
  * 
  * @param _y
  * The second number.
  * 
  * @param _t
  * Timestamp.
  * 
  * @return
  * The lerped number.
  **/
  static float
  lerp(const float _x, const float _y, const float _t);

  /**
  * Lerp between 2 Vector3.
  *
  * Gets the lerped number between 2 Vector3 in a specific timestamp.
  *
  * @param _x
  * The first Vector.
  *
  * @param _y
  * The second Vector.
  *
  * @param _t
  * Timestamp.
  *
  * @return
  * The lerped Vector.
  **/
  static Vector3
  lerp3(const Vector3 _x, const Vector3 _y, const float _t);

  /**
  * Lerp between 2 Vector4.
  *
  * Gets the lerped number between 2 Vector4 in a specific timestamp.
  *
  * @param _x
  * The first Vector.
  *
  * @param _y
  * The second Vector.
  *
  * @param _t
  * Timestamp.
  *
  * @return
  * The lerped Vector.
  **/
  static Vector4
  lerp4(const Vector4 _x, const Vector4 _y, const float _t);

  /***************************************************************************
  * 
  * Shapes
  * 
  ***************************************************************************/

  /***************************************************************************
  * Sphere
  ***************************************************************************/

  /**
  * Sphere intersection.
  *
  * This function checks if there is a collision between 2 spheres
  * 
  * @param _sphere
  * The reference to the sphere
  *
  * @param _other
  * The reference to the other sphere
  *
  * @return
  * a bool confirming or denying if there is a collision
  **/
  static bool
  intersectSphereSphere(Sphere& _sphere, const Sphere& _other);

  /**
  * Sphere intersection.
  *
  * This function checks if there is a collision between 2 spheres
  * 
  * @param _sphere
  * The reference to the sphere
  *
  * @param _other
  * The point that will be ckecked
  *
  * @return
  * a bool confirming or denying if there is a collision
  **/
  static bool
  intersectSpherePoint(Sphere& _sphere, const Vector3& _other);

  /***************************************************************************
  * Capsule
  ***************************************************************************/

  /**
  * Capsule intersection.
  *
  * This function checks if there is a collision between 2 Capsule
  *
  * @param _other
  * The reference to the other Capsule
  *
  * @return
  * a bool confirming or denying if there is a collision
  **/
  static bool
  intersectCapsuleCapsule(Capsule& _capsule, const Capsule& _other);

  /**
  * Capsule intersection.
  *
  * This function checks if there is a collision between 2 Capsule
  * 
  * @param _capsule
  * The reference to the Capsule
  *
  * @param _other
  * The reference to the other Capsule
  *
  * @return
  * a bool confirming or denying if there is a collision
  **/
  static bool
  intersectCapsulePoint(Capsule& _capsule, Vector3& _other);

  /***************************************************************************
  * Cube
  ***************************************************************************/

  /**
  * Cube intersection Cube.
  *
  * This function checks if there is a collision between 2 Cube.
  *
  * @param _other
  * The reference to the cube.
  *
  * @param _other
  * The reference to the other Cube.
  *
  * @return
  * A bool confirming or denying if there is a collision.
  **/
  static bool
  intersectCubeCube(Cube& _cube, const Cube& _other);

  /**
  * Cube intersection Point.
  *
  * This function checks if there is a collision between a cube
  * and a point.
  * 
  * @param _cube
  * The reference to the cube.
  *
  * @param _other
  * The reference to the point.
  *
  * @return
  * A bool confirming or denying if there is a collision.
  **/
  static bool
  intersectCubePoint(Cube& _cube, const Vector3& _other);

  /**
   * @brief Check if the value is nan.
   * @param _x Value to check.
   */
  static bool
  isNan(float _x);

  /**
   * Static variables
  **/
  static const float PI;

  static const float EULER;

  static const float SMALL_NUMBER;

  static const float RAD2DEG;

  static const float DEG2RAD;
};

using Math = PlatformMath;
}