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
  width = _width;
  height = _height;
  eye = _eye;
  at = _at;
  up = _up;
  view = Matrix4::lookAtLH(eye, at, up);
  projection = Matrix4::perspectiveFOVLH(_halfFOV,
                                           static_cast<float>(_width),
                                           static_cast<float>(_height),
                                           _nearZ,
                                           _farZ);
}

void
Camera::move(Vector3 _dist)
{
  eye += Vector4(_dist.x, _dist.y, _dist.z, 0.0f);
  at += Vector4(_dist.x, _dist.y, _dist.z, 0.0f);
  up = getUpVector();
  view = Matrix4::lookAtLH(eye, at, up);
}

void
Camera::rotate(float _x, float _y, float _z)
{
  at = Matrix4::rotation(_x, _y, _z) * at;
}

void
Camera::rotate(Vector3 _rotate)
{
  at = Matrix4::rotation(_rotate.x, _rotate.y, _rotate.z) * at;
}

Vector4
Camera::getForwardVector()
{
  Vector4 forwardVec = (at - eye);
  forwardVec.normalize();
  return forwardVec;
}

Vector4
Camera::getRightVector()
{
  Vector4 rightVec = (up ^ getForwardVector());
  rightVec.normalize();
  return rightVec;
}

Vector4
Camera::getUpVector()
{
  Vector4 upVec = getForwardVector() ^ getRightVector();
  upVec.normalize();
  return upVec;
}

void
Camera::updateRotation()
{
  setForwardVector(getForwardVector());
  setRightVector(getRightVector());
  setUpVector(getUpVector());
}
}