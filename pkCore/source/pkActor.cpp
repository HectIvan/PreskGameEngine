#include "pkActor.h"

namespace pkEngineSDK
{
  
void
Actor::setTransform(Matrix4 _transform)
{
  m_transform = _transform;
}

void
Actor::setPosition(Matrix4 _translation)
{
  m_transform = _translation;
}

void
Actor::setPosition(Vector3 _position)
{
  m_transform = Matrix4::translation(_position);
}

void
Actor::setPosition(float _x, float _y, float _z)
{
  m_transform = Matrix4::translation(_x, _y, _z);
}

void
Actor::move(Vector3 _addPos)
{
  move(_addPos.x, _addPos.y, _addPos.z);
}

void
Actor::move(float _addX, float _addY, float _addZ)
{
  // get the current transform matrix
  Matrix4 currentTranslation = m_transform;
  // add the extra position to the translation matrix
  currentTranslation.matrix[0][3] += _addX;
  currentTranslation.matrix[1][3] += _addY;
  currentTranslation.matrix[2][3] += _addZ;
  // set the current translation to the new translation
  m_transform = currentTranslation;
}

void
Actor::setRotation(Matrix4 _rotation)
{
  m_transform.setRotation(_rotation);
}

void
Actor::setRotation(Vector3 _rotation)
{
  m_transform.setRotation(Matrix4::rotation(_rotation));
}

void
Actor::setRotation(float _x, float _y, float _z)
{
  m_transform.setRotation(Matrix4::rotation(_x, _y, _z));
}

void
Actor::setScale(Matrix4 _scale)
{
  m_transform.setScale(_scale);
}

SPtr<Actor>
Actor::getChild(uint32 _index)
{
  if (!m_children.empty() && _index < m_children.size())
  {
    return m_children[_index];
  }
  return nullptr;
}

void
Actor::addComponent(SPtr<Component> _pComponent)
{
  m_components.push_back(_pComponent);
}
}