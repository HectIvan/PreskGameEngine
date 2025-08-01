/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkActor.h"
#include "pkPlatformMath.h"
#include "pkLight.h"
#include "pkLogger.h"

namespace pkEngineSDK
{

Actor::Actor()
{
  setActive(true);
  m_forward = Vector3::FORWARD;
  m_scale = Vector3(1.0f);
  m_position = Vector3(0.0f);
  m_rotation = Vector3(0.0f);
}

void
Actor::setTransform(Matrix4 _transform)
{
  m_transform = _transform;
}

void
Actor::setPosition(Matrix4 _translation)
{
  m_transform.setTranslation(_translation.getTranslation3());
}

void
Actor::setPosition(Vector3 _position)
{
  m_position = _position;
  m_transform.setTranslation(_position);
}

void
Actor::move(Vector3 _addPos)
{
  move(_addPos.x, _addPos.y, _addPos.z);
}

void
Actor::moveLocal(Vector3 _offset)
{

}

void
Actor::moveVerlet(Vector3 _direction, float _force)
{
  m_transform.setTranslation((m_transform.getTranslation3() * 2) -
                              m_prevTransform.getTranslation3() + (_direction * _force));
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
  setRotation(_rotation.x, _rotation.y, _rotation.z);
}

void
Actor::setRotation(float _x, float _y, float _z)
{
  m_rotation = Vector3(_x, _y, _z);

  // reset to default scale
  m_transform.matrix[0][0] /= m_scale.x;
  m_transform.matrix[1][1] /= m_scale.y;
  m_transform.matrix[2][2] /= m_scale.z;

  _x *= Math::DEG2RAD;
  _y *= Math::DEG2RAD;
  _z *= Math::DEG2RAD;
  m_transform.setRotation(Matrix4::rotation(_x, _y, _z));

  // return to set scale
  m_transform.matrix[0][0] *= m_scale.x;
  m_transform.matrix[1][1] *= m_scale.y;
  m_transform.matrix[2][2] *= m_scale.z;
}

void
Actor::setPosition(float _x, float _y, float _z)
{
  m_transform.setTranslation(_x, _y, _z);
  m_position = Vector3(_x, _y, _z);
}

void
Actor::setScale(Matrix4 _scale)
{
  m_transform.setScale(_scale);
}

void
Actor::setScale(float _val) {
  setScale(_val, _val, _val);
}

void
Actor::setScale(Vector3 _scale)
{
  setScale(_scale.x, _scale.y, _scale.z);
}

void
Actor::setScale(float _x, float _y, float _z)
{
  // make sure scale is never 0
  if (_x == 0.0f) { _x = Math::SMALL_NUMBER; }
  if (_y == 0.0f) { _y = Math::SMALL_NUMBER; }
  if (_z == 0.0f) { _z = Math::SMALL_NUMBER; }

  m_transform.matrix[0][0] /= m_scale.x;
  m_transform.matrix[1][1] /= m_scale.y;
  m_transform.matrix[2][2] /= m_scale.z;
  m_transform.setScale(_x, _y, _z);
  m_scale = Vector3(_x, _y, _z);
}

void
Actor::update(float _deltaTime)
{
  for (uint32 i = 0; i < m_components.size(); ++i) {
    switch (m_components[i]->getType()) {
      case COMPONENT_TYPE::kLight: {
        SPtr<Light> light = reinterpret_pointer_cast<Light>(m_components[i]);
        light->m_position = getPosition3();
        light->m_transform = m_transform;
        break;
      }
      case COMPONENT_TYPE::kCamera: {
        break;
      }
      case COMPONENT_TYPE::kMaterial: {
        break;
      }
      case COMPONENT_TYPE::kModel: {
        break;
      }
      case COMPONENT_TYPE::kUnknown: {
        break;
      }
      default: {
        break;
      }
    }
  }
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
Actor::clear()
{
  m_components.clear();
  m_children.clear();
  if (m_parent) { m_parent->clear(); }
  m_transform = Matrix4::IDENTITY;
  m_name = "";
}

void
Actor::addComponent(SPtr<Component> _pComponent)
{
  if (_pComponent) {
    m_components.push_back(_pComponent);
  }
}
}