#include "pkGameObject.h"

namespace pkEngineSDK
{

void
GameObject::init(Transform _transform)
{
  transform = _transform;
}

void
GameObject::setPosition(Matrix4 _translation)
{
  transform.setTranslation(_translation);
}

void
GameObject::setPosition(Vector3 _position)
{
  transform.setTranslation(Matrix4::translation(_position));
}

void
GameObject::setPosition(float _x, float _y, float _z)
{
  transform.setTranslation(Matrix4::translation(_x, _y, _z));
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

template <typename T>
SPtr<T>
GameObject::getComponent()
{
  // for each component in the components vector
  for (auto& comp : components) {
    // do a conversion to the data type T
    auto check = dynamic_pointer_cast<T>(comp);
    // if the conversion was successful, it means the component was found
    if (check) {
      return check;
    }
  }
  // otherwise, the component was not found, return a null pointer
  return nullptr;
}
}