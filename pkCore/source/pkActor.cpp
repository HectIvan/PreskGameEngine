/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkActor.h"
#include "pkCamera.h"
#include "pkPlatformMath.h"
#include "pkLight.h"
#include "pkLogger.h"

namespace pkEngineSDK
{

Actor::Actor()
{
  setActive(true);
  m_forward = Vector3::FORWARD;
  m_right = Vector3::RIGHT;
  m_up = Vector3::UP;
  m_scale = Vector3(1.0f);
  m_position = Vector3(0.0f);
  m_rotation = Quaternion(0.0f);
  m_transform = Matrix4::IDENTITY;
}

void
Actor::setTransform(const Matrix4& _transform)
{
  m_transform = _transform;
}

void
Actor::setPosition(Matrix4& _translation)
{
  setPosition(_translation.getTranslation3());
}

void
Actor::setPosition(const Vector3& _position)
{
  setPosition(_position.x, _position.y, _position.z);
}

void
Actor::setPosition(const float& _x, const float& _y, const float& _z)
{
  m_position = Vector3(_x, _y, _z);
  generateNewTransform();
}

void
Actor::move(const Vector3& _addPos)
{
  move(_addPos.x, _addPos.y, _addPos.z);
}

void
Actor::moveForward(const float& _offset)
{
  move(Vector3::FORWARD * _offset);
}

void
Actor::moveForwardLocal(const float& _offset)
{
  Vector3 newOffset = m_transform.getForwardVector() * _offset;
  move(newOffset);
}

void
Actor::moveRight(const float& _offset)
{
  move(Vector3::RIGHT * _offset);
}

void
Actor::moveRightLocal(const float& _offset)
{
  Vector3 newOffset = m_transform.getRightVector() * _offset;
  move(newOffset);
}

void
Actor::moveUp(const float& _offset)
{
  move(Vector3::UP * _offset);
}

void
Actor::moveUpLocal(const float& _offset)
{
  Vector3 newOffset = m_transform.getUpVector() * _offset;
  move(newOffset);
}

void
Actor::moveVerlet(const Vector3& _direction, const float& _force)
{
  m_transform.setTranslation((m_transform.getTranslation3() * 2) -
                              m_prevTransform.getTranslation3() + (_direction * _force));
}

void
Actor::move(const float& _addX, const float& _addY, const float& _addZ)
{
  m_position += Vector3(_addX, _addY, _addZ);
  generateNewTransform();
}

void
Actor::setRotation(const Vector3& _rotation)
{
  setRotation(_rotation.x, _rotation.y, _rotation.z);
}

void
Actor::setRotation(const float& _x, const float& _y, const float& _z)
{
  // m_rotation = Vector3(_x, _y, _z);
  // Matrix4 rotMat = Matrix4::rotation(_x, _y, _z);

  Vector3 rot = Vector3(_x, _y, _z);

  m_rotation = Quaternion::fromEuler(rot).normalized();
  Matrix4 rotMat = Matrix4::rotation(rot);
  
  // ---------------------------------------------------------------
  m_forward = (rotMat * Vector4::FORWARD).xyz().normalized();
  m_right = (rotMat * Vector4::RIGHT).xyz().normalized();
  m_up = (rotMat * Vector4::UP).xyz().normalized();

  generateNewTransform();
}

void
Actor::rotate(const Vector3& _rotation)
{
  rotate(_rotation.x, _rotation.y, _rotation.z);
}

void
Actor::rotate(const float& _x, const float& _y, const float& _z)
{
  const Vector3 addRot = Vector3(_x, _y, _z);

  m_rotation *= Quaternion::fromEuler(addRot);
  m_rotation.normalize();
  Matrix4 rotMat = Matrix4::rotation(m_rotation);

  m_forward = (rotMat * Vector4::FORWARD).xyz().normalized();
  m_right = (rotMat * Vector4::RIGHT).xyz().normalized();
  m_up = (rotMat * Vector4::UP).xyz().normalized();

  generateNewTransform();
}

void
Actor::setScale(Matrix4& _scale)
{
  setScale(_scale.getScale3());
}

void
Actor::setScale(const float& _val) {
  setScale(_val, _val, _val);
}

void
Actor::setScale(const Vector3& _scale)
{
  setScale(_scale.x, _scale.y, _scale.z);
}

void
Actor::setScale(const float& _x, const float& _y, const float& _z)
{
  // make sure scale is never 0
  float x = _x;
  float y = _y;
  float z = _z;
  if (_x == 0.0f) { x = Math::SMALL_NUMBER; }
  if (_y == 0.0f) { y = Math::SMALL_NUMBER; }
  if (_z == 0.0f) { z = Math::SMALL_NUMBER; }
  m_scale = Vector3(x, y, z);

  generateNewTransform();
}

void
Actor::update(float)
{
  const uint32 compCount = static_cast<uint32>(m_components.size());
  for (uint32 i = 0; i < compCount; ++i) {
    m_components[i]->update(*this);
  }
}

SPtr<Actor>
Actor::getChild(const uint32 _index)
{
  // if there are children and the index is inside the range of existing children.
  if (!m_children.empty() && _index < m_children.size()) {
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
Actor::generateNewTransform()
{
  const Quaternion rot = m_rotation;

  Matrix4 posMat = Matrix4::translation(m_position);
  Matrix4 rotMat = Matrix4::rotation(rot);
  Matrix4 scaleMat = Matrix4::scale(m_scale);

  m_transform = posMat * rotMat * scaleMat;
}

void
Actor::generateNewLocalTransform()
{
  const Quaternion rot = m_rotation;

  const Matrix4 posMat = Matrix4::translation(m_position);
  const Matrix4 rotMat = Matrix4::rotation(rot);
  const Matrix4 scaleMat = Matrix4::scale(m_scale);

  m_transform = scaleMat * rotMat * posMat;
}

void
Actor::setPositionLocal(const Vector3& _offset)
{
  m_position = _offset;
  generateNewLocalTransform();
}

void
Actor::setPositionForwardLocal(const float& _offset)
{
  const Vector3 offset = m_forward * _offset;
  m_position += offset;
  generateNewLocalTransform();
}

void
Actor::addComponent(const SPtr<Component>& _pComponent)
{
  // if the component exists, insert it into the list.
  if (_pComponent) {
    m_components.push_back(_pComponent);
    // log registry.
    const String msg = "Inserted Actor component: " + String(_pComponent->getName());
    LOG_REGISTER(msg, __FILE__, __LINE__);
  }
}
}