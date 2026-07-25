/************************************************************************/
/**
* @pkColor pkColor.h
* @Hector Ivan Muñoz Ceballos
* @date 22/11/2024
* @Color file for the Presk Game Engine.
*
* Color struct for the engine
*
* @bug No bug known.
*
* @HectIvan 25/11/2024
* Created the file
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesUtilities.h"
#include "pkVector4.h"

namespace pkEngineSDK
{

struct Color;

struct PK_UTILITY_EXPORT FColor final
{
  FColor() = default;
  // constructor with select colors
  FColor(const float& _r, const float& _g, const float& _b, const float _a = 1.0f) :
    r(_r), g(_g), b(_b), a(_a)
  {}
  FColor(const Color& _color);
  // constructor with one set color & alpha
  explicit FColor(const float& _rgb, const float& _a)
  {
    r = _rgb;
    g = _rgb;
    b = _rgb;
    a = _a;
  }
  // constructor with a vector3 & alpha.
  explicit FColor(const Vector3 _rgb, const float _a = 1.0f)
  {
    r = _rgb.x;
    g = _rgb.y;
    b = _rgb.z;
    a = _a;
  }
  // construct color with another
  FColor(const FColor& _other)
  {
    r = _other.r;
    g = _other.g;
    b = _other.b;
    a = _other.a;
  }

  // construct color from one single value
  FColor(const float& _other)
  {
    r = _other;
    g = _other;
    b = _other;
    a = _other;
  }

  virtual ~FColor() = default;

  PKFORCEINLINE const FColor
  operator/(const float _val) const
  {
    FColor newColor;
    newColor.r = static_cast<float>(r) / _val;
    newColor.g = static_cast<float>(g) / _val;
    newColor.b = static_cast<float>(b) / _val;
    newColor.a = static_cast<float>(a) / _val;
    return newColor;
  }

  PKFORCEINLINE const FColor&
  operator=(const FColor& _other) {
    r = _other.r;
    g = _other.g;
    b = _other.b;
    a = _other.a;
    return *this;
  }

  PKFORCEINLINE const FColor
  operator=(const Vector3& _other) {
    FColor newColor;
    r = _other.x;
    g = _other.y;
    b = _other.z;
    a = 1.0f;
    return newColor;
  }

  FColor
  operator*(const float& _scalar) const
  {
    return FColor(r * _scalar, g * _scalar, b * _scalar, a * _scalar);
  }

  FColor
  operator*=(const float& _scalar)
  {
    *this = *this * _scalar;
    return *this;
  }

  FColor
  operator+(const FColor& _other) const
  {
    return FColor(r + _other.r, g + _other.g, b + _other.b, a + _other.a);
  }

  FColor
  operator+=(const FColor& _other)
  {
    *this = *this + _other;
    return *this;
  }

  FColor
  operator-(const FColor& _other) const
  {
    return FColor(r - _other.r, g - _other.g, b - _other.b, a - _other.a);
  }

  /**
   * @brief Get the RGB value of the color into a vector.
   * @return Vector with the RGB values
   */
  const Vector3
  rgb() const { return Vector3(r, g, b); }

  /**
   * @brief Convert the FColor to a Color.
   * @return The Color representation.
   */
  Color
  toColor() const;

  float r;
  float g;
  float b;
  float a;

  static const FColor WHITE;
  static const FColor BLACK;
  static const FColor RED;
  static const FColor BLUE;
  static const FColor YELLOW;
  static const FColor GREEN;
  static const FColor MAGENTA;
  static const FColor CYAN;
};
}
