/************************************************************************/
/**
* @pkGameObject pkGameObject.h
* @Hector Ivan Muñoz Ceballos
* @date 13/11/2024
* @Game object file for the Presk Game Engine.
*
* This file will contain the Game object used by the engine
*
* @bug No known bugs.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkComponent.h"
#include "pkMatrix4.h"
#include "pkModel.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT GameObject
{
 public:
  GameObject() = default;
  virtual ~GameObject() = default;

  /**
  * Initialize the Game Object.
  * 
  * @param _transform
  * Transform of the Game Object.
  **/
  void
  setTransform(Transform _transform);

  /**
  * Set the position of the Game Object.
  * 
  * @param _translation
  * Translation matrix.
  **/
  void
  setPosition(Matrix4 _translation);
  
  /**
  * Set the position of the Game Object.
  *
  * @param _translation
  * Translation vector.
  **/
  void
  setPosition(Vector3 _position);

  /**
  * Set the position of the Game Object.
  *
  * @param _x
  * Translation in the X vector.
  * 
  * @param _y
  * Translation in the Y vector.
  * 
  * @param _z
  * Translation in the Z vector.
  **/
  void
  setPosition(float _x, float _y, float _z);

  /**
  * Modify the game object translation with a new value.
  * 
  * @param _addPos
  * Vector with the positon increase values.
  **/
  void
  move(Vector3 _addPos);

  /**
  * Modify the game object translation with a new value.
  * 
  * @param _addX
  * increase in the X axis.
  * 
  * @param _addY
  * Increase in the Y axis.
  * 
  * @param _addZ
  * Increase in the Z axis.
  **/
  void
  move(float _addX, float _addY, float _addZ);

  /**
  * Set the rotation of the GameObject.
  * 
  * @param _rotation
  * Rotation Matrix.
  **/
  void
  setRotation(Matrix4 _rotation);

  /**
  * Set the rotation of the GameObject.
  *
  * @param _rotation
  * Rotation Vector.
  **/
  void
  setRotation(Vector3 _rotation);

  /**
  * Set the rotation of the GameObject.
  *
  * @param _x
  * Rotation in the X vector.
  * 
  * @param _y
  * Rotation in the Y vector.
  * 
  * @param _z
  * Rotation in the Z vector.
  **/
  void
  setRotation(float _x, float _y, float _z);

  /**
  * Set the scale of the Game Object.
  * 
  * @param _scale
  * Scale Matrix.
  **/
  void
  setScale(Matrix4 _scale);

  /**
  * Inserts a new model into the model vector.
  * 
  * @param _model
  * Insert the new model.
  **/
  void
  insertModel(SPtr<Model> _pModel);
  
  /**
  * adds a component of type T.
  * 
  * @param _pComponent
  * Shared pointer of the component to add.
  **/
  void
  addComponent(SPtr<Component> _pComponent);

  /**
  * Get the component of type T.
  * 
  * @return
  * Returns the shared pointer of type T from the
  * components of the game object.
  **/
  template <typename T>
  SPtr<T>
  getComponent()
  {
    // for each component in the components vector
    for (auto& comp : components) {
      // if the conversion was successful, it means the component was found
      if (comp->getType() == T::getObjType()) {
        return reinterpret_pointer_cast<T>(comp);
      }
    }
    // otherwise, the component was not found, return a null pointer
    return nullptr;
  }

  /**
  * Get a specific child of the game object.
  * 
  * @param _index
  * Position of the desired child.
  * 
  * @return
  * Pointer to the GameObject
  **/
  SPtr<GameObject>
  getChild(uint32 _index = 0);

  /**
  * Get a pointer to the parent
  **/
  SPtr<GameObject>
  getParent() { return parent; }

  /**
  * Cleans the GameObject.
  **/
  void
  clear();

 public:
  Vector<SPtr<Model>> models;
  Vector<SPtr<Component>> components;
  Vector<SPtr<GameObject>> children;
  SPtr<GameObject> parent;
  Transform transform;
};
}