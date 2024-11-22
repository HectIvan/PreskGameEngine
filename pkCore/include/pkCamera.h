/************************************************************************/
/**
* @pkCamera pkCamera.h
* @Hector Ivan Muñoz Ceballos
* @date 30/09/2024
* @Camera file for the Presk Game Engine.
*
* This file will contain the Camera used for the engine
*
* @bug No bug known.
*
* @HectIvan 30/09/2024
*/
/************************************************************************/
#pragma once

#include "pkMatrix4.h"
#include "pkPrerequisitesCore.h"
#include "pkVector2.h"
#include "pkVector3.h"
#include "pkVector4.h"

namespace pkEngineSDK
{

class Camera
{
 public:
  Camera() = default;
  virtual ~Camera() = default;

  /**
  * Initialize the camera.
  **/
  void
  Init(uint32 _width,
       uint32 _height,
       float _halfFOV,
       float _nearZ,
       float _farZ,
       Vector4 _eye,
       Vector4 _at,
       Vector4 _up
  );
  
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
  Vector4
  getForwardVector();

  /**
  * Gets the right vector of the camera.
  *
  * @return
  * The right vector as a vector4.
  **/
  Vector4
  getRightVector();

  /**
  * Gets the up vector of the camera.
  *
  * @return
  * The up vector as a vector4.
  **/
  Vector4
  getUpVector();

  /**
  * Sets the forward vector of the camera.
  * 
  * @param _vec
  * New forward vector.
  **/
  void
  setForwardVector(Vector4 _vec) { m_forward = _vec; }

  /**
  * Sets the right vector of the camera.
  * 
  * @param _vec
  * New right vector.
  **/
  void
  setRightVector(Vector4 _vec) { m_right = _vec; }

  /**
  * Sets the up vector of the camera.
  * 
  * @param _vec
  * New up vector.
  **/
  void
  setUpVector(Vector4 _vec) { m_up = _vec; }

  /**
  * Set all the direction vectors.
  **/
  void
  updateRotation();

  // Camera view
  Matrix4 m_view;
  Matrix4 m_projection;
  uint32 m_width;
  uint32 m_height;

  // Camera position
  Vector4 m_eye;
  Vector4 m_at;
  Vector4 m_up;

  // camera vectors
  Vector4 m_forward;
  Vector4 m_right;

  // camera rotation and start position
  Vector3 m_rotation = Vector3(0.0f);
  Vector2 m_startPos = Vector2(0.0f);
};
}