/*****************************************************************************/
/**
 * @file    pkCamera.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    30/09/2024
 * @brief   Camera component for the game engine.
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
#include "pkComponent.h"
#include "pkMatrix4.h"
#include "pkPrerequisitesCore.h"
#include "pkVector2.h"
#include "pkVector3.h"
#include "pkVector4.h"
#include "pkQuaternion.h"

namespace pkEngineSDK
{

class Actor;
class Quaternion;

namespace CAMERA_PROJ
{
  enum E : uint32
  {
    kPerspective = 0,
    kOrthographic,
  };
}

struct CameraDesc
{
  CameraDesc() = default;
  CameraDesc(uint32 _width, uint32 _height, float _halfFOV, float _nearZ, float _farZ,
             Vector3 _eye, Vector3 _at, Vector3 _up, CAMERA_PROJ::E _camMode) {
    width = _width;
    height = _height;
    halfFOV = _halfFOV;
    nearZ = _nearZ;
    farZ = _farZ;
    eye = _eye;
    at = _at;
    up = _up;
    camMode = _camMode;
  }
  uint32 width;
  uint32 height;
  float halfFOV;
  float nearZ;
  float farZ;
  Vector3 eye;
  Vector3 at;
  Vector3 up;
  CAMERA_PROJ::E camMode;
};

class PK_CORE_EXPORT Camera : public Component
{
 public:
  Camera() = default;
  virtual ~Camera() = default;

  /**
   * @brief Initialize the camera based on a descriptor.
   * @param _desc Descriptor to use.
   */
  void
  init(const CameraDesc& _desc);

  /**
   * @brief Update the camera component.
   * @param _owner The actor that owns this component.
   */
  void
  update(Actor& _owner) override;

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
  init(const uint32& _width,
       const uint32& _height,
       const float& _halfFOV,
       const float& _nearZ,
       const float& _farZ,
       const Vector3& _eye,
       const Vector3& _at,
       const Vector3& _up,
       const CAMERA_PROJ::E& _camMode = CAMERA_PROJ::kPerspective
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
  move(const Vector3& _dist);

  /**
   * @brief Move the camera forward in the global axis.
   * @param _offset Distance to move.
   */
  void
  moveForward(const float& _offset);

  /**
   * @brief Move the camera forward in its local axis.
   * @param _offset Distance to move.
   */
  void
  moveForwardLocal(const float& _offset);

  /**
   * @brief Move the camera right in the global axis.
   * @param _offset Distance to move.
   */
  void
  moveRight(const float& _offset);

  /**
   * @brief Move the camera right in its local axis.
   * @param _offset Distance to move.
   */
  void
  moveRightLocal(const float& _offset);

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
   * @brief Rotate the camera.
   * @param _rotation Rotation amount.
   */
  void
  rotate(const Vector3& _rotation);

  /**
   * @brief Rotate the camera.
   * @param _x x rotation amount.
   * @param _y y rotation amount.
   * @param _z z rotation amount.
   */
  void
  rotate(const float& _x, const float& _y, const float& _z);

  /**
   * @brief Set the rotation of the camera.
   * @param _rotation New rotation values.
   */
  void
  rotation(const Quaternion& _rotation);

  /**
   * @brief Get the forward vector of the camera.
   * @return The forward vector as a vector4.
   */
  Vector3
  getForward() const;

  /**
   * @brief Get the right vector of the camera.
   * @return The right vector as a vector4.
   */
  Vector3
  getRight() const;

  /**
   * @brief Get the up vector of the camera.
   * @return The up vector as a vector4.
   */
  Vector3
  getUp() const;

  /**
   * @brief Get the position of the camera.
   * @return The position of the camera as a vector4.
   */
  FORCEINLINE const Vector4&
  getEye() const { return m_eye; }

  /**
   * @brief Get the near and far planes of the camera.
   * @return The near and far planes as a vector2.
   */
  FORCEINLINE const Vector2&
  getFarNear() const { return m_farNear; }

  /**
   * @brief Get the view matrix of the camera.
   * @return The view matrix.
   */
  FORCEINLINE const Matrix4&
  getView() const { return m_view; }

  /**
   * @brief Get the projection matrix of the camera.
   * @return The projection matrix.
   */
  FORCEINLINE const Matrix4&
  getProjection() const { return m_projection; }

  /**
   * @brief Get the point the camera is looking at.
   * @return The point the camera is looking at as a vector4.
   */
  FORCEINLINE const Vector4&
  getAt() const { return m_at; }

  /**
   * @brief Get the camera descriptor.
   * @return The camera descriptor.
   */
  FORCEINLINE const CameraDesc
  getDescriptor() const { return m_descriptor; }

  /**
   * @brief Set the forward vector of the camera.
   * @param _vec New forward vector.
   */
  FORCEINLINE void
  setForward(const Vector3& _vec) { m_forward = _vec; }

  /**
   * @brief Set the right vector of the camera.
   * @param _vec New right vector.
   */
  FORCEINLINE void
  setRight(const Vector3& _vec) { m_right = _vec; }

  /**
   * @brief Set the up vector of the camera.
   * @param _vec New up vector.
   */
  FORCEINLINE void
  setUp(const Vector3& _vec) { m_up = _vec; }

  /**
   * @brief Get the component type of this component.
   * @return The component type.
   */
  COMPONENT_TYPE::E
  getType() override { return COMPONENT_TYPE::kCamera; }

  /**
   * @brief Get the name of the component.
   */
  const ANSICHAR*
  getName() override { return "Camera"; }

  /**
   * @brief Get the component type of this component.
   * @return The component type.
   */
  static COMPONENT_TYPE::E
  getObjType() { return COMPONENT_TYPE::kCamera; }

 private:
  // Camera view
  Matrix4 m_view;
  Matrix4 m_projection;
  uint32 m_width;
  uint32 m_height;
  CAMERA_PROJ::E m_projType;
  CameraDesc m_descriptor;

  // Camera info
  Vector4 m_eye;
  Vector4 m_at;

  // camera vectors
  Vector3 m_forward;
  Vector3 m_right;
  Vector3 m_up;

  // camera rotation and start position
  Quaternion m_rotation = Quaternion::IDENTITY;
  Vector2 m_startPos = Vector2(0.0f);

  Vector2 m_farNear;
};
}
