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

void
Actor::setTransform(const Matrix4& _transform)
{
  m_transform = _transform;

  // m_forward = m_transform.getForwardVector().normalized();
  // m_right = m_transform.getRightVector().normalized();
  // m_up = m_transform.getUpVector().normalized();
  // 
  // m_position = m_transform.getTranslation3();
  // m_rotation = Quaternion::fromRotationMatrix(m_transform);
  // m_scale = m_transform.getScale3();
}

void
Actor::moveVerlet(const Vector3& _direction, const float& _force)
{
  m_transform.setTranslation((m_transform.getTranslation3() * 2) -
                              m_prevTransform.getTranslation3() + (_direction * _force));
}

void
Actor::setRotation(const float& _x, const float& _y, const float& _z)
{
  // Create and normalize rotation.
  m_rotation = Quaternion::fromEuler(_x, _y, _z).normalized();

  // verify that the rotation does not contain NaN values.
  if (m_rotation.hasNan()) {
    LOG_ERROR("Rotation contains NaN values. Resetting to identity.", __FILE__, __LINE__);
    m_rotation = Quaternion::IDENTITY;
  }

  // Rotate basis vectors directly using quaternion.
  recalculateDirections();
}

void
Actor::rotate(const float& _x, const float& _y, const float& _z)
{
  // world up rotation.
  const Vector3 localUp = m_rotation * Vector3::UP;
  const Quaternion yaw = Quaternion::fromAxisAngle(localUp, _x);
  m_rotation = yaw * m_rotation;

  // local right rotation.
  const Vector3 localRight = m_rotation * Vector3::RIGHT;
  const Quaternion pitch = Quaternion::fromAxisAngle(localRight, _y);
  m_rotation = pitch * m_rotation;

  // local forward rotation.
  const Vector3 localForward = m_rotation * Vector3::FORWARD;
  const Quaternion roll = Quaternion::fromAxisAngle(localForward, _z);
  m_rotation = roll * m_rotation;

  m_rotation.normalize();

  // recalculate directions.
  recalculateDirections();
}

void
Actor::setScale(const float& _x, const float& _y, const float& _z)
{
  // make sure scale is never 0.
  float x = _x;
  float y = _y;
  float z = _z;
  if (_x == 0.0f) { x = Math::SMALL_NUMBER; }
  if (_y == 0.0f) { y = Math::SMALL_NUMBER; }
  if (_z == 0.0f) { z = Math::SMALL_NUMBER; }

  m_scale = Vector3(x, y, z);
}

void
Actor::update(float)
{
  const uint32 compCount = static_cast<uint32>(m_components.size());
  for (uint32 i = 0; i < compCount; ++i) {
    m_components[i]->update(*this);
  }
  generateNewTransform();
}

const SPtr<Actor>
Actor::getChild(const uint32& _index) const
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
  const uint32 childCount = getChildCount();

  for (uint32 i = 0; i < childCount; ++i) {
    m_children[i]->clear();
  }
  m_children.clear();
  m_components.clear();

  m_transform = Matrix4::IDENTITY;
  m_name = "";
  m_parent = nullptr;

  // assert that both vectors are now empty.
  PK_ASSERT(m_children.empty() && "Failed to clear children.");
  PK_ASSERT(m_components.empty() && "Failed to clear components.");
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

void
Actor::removeComponent(const uint32& _index)
{
  // if there are no components or the index is out of range.
  if (m_components.empty() || _index >= getComponentCount()) {
    LOG_ERROR("Tried to remove a component with an index that is out of range.",
              __FILE__,
              __LINE__);
    return;
  }

  m_components.erase(m_components.begin() + _index);
}

const SPtr<Component>
Actor::getComponent(const uint32& _index) const
{
  const uint32 compCount = getComponentCount();

  // if there are no components or the index is out of range, return a null pointer.
  if (m_components.empty() || _index >= compCount) {
    LOG_ERROR("Tried to get a component with an index that is out of range.", __FILE__, __LINE__);
    return nullptr;
  }

  // search for the component in the components vector and return it if found.
  for (uint32 i = 0; i < compCount; ++i) {
    if (i == _index) {
      return m_components[i];
    }
  }
  return nullptr;
}

void
Actor::generateNewTransform()
{
  const Matrix4 posMat = Matrix4::translation(m_position);
  const Matrix4 rotMat = Matrix4::rotation(m_rotation);
  const Matrix4 scaleMat = Matrix4::scale(m_scale);

  m_transform = posMat * rotMat * scaleMat;
}

void
Actor::generateNewLocalTransform()
{
  const Matrix4 posMat = Matrix4::translation(m_position);
  const Matrix4 rotMat = Matrix4::rotation(m_rotation);
  const Matrix4 scaleMat = Matrix4::scale(m_scale);

  m_transform = scaleMat * rotMat * posMat;
}

void
Actor::recalculateDirections()
{
  // recalculate directions.
  m_forward = m_rotation.rotate(Vector3::FORWARD).normalized();
  m_right   = m_rotation.rotate(Vector3::RIGHT).normalized();
  m_up      = m_rotation.rotate(Vector3::UP).normalized();
}

}