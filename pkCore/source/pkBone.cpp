#include <assimp/anim.h>

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
  numPositions = _channel->mNumPositionKeys;
  for (uint32 i = 0; i < numPositions; ++i)
  {
    Vector3 position = Vector3(_channel->mPositionKeys[i].mValue.x,
                               _channel->mPositionKeys[i].mValue.y,
                               _channel->mPositionKeys[i].mValue.z);
    float timeStamp = static_cast<float>(_channel->mPositionKeys[i].mTime);
    KeyPosition data;
    data.position = position;
    data.timeStamp = timeStamp;
    positions.push_back(data);
  }

  // get all the rotations
  numRotations = _channel->mNumRotationKeys;
  for (uint32 i = 0; i < numRotations; ++i)
  {
    Vector4 rotation = Vector4(_channel->mRotationKeys[i].mValue.x,
                               _channel->mRotationKeys[i].mValue.y,
                               _channel->mRotationKeys[i].mValue.z,
                               _channel->mRotationKeys[i].mValue.w);
    float timeStamp = static_cast<float>(_channel->mRotationKeys[i].mTime);
    KeyRotation data;
    data.rotation = rotation;
    data.timeStamp = timeStamp;
    rotations.push_back(data);
  }

  // get all the scales
  numScales = _channel->mNumScalingKeys;
  for (uint32 i = 0; i < numScales; ++i)
  {
    Vector3 scale = Vector3(_channel->mScalingKeys[i].mValue.x,
                            _channel->mScalingKeys[i].mValue.y,
                            _channel->mScalingKeys[i].mValue.z);
    float timeStamp = static_cast<float>(_channel->mRotationKeys[i].mTime);
    KeyScale data;
    data.scale = scale;
    data.timeStamp = timeStamp;
    scales.push_back(data);
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
  if (1 == numPositions)
  {
    return Matrix4::translation(positions[0].position);
  }

  int p0Index = getPositionIndex(_deltaTime);
  int p1Index = p0Index + 1;
  float scaleFactor = getScaleFactor(positions[p0Index].timeStamp,
                                     positions[p1Index].timeStamp,
                                     _deltaTime);
  Vector3 finalPosition = PlatformMath::lerp3(positions[p0Index].position,
                                              positions[p1Index].position,
                                              scaleFactor);
  return Matrix4::translation(finalPosition);
}

Matrix4
Bone::interpolateRotation(float _deltaTime)
{
  if (1 == numRotations)
  {
    Vector4 rotation = rotations[0].rotation;
    rotation.normalize();
    return Matrix4::rotation(rotation.x, rotation.y, rotation.z);
  }

  int p0Index = getRotationIndex(_deltaTime);
  int p1Index = p0Index + 1;
  float scaleFactor = getScaleFactor(rotations[p0Index].timeStamp,
                                     rotations[p1Index].timeStamp,
                                     _deltaTime);
  Vector4 finalRotation = PlatformMath::lerp4(rotations[p0Index].rotation,
                                              rotations[p1Index].rotation,
                                              scaleFactor);
  finalRotation.normalize();
  return Matrix4::rotation(finalRotation.x, finalRotation.y, finalRotation.z);
}

Matrix4
Bone::interpolateScale(float _deltaTime)
{
  if (1 == numScales)
  {
    return Matrix4::scale(scales[0].scale);
  }
  int p0Index = getScaleIndex(_deltaTime);
  int p1Index = p0Index + 1;
  float scaleFactor = getScaleFactor(scales[p0Index].timeStamp,
                                     scales[p1Index].timeStamp,
                                     _deltaTime);
  Vector3 finalScale = PlatformMath::lerp3(scales[p0Index].scale,
                                           scales[p1Index].scale,
                                           scaleFactor);
  return Matrix4::scale(finalScale);
}

uint32
Bone::getPositionIndex(float _deltaTime)
{
  for (uint32 i = 0; i < numPositions - 1; ++i)
  {
    if (_deltaTime < positions[i + 1].timeStamp)
    {
      return i;
    }
  }
  return 1;
}

uint32
Bone::getRotationIndex(float _deltaTime)
{
  for (uint32 i = 0; i < numRotations - 1; ++i)
  {
    if (_deltaTime < rotations[i + 1].timeStamp)
    {
      return i;
    }
  }
  return 1;
}

uint32
Bone::getScaleIndex(float _deltaTime)
{
  for (uint32 i = 0; i < numScales - 1; ++i)
  {
    if (_deltaTime < scales[i + 1].timeStamp)
    {
      return i;
    }
  }
  return 1;
}

float
Bone::getScaleFactor(float _lastTimeStamp, float _nextTimeStamp, float _deltaTime)
{
  float midPoint = _deltaTime - _lastTimeStamp;
  float frameOffset = _nextTimeStamp - _lastTimeStamp;
  return midPoint / frameOffset;
}
}
