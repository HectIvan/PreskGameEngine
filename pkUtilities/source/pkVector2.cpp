/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkVector2.h"
#include "pkMath.h"

namespace pkEngineSDK
{

float 
Vector2::dotProd(const Vector2& _other)
{
  return (x * _other.x) + (y * _other.y);
}

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

void
Vector2::clamp(float _x, float _y)
{
  x = Math::clamp(x, _x, _y);
  y = Math::clamp(y, _x, _y);
}

float
Vector2::distanceTo(const Vector2& _other)
{
  return Math::sqrt(Math::pow((_other.x - x), 2) +
                    Math::pow((_other.y - y), 2));
}
}
