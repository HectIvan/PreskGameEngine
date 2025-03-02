#include "pkTransform.h"

namespace pkEngineSDK
{

Transform::Transform(Matrix4 _rotation,
                     Matrix4 _scale,
                     Matrix4 _translation)
{
  setRotation(_rotation);
  setScale(_scale.getScale3());
  setTranslation(_translation.getTranslation3());
}

Transform::Transform(Matrix4 _matrix)
{
  setRotation(_matrix.getScale());
  setTranslation(_matrix.getTranslation3());
  setScale(_matrix.getScale3());
}

Transform::Transform(float _val)
{
  Transform(Matrix4(_val));
}

Matrix4
Transform::getRotation()
{
  return Matrix4(0);
}

Matrix4
Transform::getScale()
{
  return Matrix4(0);
}

Matrix4
Transform::getTranslation()
{
  return Matrix4(0);
}

void
Transform::setRotation(Matrix4 _rotation)
{
  transform.setRotation(_rotation);
}

void
Transform::setScale(Vector3 _scale)
{
  transform.setScale(_scale);
  scale = _scale;
}

void
Transform::setTranslation(Vector3 _translation)
{
  transform.setTranslation(_translation);
  position = _translation;
}
}