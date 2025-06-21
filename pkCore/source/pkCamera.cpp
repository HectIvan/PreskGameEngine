#include "pkCamera.h"
#include "pkLogger.h"
#include <iostream>

namespace pkEngineSDK
{

void
Camera::init(uint32 _width,
             uint32 _height,
             float _halfFOV,
             float _nearZ,
             float _farZ,
             Vector3 _eye,
             Vector3 _at,
             Vector3 _up,
             CAMERA_PROJ::E _camMode)
{
  m_width = _width;
  m_height = _height;
  m_eye = Vector4(_eye, 1.0f);
  m_at = Vector4(_at, 0.0f);
  m_up = _up;
  m_forward = Vector3::FORWARD;
  m_right = Vector3::RIGHT;
  m_view = Matrix4::lookAtLH(m_eye, m_at, m_up);
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
  Vector3 offset = getRight() * _dist.x + getUp() * _dist.y + getForward() * _dist.z;
  m_eye += _dist;
  m_at += _dist;
  m_view = Matrix4::lookAtLH(m_eye, m_at, m_up);
}

void
Camera::moveForward(float _offset)
{
  // scale is the third column in the matrix, which in a view matrix is the view direction.
  Vector3 forward = (m_view.inverse().getScale3()).normalized();
  // m_eye += forward * _offset;
  // m_at += forward * _offset;
  Vector3 pos = forward * _offset;
  m_view *= Matrix4::translation(pos);
  // m_view = Matrix4::lookAtLH(m_eye, m_at, m_up);
}

void
Camera::rotate(float _x, float _y, float _z)
{
  m_view *= Matrix4::rotation(_x, _y, _z);
}

void
Camera::rotate(Vector3 _rotate)
{
  rotate(_rotate.x, _rotate.y, _rotate.z);
}

Vector3
Camera::getForward()
{
  Vector4 forwardVec = (m_at - m_eye);
  forwardVec.normalize();
  Vector3 forward3 = Vector3(forwardVec.x, forwardVec.y, forwardVec.z);
  setForward(forward3);
  return forward3;
}

Vector3
Camera::getRight()
{
  Vector3 rightVec = (getForward() ^ m_up);
  rightVec.normalize();
  setRight(rightVec);
  return rightVec;
}

Vector3
Camera::getUp()
{
  Vector3 upVec = getForward() ^ getRight();
  upVec.normalize();
  setUp(upVec);
  return upVec;
}

void
Camera::updateRotation()
{
  setForward(getForward());
  setRight(getRight());
  setUp(getUp());
}
}