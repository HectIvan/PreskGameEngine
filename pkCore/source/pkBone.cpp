#include "pkBone.h"
#include "pkPlatformMath.h"

namespace pkEngineSDK
{
Bone::Bone(const String& _name, int32 _ID, const aiNodeAnim* _channel)
{
  setBoneName(_name);
  setBoneID(_ID);
  setLocalTransform(Transform(1.0f));

  // get all the positions
  m_numPositions = _channel->mNumPositionKeys;
  for (uint32 i = 0; i < m_numPositions; ++i)
  {
    Vector3 position = Vector3(_channel->mPositionKeys[i].mValue.x,
                               _channel->mPositionKeys[i].mValue.y,
                               _channel->mPositionKeys[i].mValue.z);
    float timeStamp = _channel->mPositionKeys[i].mTime;
    KeyPosition data;
    data.position = position;
    data.timeStamp = timeStamp;
    m_positions.push_back(data);
  }

  // get all the rotations
  m_numRotations = _channel->mNumRotationKeys;
  for (uint32 i = 0; i < m_numRotations; ++i)
  {
    Vector4 rotation = Vector4(_channel->mRotationKeys[i].mValue.x,
                               _channel->mRotationKeys[i].mValue.y,
                               _channel->mRotationKeys[i].mValue.z,
                               _channel->mRotationKeys[i].mValue.w);
    float timeStamp = _channel->mRotationKeys[i].mTime;
    KeyRotation data;
    data.rotation = rotation;
    data.timeStamp = timeStamp;
    m_rotations.push_back(data);
  }

  // get all the scales
  m_numScales = _channel->mNumScalingKeys;
  for (uint32 i = 0; i < m_numScales; ++i)
  {
    Vector3 scale = Vector3(_channel->mScalingKeys[i].mValue.x,
                            _channel->mScalingKeys[i].mValue.y,
                            _channel->mScalingKeys[i].mValue.z);
    float timeStamp = _channel->mRotationKeys[i].mTime;
    KeyScale data;
    data.scale = scale;
    data.timeStamp = timeStamp;
    m_scales.push_back(data);
  }
}

void
Bone::update(float _deltaTime)
{
  Matrix4 translation = interpolatePosition(_deltaTime);
  Matrix4 rotation = interpolateRotation(_deltaTime);
  Matrix4 scale = interpolateScale(_deltaTime);

  setLocalTransform(Transform(rotation, scale, translation));
}

Matrix4
Bone::interpolatePosition(float _deltaTime)
{
  if (1 == m_numPositions)
  {
    return Matrix4::translation(m_positions[0].position);
  }

  int p0Index = getPositionIndex(_deltaTime);
  int p1Index = p0Index + 1;
  float scaleFactor = getScaleFactor(m_positions[p0Index].timeStamp,
                                     m_positions[p1Index].timeStamp,
                                     _deltaTime);
  Vector3 finalPosition = PlatformMath::lerp3(m_positions[p0Index].position,
                                              m_positions[p1Index].position,
                                              scaleFactor);
  return Matrix4::translation(finalPosition);
}

Matrix4
Bone::interpolateRotation(float _deltaTime)
{
  if (1 == m_numRotations)
  {
    Vector4 rotation = m_rotations[0].rotation;
    rotation.normalize();
    return Matrix4::rotation(rotation.x, rotation.y, rotation.z);
  }

  int p0Index = getRotationIndex(_deltaTime);
  int p1Index = p0Index + 1;
  float scaleFactor = getScaleFactor(m_rotations[p0Index].timeStamp,
                                     m_rotations[p1Index].timeStamp,
                                     _deltaTime);
  Vector4 finalRotation = PlatformMath::lerp4(m_rotations[p0Index].rotation,
                                              m_rotations[p1Index].rotation,
                                              scaleFactor);
  finalRotation.normalize();
  return Matrix4::rotation(finalRotation.x, finalRotation.y, finalRotation.z);
}

Matrix4
Bone::interpolateScale(float _deltaTime)
{
  if (1 == m_numScales)
  {
    return Matrix4::scale(m_scales[0].scale);
  }
  int p0Index = getScaleIndex(_deltaTime);
  int p1Index = p0Index + 1;
  float scaleFactor = getScaleFactor(m_scales[p0Index].timeStamp,
                                     m_scales[p1Index].timeStamp,
                                     _deltaTime);
  Vector3 finalScale = PlatformMath::lerp3(m_scales[p0Index].scale,
                                           m_scales[p1Index].scale,
                                           scaleFactor);
  return Matrix4::scale(finalScale);
}

uint32
Bone::getPositionIndex(float _deltaTime)
{
  for (uint32 i = 0; i < m_numPositions - 1; ++i)
  {
    if (_deltaTime < m_positions[i + 1].timeStamp)
    {
      return i;
    }
  }
}

uint32
Bone::getRotationIndex(float _deltaTime)
{
  for (uint32 i = 0; i < m_numRotations - 1; ++i)
  {
    if (_deltaTime < m_rotations[i + 1].timeStamp)
    {
      return i;
    }
  }
}

uint32
Bone::getScaleIndex(float _deltaTime)
{
  for (uint32 i = 0; i < m_numScales - 1; ++i)
  {
    if (_deltaTime < m_scales[i + 1].timeStamp)
    {
      return i;
    }
  }
}

float
Bone::getScaleFactor(float _lastTimeStamp, float _nextTimeStamp, float _deltaTime)
{
  float midPoint = _deltaTime - _lastTimeStamp;
  float frameOffset = _nextTimeStamp - _lastTimeStamp;
  return midPoint / frameOffset;
}
}
