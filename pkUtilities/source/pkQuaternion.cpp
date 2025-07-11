/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkMath.h"
#include "pkQuaternion.h"

namespace pkEngineSDK {

void
pkEngineSDK::Quaternion::invert()
{
  x *= -1.0f;
  y *= -1.0f;
  z *= -1.0f;
}

float
Quaternion::magnitudeSquare() const
{
  return x * x + y * y + z * z + w * w;
}

float
Quaternion::magnitude() const
{
  return Math::sqrt(magnitudeSquare());
}

void
Quaternion::normalize()
{
  float mag = magnitude();
  if (mag == 0.0f) { return; }
  mag = 1.0f / mag;
  x *= mag;
  y *= mag;
  z *= mag;
  w *= mag;
}
}
