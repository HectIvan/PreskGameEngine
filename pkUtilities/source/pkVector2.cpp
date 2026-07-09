/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkVector2.h"
#include "pkMath.h"

namespace pkEngineSDK
{

const Vector2 Vector2::ZERO(0.0f, 0.0f);

float
Vector2::magnitudeSquare() const
{
  return x * x + y * y;
}

float 
Vector2::magnitude() const
{
  return Math::sqrt(magnitudeSquare());
}

void 
Vector2::normalize()
{
  float mag = magnitude();
  mag = 1.0f / mag;
  x *= mag;
  y *= mag;
}

const Vector2
Vector2::normalized() const
{
  float mag = magnitude();
  mag = 1.0f / mag;
  return Vector2(x * mag, y * mag);
}
}
