/************************************************************************/
/**
* @pkBone pkBone.h
* @Hector Ivan Muñoz Ceballos
* @date 23/11/2024
* @Bone file for the Presk Game Engine.
*
* This file will contain the bone used for the engine
*
* @bug No bug known.
*
* @HectIvan 23/11/2024
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"
#include "pkTransform.h"

struct aiNodeAnim;

namespace pkEngineSDK
{

class Matrix4;

class Bone
{
 public:
  Bone() = default;
  Bone(const String& _name, int32 _ID, const aiNodeAnim* _channel);
  virtual ~Bone() = default;

  /**
   * @brief Update the bone.
   * @param _deltaTime Time between frames.
   */
  void
  update(float _deltaTime);

  /**
   * @brief Interpolate the position matrix that should be in
   * the designated time.
   * @param _deltaTime Time to search for the position.
   * @return The position matrix.
   */
  Matrix4
  interpolatePosition(float _deltaTime);

  /**
   * @brief Interpolate the rotation matrix that should be in
   * the designated time.
   * @param _deltaTime Time to search for the rotation.
   * @return The rotation matrix.
   */
  Matrix4
  interpolateRotation(float _deltaTime);

  /**
   * @brief Interpolate the scale matrix that should be in
   * the designated time.
   * @param _deltaTime Time to search for the scale.
   * @return The scale matrix.
   */
  Matrix4
  interpolateScale(float _deltaTime);

  /**
   * @brief Set the local transform of the object.
   * @param _transform The new transform matrix.
   */
  void
  setLocalTransform(const Transform& _transform) { m_transform = _transform; }

  /**
   * @brief Get the local transform of the object.
   * @return The transform matrix.
   */
  Transform
  getLocalTransform() { return m_transform; }

  /**
   * @brief Get the name of the bone.
   * @return The name of the bone.
   */
  String
  getBoneName() { return m_name; }

  /**
   * @brief Set the name of the bone.
   * @param _name The new ide of the bone.
   */
  void
  setBoneName(String _name) { m_name = _name; }

  /**
    * @brief Get the ID of the bone.
    * @return The ID of the bone.
    */
  int32
  getBoneID() { return m_ID; }

  /**
   * @brief Set the ID of the bone.
   * @param _id The new bone ID.
   */
  void
  setBoneID(int32 _id) { m_ID = _id; }

  uint32
  getPositionIndex(float _deltaTime);

  uint32
  getRotationIndex(float _deltaTime);

  uint32
  getScaleIndex(float _deltaTime);

  /**
   * @brief Get the scale that should be scaled between the last
   * timestamp and the next timestamp.
   * @param _lastTimeStamp The last time stamp to check.
   * @param _nextTimeStamp The next time stamp to check.
   * @param _deltaTime How long has it been between frames.
   */
  float
  getScaleFactor(float _lastTimeStamp,
                 float _nextTimeStamp,
                 float _deltaTime);
 
 public:
  Vector<KeyPosition> positions;
  Vector<KeyRotation> rotations;
  Vector<KeyScale> scales;
  uint32 numPositions;
  uint32 numRotations;
  uint32 numScales;

 private:
  int32 m_ID;
  String m_name;
  Transform m_transform;
};
}