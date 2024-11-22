#include "pkTransform.h"

namespace pkEngineSDK
{

Transform::Transform(Matrix4 _rotation,
                     Matrix4 _scale,
                     Matrix4 _translation)
{
  setRotation(_rotation);
  setScale(_rotation);
  setTranslation(_translation);
}

Transform::Transform(float _val)
{
  /**
  * Rotation set.
  **/
  transform.matrix[0][0] = _val;
  transform.matrix[0][1] = _val;
  transform.matrix[0][2] = _val;

  transform.matrix[1][0] = _val;
  transform.matrix[1][1] = _val;
  transform.matrix[1][2] = _val;

  transform.matrix[2][0] = _val;
  transform.matrix[2][1] = _val;
  transform.matrix[2][2] = _val;

  /**
  * Translation set.
  **/
  transform.matrix[0][3] = _val;
  transform.matrix[1][3] = _val;
  transform.matrix[2][3] = _val;

  /**
  * Scale set.
  **/
  transform.matrix[3][0] = _val;
  transform.matrix[3][1] = _val;
  transform.matrix[3][2] = _val;
}

Matrix4
Transform::getRotation()
{
  Matrix4 M = Matrix4::IDENTITY;
  M.matrix[0][0] = transform.matrix[0][0];
  M.matrix[0][1] = transform.matrix[0][1];
  M.matrix[0][2] = transform.matrix[0][2];

  M.matrix[1][0] = transform.matrix[1][0];
  M.matrix[1][1] = transform.matrix[1][1];
  M.matrix[1][2] = transform.matrix[1][2];

  M.matrix[2][0] = transform.matrix[2][0];
  M.matrix[2][1] = transform.matrix[2][1];
  M.matrix[2][2] = transform.matrix[2][2];
  return M;
}

Matrix4
Transform::getScale()
{
  Matrix4 M = Matrix4::IDENTITY;
  M.matrix[0][0] = transform.matrix[3][0];
  M.matrix[1][1] = transform.matrix[3][1];
  M.matrix[2][2] = transform.matrix[3][2];
  return M;
}

Matrix4
Transform::getTranslation()
{
  Matrix4 M = Matrix4::IDENTITY;
  M.matrix[0][3] = transform.matrix[0][3];
  M.matrix[1][3] = transform.matrix[1][3];
  M.matrix[2][3] = transform.matrix[2][3];
  return M;
}

void
Transform::setRotation(Matrix4 _rotation)
{
  transform.matrix[0][0] = _rotation.matrix[0][0];
  transform.matrix[0][1] = _rotation.matrix[0][1];
  transform.matrix[0][2] = _rotation.matrix[0][2];

  transform.matrix[1][0] = _rotation.matrix[1][0];
  transform.matrix[1][1] = _rotation.matrix[1][1];
  transform.matrix[1][2] = _rotation.matrix[1][2];

  transform.matrix[2][0] = _rotation.matrix[2][0];
  transform.matrix[2][1] = _rotation.matrix[2][1];
  transform.matrix[2][2] = _rotation.matrix[2][2];
}

void
Transform::setScale(Matrix4 _scale)
{
  transform.matrix[3][0] = _scale.matrix[0][0];
  transform.matrix[3][1] = _scale.matrix[1][1];
  transform.matrix[3][2] = _scale.matrix[2][2];
}
void
Transform::setTranslation(Matrix4 _translation)
{
  transform.matrix[0][3] = _translation.matrix[0][3];
  transform.matrix[1][3] = _translation.matrix[1][3];
  transform.matrix[2][3] = _translation.matrix[2][3];
}
}