#include "pkCamera.h"

namespace pkEngineSDK
{
void
Camera::init(uint32 _width,
             uint32 _height,
             float _halfFOV,
             float _nearZ,
             float _farZ,
             Vector4 _eye,
             Vector4 _at,
             Vector4 _up)
{
  m_width = _width;
  m_height = _height;
  m_eye = _eye;
  m_at = _at;
  m_up = _up;
  m_view = Matrix4::lookAtLH(m_eye, m_at, m_up);
  m_projection = Matrix4::perspectiveFOVLH(_halfFOV,
                                           static_cast<float>(_width),
                                           static_cast<float>(_height),
                                           _nearZ,
                                           _farZ);
}

void
Camera::move(Vector3 _dist)
{
  m_eye += Vector4(_dist.x, _dist.y, _dist.z, 0.0f);
  m_at += Vector4(_dist.x, _dist.y, _dist.z, 0.0f);
  m_up = getUpVector();
}

void
Camera::rotate(float _x, float _y, float _z)
{
  m_at = Matrix4::rotation(_x, _y, _z) * m_at;
}

void
Camera::rotate(Vector3 _rotate)
{
  m_at = Matrix4::rotation(_rotate.x, _rotate.y, _rotate.z) * m_at;
}

Vector4
Camera::getForwardVector()
{
  Vector4 forward = (m_at - m_eye);
  forward.normalize();
  return forward;
}

Vector4
Camera::getRightVector()
{
  Vector4 right = (m_up ^ getForwardVector());
  right.normalize();
  return right;
}

Vector4
Camera::getUpVector()
{
  Vector4 up = getForwardVector() ^ getRightVector();
  up.normalize();
  return up;
}

void
Camera::updateRotation()
{
  setForwardVector(getForwardVector());
  setRightVector(getRightVector());
  setUpVector(getUpVector());
}
}