/************************************************************************/
/**
* @pkTransform pkTransform.h
* @Hector Ivan Muñoz Ceballos
* @date 13/11/2024
* @Transform file for the Presk Game Engine.
*
* This file will contain the Transform used for the engine
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
#include "pkMatrix4.h"

namespace pkEngineSDK
{

class PK_UTILITY_EXPORT Transform
{
 public:
  Transform() = default;
  Transform(Matrix4 _rotation,
            Matrix4 _scale,
            Matrix4 _translation);
  explicit Transform(Matrix4 _matrix);
  explicit Transform(float _val);
  virtual ~Transform() = default;

  /**
  * Get rotation.
  * 
  * Gets the current transform rotation.
  * 
  * @return
  * The rotation matrix.
  **/
  Matrix4
  getRotation();

  /**
  * Get scale
  * 
  * Gets the current transform scale.
  * 
  * @retun
  * The scale matrix.
  **/
  Matrix4
  getScale();

  /**
  * Get translation.
  * 
  * Gets the current transform translation.
  * 
  * @return
  * The translation matrix.
  **/
  Matrix4
  getTranslation();

  /**
  * Set rotation
  * 
  * Sets the desired transform rotation.
  * 
  * @param _rotation
  * The desired rotation matrix.
  **/
  void
  setRotation(Matrix4 _rotation);

  /**
  * Set scale
  *
  * Sets the desired transform scale.
  *
  * @param _scale
  * The desired scale matrix.
  **/
  void
  setScale(Vector3 _scale);

  /**
   * Set translation
   *
   * Sets the desired transform translation.
   *
   * @param _translation
   * The desired translation matrix.
   */
  void
  setTranslation(Vector3 _translation);

 public:
  Vector3 position;
  Vector3 scale;
  Vector3 rotation;
  Matrix4 transform;
};
}
