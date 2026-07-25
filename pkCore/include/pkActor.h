/*****************************************************************************/
/**
 * @file    pkActor.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    22/01/2024
 * @brief   Actor for the scene in the engine.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"
#include "pkComponent.h"
#include "pkMatrix4.h"
#include "pkPlatformMath.h"
#include "pkQuaternion.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT Actor
{
 public:
  Actor(const String& _name = "", const Matrix4& _transform = Matrix4::IDENTITY) :
    m_transform(_transform), m_name(_name)
  {}
  ~Actor() {
    clear();
  }

  /**
   * @brief Clears all the data linked to the actor
   */
  void
  clear();

  /**
   * @brief Sets wether the object is active or not.
   * @param _active State of the actor.
   */
  void
  setActive(const bool& _active) { m_active = _active; }

  /**
   * @brief If the actor is active or not.
   * @return A true or false value of the actor activity.
   */
  bool&
  isActive() { return m_active; }

  /**
   * @brief Set the actor transform.
   * @param _transform Transform of the actor.
   */
  void
  setTransform(const Matrix4& _transform = Matrix4::IDENTITY);

  /**
   * @brief Set the position of the actor.
   * @param _translation Translation matrix.
   */
  PKFORCEINLINE void
  setPosition(const Matrix4& _translation) { setPosition(_translation.getTranslation3()); }
  
  /**
   * @brief Set the position of the actor.
   * @param _translation Translation vector.
   */
  PKFORCEINLINE void
  setPosition(const Vector3& _position) { setPosition(_position.x, _position.y, _position.z); }

  /**
   * @brief Set the position of the actor.
   * @param _x Translation in the X vector.
   * @param _y Translation in the Y vector.
   * @param _z Translation in the Z vector.
   */
  PKFORCEINLINE void
  setPosition(const float& _x, const float& _y, const float& _z)
  {
    m_position = Vector3(_x, _y, _z);
  }

  /**
   * @brief Modify the actor translation with a new value.
   * @param _addPos Vector with the positon increase values.
   */
  PKDEPRECATED PKFORCEINLINE void
  move(const Vector3& _addPos)
  {
    move(_addPos.x, _addPos.y, _addPos.z);
  }

  /**
   * @brief Modify the actor translation with a new value.
   * @param _addX Increase in the X axis.
   * @param _addY Increase in the Y axis.
   * @param _addZ Increase in the Z axis.
   */
  PKDEPRECATED PKFORCEINLINE void
  move(const float& _addX, const float& _addY, const float& _addZ)
  {
    m_position += Vector3(_addX, _addY, _addZ);
  }

  /**
   * @brief Move the actor using the verlet integration.
   * @param _direction Direction in which the object will move.
   * @param _force Force that the object will move with.
   */
  PKDEPRECATED void
  moveVerlet(const Vector3& _direction, const float& _force);
  
  /**
   * @brief Move the actor on the forward vector.
   * @param _offset How much will the movement be.
   */
  PKFORCEINLINE void
  moveForward(const float& _offset)
  {
    m_position += Vector3::FORWARD * _offset;
  }

  /**
   * @brief Move the actor relative to its forward vector.
   * @param _offset How much will the movement be.
   */
  PKFORCEINLINE void
  moveForwardLocal(const float& _offset)
  {
    m_position += m_forward * _offset;
  }

  /**
   * @brief Move the actor on the right vector.
   */
  PKFORCEINLINE void
  moveRight(const float& _offset)
  {
    m_position += Vector3::RIGHT * _offset;
  }

  /**
   * @brief Move the actor relative to its right vector.
   * @param _offset How much will the movement be.
   */
  PKFORCEINLINE void
  moveRightLocal(const float& _offset)
  {
    m_position += m_right * _offset;
  }

  /**
   * @brief Move the actor on the up vector.
   * @param _offset How much will the movement be.
   */
  PKFORCEINLINE void
  moveUp(const float& _offset)
  {
    m_position += Vector3::UP * _offset;
  }

  /**
   * @brief Move the actor relative to its up vector.
   * @param _offset How much will the movement be.
   */
  PKFORCEINLINE void
  moveUpLocal(const float& _offset)
  {
    m_position += m_up * _offset;
  }

  /**
   * @brief Get the actor position.
   * @return The vector position.
   */
  Vector3
  getPosition3() { return m_transform.getTranslation3(); }

  /**
   * @brief Set the rotation of the Actor.
   * @param _rotation Rotation Vector.
   */
  PKFORCEINLINE void
  setRotation(const Vector3& _rotation)
  {
    setRotation(_rotation.x, _rotation.y, _rotation.z);
  }

  /**
   * @brief Rotate the Actor.
   * @param _rotation Rotation Vector.
   */
  PKFORCEINLINE void
  rotate(const Vector3& _rotation);

  /**
   * @brief Set the rotation of the Actor.
   * @param _x Rotation in the X vector.
   * @param _y Rotation in the Y vector.
   * @param _z Rotation in the Z vector.
   */
  void
  setRotation(const float& _x, const float& _y, const float& _z);

  /**
   * @brief Rotate the Actor.
   * @param _x Rotation in the X vector.
   * @param _y Rotation in the Y vector.
   * @param _z Rotation in the Z vector.
   */
  void
  rotate(const float& _x, const float& _y, const float& _z);

  /**
   * @brief Set the scale of the Actor.
   * @param _scale Scale Matrix.
   */
  PKFORCEINLINE void
  setScale(const Matrix4& _scale) { setScale(_scale.getScale3()); }

  /**
   * @brief Set the scale of the Actor.
   * @param _val Scale in all axis.
   */
  PKFORCEINLINE void
  setScale(const float& _val) { setScale(_val, _val, _val); }

  /**
   * @brief Set the scale of the Actor.
   * @param _scale Scale Vector.
   */
  PKFORCEINLINE void
  setScale(const Vector3& _scale) { setScale(_scale.x, _scale.y, _scale.z); }

  /**
   * @brief Set the scale of the Actor.
   * @param _x Scale in the x.
   * @param _y Scale in the y.
   * @param _z Scale in the z.
   */
  void
  setScale(const float& _x, const float& _y, const float& _z);

  /**
   * @brief Get the actor scale.
   * @return The actor scale.
   */
  PKFORCEINLINE const Vector3
  getScale() const { return m_scale; }

  /**
   * @brief Get the actor rotation.
   * @return The actor rotation.
   */
