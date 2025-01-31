#include "pkCamera.h"
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
  width = _width;
  height = _height;
  eye = Vector4(_eye, 1.0f);
  at = Vector4(_at, 0.0f);
  up = _up;
  forward = Vector3::FORWARD;
  right = Vector3::RIGHT;
  view = Matrix4::lookAtLH(eye, at, up);
  if (_camMode == CAMERA_PROJ::kPerspective) {
    projection = Matrix4::perspectiveFOVLH(_halfFOV,
                                           static_cast<float>(_width),
                                           static_cast<float>(_height),
                                           _nearZ,
                                           _farZ);
  }
  if (_camMode == CAMERA_PROJ::kOrthographic) {
    projection = Matrix4::orthographicFOVLH(static_cast<float>(_width) * -0.5f,
                                            static_cast<float>(_width) * 0.5f,
                                            static_cast<float>(_height) * -0.5f,
                                            static_cast<float>(_height) * 0.5f,
                                            _nearZ,
                                            _farZ);
  }
  rotation = Vector3(0.0f);
}

void
Camera::setView(const Vector4 _eye, const Vector4 _at, const Vector3 _up)
{
  
}

void
Camera::move(Vector3 _dist)
{
  std::cout << getForward().x << "  " << getForward().y << "  " << getForward().z << std::endl;
  Vector3 offset = getRight() * _dist.x + Vector3::UP * _dist.y + getForward() * _dist.z;
  eye += offset;  
  at = eye + offset;
  view = Matrix4::lookAtLH(eye, at, up);
}

void
Camera::rotate(float _x, float _y, float _z)
{
  // view *= Matrix4::rotation(_x, _y, _z);
  // updateRotation();
  // Matrix4 rotation = Matrix4::rotation(_x, _y, _z);
  // Vector4 newForward = (rotation * newForward).normalized();
  // 
  // setView(eye, eye - newForward, Vector3::UP);

  Matrix4 rotRight = Matrix4::MatrixRotationAxis(getRight(), _y);
  Matrix4 rotUp = Matrix4::MatrixRotationAxis(Vector3::UP, _x);
}

void
Camera::rotate(Vector3 _rotate)
{
  rotate(_rotate.x, _rotate.y, _rotate.z);
}

Vector3
Camera::getForward()
{
  Vector4 forwardVec = (at - eye);
  forwardVec.normalize();
  Vector3 forward3 = Vector3(forwardVec.x, forwardVec.y, forwardVec.z);
  setForward(forward3);
  return forward3;
}

Vector3
Camera::getRight()
{
  Vector3 rightVec = (getForward() ^ up);
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

void
Camera::addRotation(Vector3 _rot)
{
  rotation.x += _rot.x;
  rotation.y += _rot.y;
  rotation.z += _rot.z;
}
}