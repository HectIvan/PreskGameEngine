#include "pkBone.h"
#include "pkMath.h"

namespace pkEngineSDK
{

Bone::Bone(const Bone& _bone) {
  positions = _bone.positions;
  rotations = _bone.rotations;
  scales = _bone.scales;
  numPositions = _bone.numPositions;
  numRotations = _bone.numRotations;
  numScales = _bone.numScales;

  setBoneName(_bone.getBoneName());
  setBoneID(_bone.getBoneID());
  setLocalTransform(_bone.getLocalTransform());
}

Bone&
Bone::operator=(const Bone& _bone)
{
  positions = _bone.positions;
  rotations = _bone.rotations;
  scales = _bone.scales;
  numPositions = _bone.numPositions;
  numRotations = _bone.numRotations;
  numScales = _bone.numScales;

  setBoneName(_bone.getBoneName());
  setBoneID(_bone.getBoneID());
  setLocalTransform(_bone.getLocalTransform());

  return *this;
}

void
Bone::update(float _deltaTime)
{
  Vector3 translation = interpolatePosition(_deltaTime).getTranslation3();
  // Matrix4 rotation = interpolateRotation(_deltaTime);
  // Matrix4 scale = interpolateScale(_deltaTime);

  // to do: fix this later on
  setLocalTransform(Matrix4(Vector4(translation, 0),
                            Vector4(0.0f),
                            Vector4(0.0f),
                            Vector4(0.0f)));
}

Matrix4
Bone::interpolatePosition(float _deltaTime)
{
  if (1 == numPositions) {
    return Matrix4::translation(positions[0].position);
  }

  uint32 p0Index = getPositionIndex(_deltaTime);
  uint32 p1Index = p0Index + 1;
  float scaleFactor = getScaleFactor(positions[p0Index].timeStamp,
                                     positions[p1Index].timeStamp,
                                     _deltaTime);
  Vector3 finalPosition = Math::lerp(positions[p0Index].position,
                                     positions[p1Index].position,
                                     scaleFactor);
  return Matrix4::translation(finalPosition);
}

Matrix4
Bone::interpolateRotation(float _deltaTime)
{
  if (1 == numRotations) {
    Vector4 rotation = rotations[0].rotation;
    rotation.normalize();
    return Matrix4::rotation(rotation.x, rotation.y, rotation.z);
  }

  uint32 p0Index = getRotationIndex(_deltaTime);
  uint32 p1Index = p0Index + 1;
  float scaleFactor = getScaleFactor(rotations[p0Index].timeStamp,
                                     rotations[p1Index].timeStamp,
                                     _deltaTime);
  Vector4 finalRotation = Math::lerp(rotations[p0Index].rotation,
                                     rotations[p1Index].rotation,
                                     scaleFactor);
  finalRotation.normalize();
  return Matrix4::rotation(finalRotation.x, finalRotation.y, finalRotation.z);
}

Matrix4
Bone::interpolateScale(float _deltaTime)
{
  if (1 == numScales) {
    return Matrix4::scale(scales[0].scale);
  }
  uint32 p0Index = getScaleIndex(_deltaTime);
  uint32 p1Index = p0Index + 1;
  float scaleFactor = getScaleFactor(scales[p0Index].timeStamp,
                                     scales[p1Index].timeStamp,
                                     _deltaTime);
  Vector3 finalScale = Math::lerp(scales[p0Index].scale, scales[p1Index].scale, scaleFactor);
  return Matrix4::scale(finalScale);
}

uint32
Bone::getPositionIndex(float _deltaTime)
{
  for (uint32 i = 0; i < numPositions - 1; ++i) {
    if (_deltaTime < positions[i + 1].timeStamp) {
      return i;
    }
  }
  return 1;
}

uint32
Bone::getRotationIndex(float _deltaTime)
{
  for (uint32 i = 0; i < numRotations - 1; ++i) {
    if (_deltaTime < rotations[i + 1].timeStamp) {
      return i;
    }
  }
  return 1;
}

uint32
Bone::getScaleIndex(float _deltaTime)
{
  for (uint32 i = 0; i < numScales - 1; ++i) {
    if (_deltaTime < scales[i + 1].timeStamp) {
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
