/************************************************************************/
/**
* @pkFrustum pkFrustum.h
* @Hector Ivan Muñoz Ceballos
* @date 22/01/2025
* @Frustum file for the Presk Game Engine.
*
* This file will contain the Frustum class used for the engine
*
* @bug No current function.
*
* @HectIvan 22/01/20245
* File Creation.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"
#include "pkComponent.h"
#include "pkMatrix4.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT Actor
{
 public:
  Actor();
  virtual ~Actor() {
    clear();
  }

  /**
   * @brief Set the actor transform.
   * @param _transform Transform of the actor.
   */
  void
  setTransform(Matrix4 _transform);

  /**
   * @brief Set the position of the actor.
   * @param _translation Translation matrix.
   */
  void
  setPosition(Matrix4 _translation);
  
  /**
   * @brief Set the position of the actor.
   * @param _translation Translation vector.
   */
  void
  setPosition(Vector3 _position);

  /**
   * @brief Set the position of the actor.
   * @param _x Translation in the X vector.
   * @param _y Translation in the Y vector.
   * @param _z Translation in the Z vector.
   */
  void
  setPosition(float _x, float _y, float _z);

  /**
   * @brief Modify the actor translation with a new value.
   * @param _addPos Vector with the positon increase values.
   */
  void
  move(Vector3 _addPos);

  /**
   * @brief Move the actor relative to its local axis.
   */
  void
  moveLocal(Vector3 _offset);

  /**
   * @brief Move the actor using the verlet integration.
   * @param _direction Direction in which the object will move.
   * @param _force Force that the object will move with.
   */
  void
  moveVerlet(Vector3 _direction, float _force);

  /**
   * @brief Modify the actor translation with a new value.
   * @param _addX Increase in the X axis.
   * @param _addY Increase in the Y axis.
   * @param _addZ Increase in the Z axis.
   */
  void
  move(float _addX, float _addY, float _addZ);
  
  /**
   * @brief Get the actor position.
   * @return The vector position.
   */
  Vector3
  getPosition3() { return m_transform.getTranslation3(); }

  /**
   * @brief Set the rotation of the Actor.
   * @param _rotation Rotation Matrix.
   */
  void
  setRotation(Matrix4 _rotation);

  /**
   * @brief Set the rotation of the Actor.
   * @param _rotation Rotation Vector.
   */
  void
  setRotation(Vector3 _rotation);

  /**
   * @brief Set the rotation of the Actor.
   * @param _x Rotation in the X vector.
   * @param _y Rotation in the Y vector.
   * @param _z Rotation in the Z vector.
   */
  void
  setRotation(float _x, float _y, float _z);

  /**
   * @brief Set the scale of the Actor.
   * @param _scale Scale Matrix.
   */
  void
  setScale(Matrix4 _scale);

  /**
   * @brief Set the scale of the Actor.
   * @param _val Scale in all axis.
   */
  void
  setScale(float _val);

  /**
   * @brief Set the scale of the Actor.
   * @param _scale Scale Vector.
   */
  void
  setScale(Vector3 _scale);

  /**
   * @brief Set the scale of the Actor.
   * @param _x Scale in the x.
   * @param _y Scale in the y.
   * @param _z Scale in the z.
   */
  void
  setScale(float _x, float _y, float _z);

  /**
   * @brief Get the actor scale.
   * @return The actor scale.
   */
  FORCEINLINE Vector3&
  getScale() { return m_scale; }

  /**
   * @brief Get the actor rotation.
   * @return The actor rotation.
   */
  FORCEINLINE Vector3&
  getRotation() { return m_rotation; }

  /**
   * @brief adds a component of type T.
   * @param _pComponent Pointer of the component to add.
   */
  void
  addComponent(SPtr<Component> _pComponent);

  /**
   * @brief Get the component of type T.
   * @return Returns the Pointer of type T from the components of the actor.
   */
  template <typename T>
  SPtr<T>
  getComponent()
  {
    // for each component in the components vector
    for (auto& comp : m_components) {
      // if the conversion was successful, it means the component was found
      if (comp->getType() == T::getObjType()) {
        return reinterpret_pointer_cast<T>(comp);
      }
    }
    // otherwise, the component was not found, return a null pointer
    return nullptr;
  }

  /**
   * @brief Get all the components of the actor.
   * @return Vector with all the components.
   */
  Vector<SPtr<Component>>&
  getComponents() { return m_components; }

  /**
   * @brief Get the actor name.
   * @return Name of the actor
   */
  String&
  getName() { return m_name; }

  /**
   * @brief Set the actor name.
   * @param _name Name of the actor
   */
  void
  setName(String _name) { m_name = _name.c_str(); }

  /**
   * @brief Get the actor name as a const char*.
   * @return Name as a const char*
   */
  const char*
  getNameCSTR() { return m_name.c_str(); }

  /**
   * @brief Update the actor.
   * @brief _deltaTime Time between frames.
   */
  void
  update(float _deltaTime);

  /**
   * @brief Get a specific child of the actor.
   * @param _index Position of the desired child.
   * @return Pointer to the actor.
   */
  SPtr<Actor>
  getChild(uint32 _index = 0);

  /**
   * @brief Get a pointer to the parent.
   * @return Pointer to the actor.
   */
  SPtr<Actor>
  getParent() { return m_parent; }

  /**
   * @brief Sets wether the object is active or not.
   * @param _active State of the actor.
   */
  void
  setActive(bool _active) { m_active = _active; }

  /**
   * @brief If the actor is active or not.
   * @return A true or false value of the actor activity.
   */
  bool&
  isActive() { return m_active; }

  /**
   * @brief Clears all the data linked to the actor
   */
  void
  clear();

 private:
  void
  generateNewTransform();

 public:
  Vector3 m_forward;
  Vector<SPtr<Component>> m_components;
  Vector<SPtr<Actor>> m_children;
  SPtr<Actor> m_parent;
  Matrix4 m_transform;
  Matrix4 m_prevTransform;

  Vector3 m_scale;
  Vector3 m_rotation;
  Vector3 m_position;

 private:
  bool m_active = true;
  String m_name;
};
}