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
#include "pkTransform.h"
#include "pkPrerequisitesCore.h"

struct aiNodeAnim;

namespace pkEngineSDK
{

class Bone
{
 public:
  Bone() = default;
  Bone(const String& _name, int32 _ID, const aiNodeAnim* _channel);
  virtual ~Bone() = default;

  void
  update(float _deltaTime);

  /**
  * Interpolate position.
  * 
  * Gets the position matrix that should be in
  * the designated time.
  * 
  * @param _deltaTime
  * Time to search for the position.
  * 
  * @return
  * The position matrix.
  **/
  Matrix4
  interpolatePosition(float _deltaTime);

  /**
  * Interpolate rotation.
  *
  * Gets the rotation matrix that should be in
  * the designated time.
  *
  * @param _deltaTime
  * Time to search for the rotation.
  * 
  * @return
  * The rotation matrix.
  **/
  Matrix4
  interpolateRotation(float _deltaTime);

  /**
  * Interpolate scale.
  *
  * Gets the scale matrix that should be in
  * the designated time.
  *
  * @param _deltaTime
  * Time to search for the scale.
  * 
  * @return
  * The scale matrix.
  **/
  Matrix4
  interpolateScale(float _deltaTime);

  /**
  * Sets the local transform of the object.
  *
  * @param _transform
  * The new transform matrix.
  **/
  void
  setLocalTransform(const Transform& _transform) { m_transform = _transform; }

  /**
  * Gets the local transform of the object.
  * 
  * @return
  * The transform matrix.
  **/
  Transform
  getLocalTransform() { return m_transform; }

  /**
  * Gets the name of the bone.
  * 
  * @return
  * The name of the bone.
  **/
  String
  getBoneName() { return m_name; }

  /**
  * Set the name of the bone.
  * 
  * @param _name
  * The new ide of the bone.
  **/
  void
  setBoneName(String _name) { m_name = _name; }

  /**
  * Get the ID of the bone.
  * 
  * @return
  * The ID of the bone.
  **/
  int32
  getBoneID() { return m_ID; }

  /**
  * Sets the ID of the bone.
  * 
  * @param _id
  * The new bone ID.
  **/
  void
  setBoneID(int32 _id) { m_ID = _id; }


  uint32
  getPositionIndex(float _deltaTime);

  uint32
  getRotationIndex(float _deltaTime);

  uint32
  getScaleIndex(float _deltaTime);

  /**
  * Gets the scale that should be scaled between the last
  * timestamp and the next timestamp.
  * 
  * @param _lastTimeStamp
  * The last time stamp to check.
  * 
  * @param _nextTimeStamp
  * The next time stamp to check.
  * 
  * @param _deltaTime
  * How long has it been between frames.
  **/
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
  Transform m_transform = Transform(0);
};
}