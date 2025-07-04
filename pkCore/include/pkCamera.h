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
#include "pkComponent.h"
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

class PK_CORE_EXPORT Camera : public Component
{
 public:
  Camera() = default;
  virtual ~Camera() = default;

  /**
   * @brief Initialize the camera.
   * @param _width Width to the view.
   * @param _height Height of the view.
   * @param _halfFov Half of the field of view.
   * @param _nearZ Nearest point to the camera.
   * @param _farZ Furthest point to the camera.
   * @param _eye Position of the camera view.
   * @param _at Where the camera will be looking at.
   * @param _up Up vector of the camera.
   * @param _camMode Wether Camera perspective.
   */
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
   * @brief Move the camera to a new position.
   * @param _dist New position of the camera.
   */
  void
  move(Vector3 _dist);

  /**
   * @brief Move the camera forward in the global axis.
   * @param _offset Distance to move.
   */
  void
  moveForward(float _offset);

  /**
   * @brief Move the camera forward in its local axis.
   * @param _offset Distance to move.
   */
  void
  moveForwardLocal(float _offset);

  /**
   * @brief Move the camera right in the global axis.
   * @param _offset Distance to move.
   */
  void
  moveRight(float _offset);

  /**
   * @brief Move the camera right in its local axis.
   * @param _offset Distance to move.
   */
  void
  moveRightLocal(float _offset);

  /**
   * @brief Move the camera up in the global axis.
   * @param _offset Distance to move.
   */
  void
  moveUp(float _offset);

  /**
   * @brief Move the camera up in its local axis.
   * @param _offset Distance to move.
   */
  void
  moveUpLocal(float _offset);

  /**
   * @brief Rotate the camera by rotating the At vector.
   * @param _x New x rotation.
   * @param _y New y rotation.
   * @param _z New z rotation.
   */
  void
  rotate(float _x, float _y, float _z);

  /**
   * @brief Rotate the camera by rotating the At vector.
   * @param _rotate New rotation vector.
   */
  void
  rotate(Vector3 _rotate);

  /**
   * @brief Get the forward vector of the camera.
   * @return The forward vector as a vector4.
   */
  Vector3
  getForward();

  /**
   * @brief Get the right vector of the camera.
   * @return The right vector as a vector4.
   */
  Vector3
  getRight();

  /**
   * @brief Get the up vector of the camera.
   * @return The up vector as a vector4.
   */
  Vector3
  getUp();

  /**
   * @brief Set the forward vector of the camera.
   * @param _vec New forward vector.
   */
  void
  setForward(Vector3 _vec) { m_forward = _vec; }

  /**
   * @brief Set the right vector of the camera.
   * @param _vec New right vector.
   */
  void
  setRight(Vector3 _vec) { m_right = _vec; }

  /**
   * @brief Set the up vector of the camera.
   * @param _vec New up vector.
   */
  void
  setUp(Vector3 _vec) { m_up = _vec; }

  /**
   * @brief Get the component type of this component.
   * @return The component type.
   */
  COMPONENT_TYPE::E
  getType() override { return COMPONENT_TYPE::kCamera; }

  /**
   * @brief Get the name of the component.
   */
  const char*
  getName() override { return "Camera"; }

  /**
   * @brief Get the component type of this component.
   * @return The component type.
   */
  static COMPONENT_TYPE::E
  getObjType() { return COMPONENT_TYPE::kCamera; }

 public:
  // Camera view
  Matrix4 m_view;
  Matrix4 m_projection;
  uint32 m_width;
  uint32 m_height;

  // Camera position
  Vector4 m_eye;
  Vector4 m_at;
  Vector3 m_up;

  // camera vectors
  Vector3 m_forward;
  Vector3 m_right;

  // camera rotation and start position
  Vector3 m_rotation;
  Vector2 m_startPos = Vector2(0.0f);
};
}
