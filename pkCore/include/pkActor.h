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
  getPosition3();

  /**
   * @brief get the position taking into account the parent global.
   */
  Vector3
  getPosition3Global();

  /**
   * @brief Set the rotation of the GameObject.
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
   * @param _scale Scale Vector.
   */
  void
  setScale(Vector3 _scale);

  /**
   * @brief Set the scale of the Actor.
   * @param _x Scale in the x.
   * @param _x Scale in the y.
   * @param _x Scale in the z.
   */
  void
  setScale(float _x, float _y, float _z);

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
  bool
  isActive() { return m_active; }

  /**
   * @brief Clears all the data linked to the actor
   */
  void
  clear();

 public:
  bool m_active = true;
  Vector3 m_forward;
  Vector<SPtr<Component>> m_components;
  Vector<SPtr<Actor>> m_children;
  SPtr<Actor> m_parent;
  Matrix4 m_transform;
  Matrix4 m_prevTransform;
  Matrix4 m_globalTransform;
  String m_name;
};
}