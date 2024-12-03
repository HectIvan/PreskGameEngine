#include "pkGameObject.h"

namespace pkEngineSDK
{

void
GameObject::init(Transform _transform)
{
  m_transform = _transform;
}

void
GameObject::setPosition(Matrix4 _translation)
{
  m_transform.setTranslation(_translation);
}

void
GameObject::setPosition(Vector3 _position)
{
  m_transform.setTranslation(Matrix4::translation(_position));
}

void
GameObject::setPosition(float _x, float _y, float _z)
{
  m_transform.setTranslation(Matrix4::translation(_x, _y, _z));
}

void
GameObject::setRotation(Matrix4 _rotation)
{
  m_transform.setRotation(_rotation);
}

void
GameObject::setRotation(Vector3 _rotation)
{
  m_transform.setRotation(Matrix4::rotation(_rotation));
}

void
GameObject::setRotation(float _x, float _y, float _z)
{
  m_transform.setRotation(Matrix4::rotation(_x, _y, _z));
}

void
GameObject::setScale(Matrix4 _scale)
{
  m_transform.setScale(_scale);
}

void
GameObject::insertModel(Model* _model)
{
  m_models.push_back(_model);
}

void
GameObject::clear()
{
  // clean all models
  while(!m_models.empty())
  {
    m_models.pop_back();
  }
}
}