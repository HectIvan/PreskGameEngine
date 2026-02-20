#include "pkCamera.h"
#include "pkLogger.h"
#include "pkActor.h"

namespace pkEngineSDK
{
void
Camera::init(const CameraDesc& _desc)
{
  init(_desc.width, _desc.height, _desc.halfFOV, _desc.nearZ, _desc.farZ, _desc.eye, _desc.at,
       _desc.up, _desc.camMode);
}

void
Camera::update(Actor& _owner)
{
  rotation(_owner.m_rotation);
  // m_view *= Matrix4::translation(_owner.m_position);
}

void
Camera::init(const uint32& _width,
             const uint32& _height,
             const float& _halfFOV,
             const float& _nearZ,
             const float& _farZ,
             const Vector3& _eye,
             const Vector3& _at,
             const Vector3& _up,
             const CAMERA_PROJ::E& _camMode)
{
  // creation parameters
  m_descriptor = CameraDesc(_width, _height, _halfFOV, _nearZ,
                            _farZ, _eye, _at, _up, _camMode);
  m_eye = Vector4(_eye, 1.0f);
  m_at = Vector4(_at, 0.0f);
  m_up = _up;
  m_forward = Vector3::FORWARD;
  m_right = Vector3::RIGHT;
  m_view = Matrix4::lookAtLH(m_eye, m_at, m_up);
  m_farNear = Vector2(_farZ, _nearZ);
  m_projType = _camMode;
  if (_camMode == CAMERA_PROJ::kPerspective) {
    m_projection = Matrix4::perspectiveFOVLH(_halfFOV,
                                             static_cast<float>(_width),
                                             static_cast<float>(_height),
                                             _nearZ,
                                             _farZ);
  }
  if (_camMode == CAMERA_PROJ::kOrthographic) {
    m_projection = Matrix4::orthographicFOVLH(static_cast<float>(_width) * -0.5f,
                                              static_cast<float>(_width) * 0.5f,
                                              static_cast<float>(_height) * -0.5f,
                                              static_cast<float>(_height) * 0.5f,
                                              _nearZ,
                                              _farZ);
  }
  m_rotation = Vector3(0.0f);
  LOG_REGISTER("Created Camera component.", __FILE__, __LINE__);
}

void
Camera::setView(const Vector4 _eye, const Vector4 _at, const Vector3 _up)
{
  m_view = Matrix4::lookAtLH(_eye, _at, _up);

  m_forward = Vector3(m_view.matrix[0][2], m_view.matrix[1][2], m_view.matrix[2][2]);
  m_right = Vector3(m_view.matrix[0][0], m_view.matrix[1][0], m_view.matrix[2][0]);
  m_up = Vector3(m_view.matrix[0][1], m_view.matrix[1][1], m_view.matrix[2][1]);
}

void
Camera::move(Vector3 _dist)
{
  // Vector3 offset = getRight() * _dist.x + getUp() * _dist.y + getForward() * _dist.z;
  m_eye += _dist;
  m_at += _dist;
  m_view = Matrix4::lookAtLH(m_eye, m_at, m_up);
}

void
Camera::moveForward(float _offset)
{
  Vector3 pos = Vector3::FORWARD * _offset;
  m_eye += pos;
  m_at += pos;
  m_up += pos;
  m_view = Matrix4::lookAtLH(m_eye, m_at, m_up);
}

void
Camera::moveForwardLocal(float _offset)
{
  Vector3 forward = (m_at.xyz() - m_eye.xyz()).normalized();
  m_at += forward * _offset;
  m_eye += forward * _offset;
  m_view = Matrix4::lookAtLH(m_eye, m_at, Vector3::UP);
}

void
Camera::moveRight(float _offset)
{
  Vector3 pos = Vector3::RIGHT * _offset;
  m_eye += pos;
  m_at += pos;
  m_view = Matrix4::lookAtLH(m_eye, m_at, m_up);
}

void
Camera::moveRightLocal(float _offset)
{
  Vector3 forward = (m_at.xyz() - m_eye.xyz()).normalized();
  Vector3 right = m_view.getUpVector().cross(forward);
  m_at += right * _offset;
  m_eye += right * _offset;
  m_view = Matrix4::lookAtLH(m_eye, m_at, Vector3::UP);
}

void
Camera::moveUp(float _offset)
{
  Vector3 pos = Vector3::UP * _offset;
  m_eye += pos;
  m_at += pos;
  m_up += pos;
  m_view = Matrix4::lookAtLH(m_eye, m_at, m_up);
}

void
Camera::moveUpLocal(float _offset)
{
  m_at += Vector3::UP * _offset;
  m_eye += Vector3::UP * _offset;
  m_view = Matrix4::lookAtLH(m_eye, m_at, Vector3::UP);
}

void
Camera::rotate(const Vector3& _rotate, const PK_ROT_TYPE::E& _rotType)
{
  rotate(_rotate.x, _rotate.y, _rotate.z, _rotType);
}

void
Camera::rotate(const float& _x,
               const float& _y,
               const float& _z,
               const PK_ROT_TYPE::E& _rotType)
{
  Vector3 rot = Vector3(_x, _y, _z);

  if (_rotType == PK_ROT_TYPE::kDegrees) {
    rot *= Math::DEG2RAD;
  }

  m_view *= Matrix4::rotation(rot);
  m_at = m_eye + m_view.getForwardVector();
  m_up = m_eye.xyz() + m_view.getUpVector();
  m_view = Matrix4::lookAtLH(m_eye, m_at, Vector3::UP);
}

void
Camera::rotation(const Vector3& _rotation, const PK_ROT_TYPE::E& _rotType)
{
  Vector3 rot = _rotation;

  if (_rotType == PK_ROT_TYPE::kDegrees) {
    rot *= Math::DEG2RAD;
  }

  m_view = Matrix4::rotation(rot);
  m_at = m_eye + m_view.getForwardVector();
  m_up = m_eye.xyz() + m_view.getUpVector();
  m_view = Matrix4::lookAtLH(m_eye, m_at, Vector3::UP);
}

Vector3
Camera::getForward()
{
  Vector3 forwardVec = m_view.inverse().getForwardVector();
  forwardVec.normalize();
  return forwardVec;
}

Vector3
Camera::getRight()
{
  Vector3 rightVec = m_view.inverse().getRightVector();
  rightVec.normalize();
  return rightVec;
}

Vector3
Camera::getUp()
{
  Vector3 upVec = m_view.inverse().getUpVector();
  upVec.normalize();
  return upVec;
}
}
