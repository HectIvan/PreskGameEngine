/************************************************************************/
/**
* @pkGameObject pkGameObject.h
* @Hector Ivan Muñoz Ceballos
* @date 13/11/2024
* @Game object file for the Presk Game Engine.
*
* This file will contain the Game object used by the engine
*
* @bug No bug known.
*
* @HectIvan 13/11/2024
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"
#include "pkMatrix4.h"
#include "pkModel.h"

namespace pkEngineSDK
{

class GameObject
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
  init(Transform _transform);

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
  insertModel(Model* _model);

  /**
  * Cleans the GameObject.
  **/
  void
  clear();

 public:
  Vector<Model*> m_models;
  Transform m_transform;
};
}