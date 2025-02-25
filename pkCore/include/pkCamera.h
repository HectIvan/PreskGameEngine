/************************************************************************/
/**
* @pkCamera pkCamera.h
* @Hector Ivan Muñoz Ceballos
* @date 30/09/2024
* @Camera file for the Presk Game Engine.
*
* This file will contain the Camera used for the engine
*
* @bug.
* Camera movement not as intended when moving to a certain Z coordinat range.
* Erratic camera rotation when its performed constantly.
*
* @HectIvan 02/01/2025
* Movement and camera rotation at the same time.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkMatrix4.h"
#include "pkPrerequisitesCore.h"
#include "pkVector2.h"
#include "pkVector3.h"
#include "pkVector4.h"

namespace pkEngineSDK
{

namespace CAMERA_PROJ
{
  enum E : uint32
  {
    kPerspective = 0,
    kOrthographic,
  };
}

class PK_CORE_EXPORT Camera
{
 public:
  Camera() = default;
  virtual ~Camera() = default;

  /**
  * Initialize the camera.
  **/
  void
  init(uint32 _width,
       uint32 _height,
       float _halfFOV,
       float _nearZ,
       float _farZ,
       Vector3 _eye,
       Vector3 _at,
       Vector3 _up,
       CAMERA_PROJ::E _camMode = CAMERA_PROJ::kPerspective
  );

  /**
   * @brief Set the view matrix data
   */
  void
  setView(const Vector4 _eye, const Vector4 _at, const Vector3 _up);
  
  /**
  * Move the camera to a new position.
  * 
  * @param _dist
  * New position of the camera.
  **/
  void
  move(Vector3 _dist);

  /**
  * Rotate the camera by rotating the At vector
  * 
  * @param _x
  * New x rotation.
  * 
  * @param _y
  * New y rotation.
  * 
  * @param _z
  * New z rotation.
  **/
  void
  rotate(float _x, float _y, float _z);

  /**
  * Rotate the camera by rotating the At vector
  *
  * @param _rotate
  * New rotation vector.
  **/
  void
  rotate(Vector3 _rotate);

  /**
  * Gets the forward vector of the camera.
  * 
  * @return
  * The forward vector as a vector4.
  **/
  Vector3
  getForward();

  /**
  * Gets the right vector of the camera.
  *
  * @return
  * The right vector as a vector4.
  **/
  Vector3
  getRight();

  /**
  * Gets the up vector of the camera.
  *
  * @return
  * The up vector as a vector4.
  **/
  Vector3
  getUp();

  /**
  * Sets the forward vector of the camera.
  * 
  * @param _vec
  * New forward vector.
  **/
  void
  setForward(Vector3 _vec) { forward = _vec; }

  /**
  * Sets the right vector of the camera.
  * 
  * @param _vec
  * New right vector.
  **/
  void
  setRight(Vector3 _vec) { right = _vec; }

  /**
  * Sets the up vector of the camera.
  * 
  * @param _vec
  * New up vector.
  **/
  void
  setUp(Vector3 _vec) { up = _vec; }

  /**
  * Set all the direction vectors.
  **/
  void
  updateRotation();

  // Camera view
  Matrix4 view;
  Matrix4 projection;
  uint32 width;
  uint32 height;

  // Camera position
  Vector4 eye;
  Vector4 at;
  Vector3 up;

  // camera vectors
  Vector3 forward;
  Vector3 right;

  // camera rotation and start position
  Vector3 rotation;
  Vector2 startPos = Vector2(0.0f);
};
}