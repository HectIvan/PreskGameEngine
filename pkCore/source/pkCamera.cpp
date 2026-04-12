#include "pkCamera.h"
#include "pkLogger.h"
#include "pkActor.h"

namespace pkEngineSDK
{
Camera::Camera(const CameraDesc& _camDescription)
{
  init(_camDescription);
}

void
Camera::init(const CameraDesc& _desc)
{
  init(_desc.width, _desc.height, _desc.halfFOV, _desc.nearZ, _desc.farZ, _desc.eye, _desc.forward,
       _desc.right, _desc.camMode);
}

void
Camera::update(Actor& _owner)
{
  m_rotation = _owner.m_rotation;
  m_forward = _owner.m_forward;
  m_right = _owner.m_right;
  m_up = _owner.m_up;
  m_eye = Vector4(_owner.m_position, 1.0f);
  updateView();
}

void
Camera::init(const uint32& _width,
             const uint32& _height,
             const float& _halfFOV,
             const float& _nearZ,
             const float& _farZ,
             const Vector3& _eye,
             const Vector3& _forward,
             const Vector3& _right,
             const CAMERA_PROJ::E& _camMode)
{
  // creation parameters
  m_descriptor = CameraDesc(_width, _height, _halfFOV, _nearZ,
                            _farZ, _eye, _forward, _right, _camMode);
  m_eye = Vector4(_eye, 1.0f);
  m_at = Vector4(_eye + _forward, 0.0f);
  m_up = _forward.cross(_right);
  m_forward = _forward;
  m_right = _right;
  m_view = Matrix4::lookAtLH(m_eye, m_at, m_up);
  m_farNear = Vector2(_farZ, _nearZ);
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
Camera::move(const Vector3& _dist)
{
  // Vector3 offset = getRight() * _dist.x + getUp() * _dist.y + getForward() * _dist.z;
  m_eye += _dist;
  m_at += _dist;
  m_view = Matrix4::lookAtLH(m_eye, m_at, m_up);
}

void
Camera::moveForward(const float& _offset)
{
  const Vector3 pos = Vector3::FORWARD * _offset;
  m_eye += pos;
  m_at += pos;
  m_up += pos;
  m_view = Matrix4::lookAtLH(m_eye, m_at, m_up);
}

void
Camera::moveForwardLocal(const float& _offset)
{
  const Vector3 forward = (m_at.xyz() - m_eye.xyz()).normalized();
  m_at += forward * _offset;
  m_eye += forward * _offset;
  m_view = Matrix4::lookAtLH(m_eye, m_at, Vector3::UP);
}

void
Camera::moveRight(const float& _offset)
{
  const Vector3 pos = Vector3::RIGHT * _offset;
  m_eye += pos;
  m_at += pos;
  m_view = Matrix4::lookAtLH(m_eye, m_at, m_up);
}

void
Camera::moveRightLocal(const float& _offset)
{
  const Vector3 forward = (m_at.xyz() - m_eye.xyz()).normalized();
  const Vector3 right = m_view.getUpVector().cross(forward);
  m_at += right * _offset;
  m_eye += right * _offset;
  m_view = Matrix4::lookAtLH(m_eye, m_at, Vector3::UP);
}

void
Camera::moveUp(float _offset)
{
  const Vector3 pos = Vector3::UP * _offset;
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
Camera::rotate(const Vector3& _rotate)
{
  rotate(_rotate.x, _rotate.y, _rotate.z);
}

void
Camera::rotate(const float& _x, const float& _y, const float& _z)
{
  const Vector3 rotVec = Vector3(_x, _y, _z);

  Quaternion rot = Quaternion::fromEuler(rotVec);

  m_view *= Matrix4::rotation(rot);
  m_at = m_eye + m_view.getForwardVector();
  m_up = m_eye.xyz() + m_view.getUpVector();
  m_view = Matrix4::lookAtLH(m_eye, m_at, Vector3::UP);
}

void
Camera::rotation(const Quaternion& _rotation)
{
  Matrix4 rot = Matrix4::rotation(_rotation);
  m_at = m_eye + rot.getForwardVector();
  m_up = m_eye.xyz() + rot.getUpVector();
  m_view = Matrix4::lookAtLH(m_eye, m_at, Vector3::UP);
}

void
Camera::updateView()
{
  const Quaternion invRot = m_rotation.conjugate();
  
  const Matrix4 rot = Matrix4::rotation(invRot);
  const Matrix4 translation = Matrix4::translation(-m_eye.xyz()).getTransposed();
  m_view = rot * translation;// Matrix4::lookAtLH(m_eye, m_at, Vector3::UP);

  /* note: i seem to have accidentally implemented an orbit camera, something that i did intend on doing in the future,
     but not now, this was intended to be a first person camera, either way, i'll be saving this code for later.*/
}

Vector3
Camera::getForward() const
{
  Vector3 forwardVec = m_view.inverse().getForwardVector();
  forwardVec.normalize();
  return forwardVec;
}

Vector3
Camera::getRight() const
{
  Vector3 rightVec = m_view.inverse().getRightVector();
  rightVec.normalize();
  return rightVec;
}

Vector3
Camera::getUp() const
{
  Vector3 upVec = m_view.inverse().getUpVector();
  upVec.normalize();
  return upVec;
}
}