#if PK_ROT == PK_ROT_QUATERNION
  PKFORCEINLINE Quaternion&
  getRotation() { return m_rotation; }
#endif
#if PK_ROT == PK_ROT_EULER
  PKFORCEINLINE const Vector3
  getRotation() const { return m_rotation; }
#endif

  /**
   * @brief Get the actor name.
   * @return Name of the actor
   */
  const String
  getName() const { return m_name; }

  /**
   * @brief Set the actor name.
   * @param _name Name of the actor
   */
  void
  setName(const String _name) { m_name = _name.c_str(); }

  /**
   * @brief Get the actor name as a const char*.
   * @return Name as a const char*
   */
  const ANSICHAR*
  getNameCSTR() const { return m_name.c_str(); }

  /**
   * @brief Update the actor.
   * @brief _deltaTime Time between frames.
   */
  void
  update(const float _deltaTime);

  /**
   * @brief Get a specific child of the actor.
   * @param _index Position of the desired child.
   * @return Pointer to the actor.
   */
  const SPtr<Actor>
  getChild(const uint32& _index = 0) const;

  /**
   * @brief Get the number of children of the actor.
   * @return Number of children.
   */
  uint32
  getChildCount() const { return toUint32(m_children.size()); }

  /**
   * @brief Get all the children of the actor.
   * @return Vector with all the children.
   */
  const Vector<SPtr<Actor>>&
  getChildren() const { return m_children; }

  /**
   * @brief Check if the actor has children.
   * @return True if the actor has children, false otherwise.
   */
  bool
  hasChildren() const { return !m_children.empty(); }

  /**
   * @brief Check if the actor has a parent.
   */
  bool
  hasParent() const { return (m_parent) ? true : false; }

  /**
   * @brief Set the parent of the actor.
   * @param _pActor Pointer to the parent actor.
   */
  void
  setParent(const SPtr<Actor>& _pActor) { m_parent = _pActor; }

  /**
   * @brief Get a pointer to the parent.
   * @return Pointer to the actor.
   */
  const SPtr<Actor>&
  getParent() const { return m_parent; }

  /**
   * @brief adds a component of type T.
   * @param _pComponent Pointer of the component to add.
   */
  void
  addComponent(const SPtr<Component>& _pComponent);

  /**
   * @brief Remove a component of the actor.
   * @param _index Position of the component to remove.
   */
  void
  removeComponent(const uint32& _index);

  /**
   * @brief Get the number of components of the actor.
   * @return Number of components.
   */
  uint32
  getComponentCount() const { return toUint32(m_components.size()); }

  /**
   * @brief Get a specific component of the actor.
   * @param _index Position of the desired component.
   * @return Pointer to the component.
   */
  const SPtr<Component>
  getComponent(const uint32& _index) const;

  /**
   * @brief Get the component of type T.
   * @return Returns the Pointer of type T from the components of the actor.
   */
  template <typename T>
  SPtr<T>
  getComponent() const {
    const uint32 compCount = getComponentCount();
    // for each component in the components vector.
    for (uint32 i = 0; i < compCount; ++i) {
      // if the conversion was successful, it means the component was found.
      SPtr<Component> comp = m_components[i];
      if (comp->getType() == T::getObjType()) {
        return reinterpret_pointer_cast<T>(comp);
      }
    }
    // otherwise, the component was not found, return a null pointer.
    return nullptr;
  }

  /**
   * @brief Get a vector with all the components of a specific type.
   * @return Vector of components.
   */
  template<typename T>
  Vector<SPtr<T>>
  getComponents() const {
    Vector<SPtr<T>> comps;
    for (auto& comp : m_components) {
      if (T::getObjType() == comp->getType()) {
        SPtr<T> obj = reinterpret_pointer_cast<T>(comp);
        comps.push_back(obj);
      }
    }
    return comps;
  }

  /**
   * @brief Get all the components of the actor.
   * @return Vector with all the components.
   */
  const Vector<SPtr<Component>>&
  getComponents() { return m_components; }

 private:
  /**
   * @brief Create a new transform for the actor based on translation, rotation
   *        and scale currently set.
   */
  void
  generateNewTransform();

  /**
   * @brief Create a new local transform for the actor based on translation, rotation
   *        and scale currently set.
   */
  void
  generateNewLocalTransform();

  /**
   * @brief Recalculate the forward, right and up vectors of the actor based on the rotation.
   */
  void
  recalculateDirections(const Quaternion& _rot);

  /**
   * @brief Recalculate the forward, right and up vectors of the actor based on the rotation.
   */
  void
  recalculateDirections(const Vector3& _rot);

 public:
  /**
   * Direction vectors. generated automatically based on the rotation of the actor.
   * They can be used for movement and other things, but they should not be modified directly.
   */
  Vector3 m_forward = Vector3::FORWARD;
  Vector3 m_right = Vector3::RIGHT;
  Vector3 m_up = Vector3::UP;

  /**
   * Transform info. used to generate the transform matrix.
   */
#if PK_ROT == PK_ROT_QUATERNION
  Quaternion m_rotation = Quaternion::IDENTITY;
#endif

#if PK_ROT == PK_ROT_EULER
  Vector3 m_rotation = Vector3::ZERO;
#endif

  Vector3 m_scale = Vector3(1.0f);
  Vector3 m_position = Vector3(0.0f);

  /**
   * Transform Matrix. generated automatically based on the position, rotation and scale
   * of the actor.
   */
  Matrix4 m_transform = Matrix4::IDENTITY;

 private:
  Matrix4 m_prevTransform = Matrix4::IDENTITY;
  String m_name = "";
  bool m_active = true;

  /**
   * Actor parts, these can be modified in runtime.
   */
  Vector<SPtr<Component>> m_components = {};
  Vector<SPtr<Actor>> m_children = {};
  SPtr<Actor> m_parent = nullptr;
};
}