#include "pkGameObject.h"

namespace pkEngineSDK
{

void
GameObject::setTransform(Matrix4 _transform)
{
  transform = _transform;
}

void
GameObject::setPosition(Matrix4 _translation)
{
  transform = _translation;
}

void
GameObject::setPosition(Vector3 _position)
{
  transform = Matrix4::translation(_position);
}

void
GameObject::setPosition(float _x, float _y, float _z)
{
  transform = Matrix4::translation(_x, _y, _z);
}

void
GameObject::move(Vector3 _addPos)
{
  move(_addPos.x, _addPos.y, _addPos.z);
}

void
GameObject::move(float _addX, float _addY, float _addZ)
{
  // get the current transform matrix
  Matrix4 currentTranslation = transform;
  // add the extra position to the translation matrix
  currentTranslation.matrix[0][3] += _addX;
  currentTranslation.matrix[1][3] += _addY;
  currentTranslation.matrix[2][3] += _addZ;
  // set the current translation to the new translation
  transform = currentTranslation;
}

void
GameObject::setRotation(Matrix4 _rotation)
{
  transform.setRotation(_rotation);
}

void
GameObject::setRotation(Vector3 _rotation)
{
  transform.setRotation(Matrix4::rotation(_rotation));
}

void
GameObject::setRotation(float _x, float _y, float _z)
{
  transform.setRotation(Matrix4::rotation(_x, _y, _z));
}

void
GameObject::setScale(Matrix4 _scale)
{
  transform.setScale(_scale);
}

void
GameObject::insertModel(SPtr<Model> _pModel)
{
  models.push_back(_pModel);
}

SPtr<GameObject>
GameObject::getChild(uint32 _index)
{
  if (!children.empty() && _index < children.size())
  {
    return children[_index];
  }
  return nullptr;
}

void
GameObject::clear()
{
  // clean all models
  models.clear();
}

void
GameObject::addComponent(SPtr<Component> _pComponent)
{
  components.push_back(_pComponent);
}
}