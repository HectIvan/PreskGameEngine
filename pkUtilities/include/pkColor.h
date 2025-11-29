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

struct PK_UTILITY_EXPORT Color final
{
  Color() = default;
  // constructor with select colors
  Color(uint8 _r, uint8 _g, uint8 _b, uint8 _a = 255)
  {
    _color.R = _r;
    _color.G = _g;
    _color.B = _b;
    _color.A = _a;
  }
  // constructor with one set color & alpha
  explicit Color(uint8 _rgb, uint8 _a)
  {
    _color.R = _rgb;
    _color.G = _rgb;
    _color.B = _rgb;
    _color.A = _a;
  }
  // constructor with a vector3 & alpha.
  explicit Color(const Vector3 _rgb, const uint8 _a = 255)
  {
    _color.R = _rgb.x;
    _color.G = _rgb.y;
    _color.B = _rgb.z;
    _color.A = _a;
  }
  // construct color with another
  Color(const Color& _other)
  {
    _color.fullColor = _other._color.fullColor;
  }
  // construct color from one single value
  Color(uint32 _other)
  {
    _color.fullColor = static_cast<uint32>(_other);
  }

  virtual ~Color() = default;

  operator uint8() const
  {
    return _color.fullColor;
  }

  PKFORCEINLINE const Vector4
  operator/(const float _val) const
  {
    Vector4 newColor;
    newColor.x = static_cast<float>(_color.R) / _val;
    newColor.y = static_cast<float>(_color.G) / _val;
    newColor.z = static_cast<float>(_color.B) / _val;
    newColor.w = static_cast<float>(_color.A) / _val;
    return newColor;
  }

  PKFORCEINLINE const Color&
  operator=(const Color& _other) {
    _color.R = _other._color.R;
    _color.G = _other._color.G;
    _color.B = _other._color.B;
    _color.A = _other._color.A;
    return *this;
  }

  PKFORCEINLINE const Color
  operator=(const Vector3& _other) {
    Color newColor;
    newColor.setR(_other.x);
    newColor.setG(_other.y);
    newColor.setB(_other.z);
    newColor.setA(1.0f);
    return newColor;
  }

  /**
   * @brief Get the red value of the color.
   * @return The red value as an uint8.
   */
  uint8
  getR() const { return _color.R; }

  /**
   * @brief Set the red value of the color.
   * @param _r The red value as an uint8.
   */
  void
  setR(uint8 _r) { _color.R = _r; }

  /**
   * @brief Get the green value of the color.
   * @return The green value as an uint8.
   */
  uint8
  getG() const { return _color.G; }

  /**
   * @brief Set the green value of the color.
   * @param _g The green value as an uint8.
   */
  void
  setG(uint8 _g) { _color.G = _g; }

  /**
   * @brief Get the blue value of the color.
   * @return The blue value as an uint8.
   */
  uint8
  getB() const { return _color.B; }

  /**
   * @brief Set the blue value of the color.
   * @param _b The blue value as an uint8.
   */
  void
  setB(uint8 _b) { _color.B = _b; }

  /**
   * @brief Get the alpha value of the color.
   * @return The alpha value as an uint8.
   */
  uint8
  getA() const { return _color.A; }

  /**
   * @brief Set the alpha value of the color.
   * @param _a The alpha value as an uint8.
   */
  void
  setA(uint8 _a) { _color.A = _a; }

  /**
   * @brief Get a Vector4 float from the color.
   * @return The vector.
   */
  Vector4
  colorTo01();

  /**
   * @brief Get the RGB value of the color into a vector.
   * @return Vector with the RGB values
   */
  Vector3
  rgb() { return Vector3(_color.R, _color.G, _color.B); }

  union
  {
    uint32 fullColor;
    struct
    {
      uint8 R;
      uint8 G;
      uint8 B;
      uint8 A;
    };
  }_color;

  static const Color WHITE;
  static const Color BLACK;
  static const Color RED;
  static const Color BLUE;
  static const Color YELLOW;
  static const Color GREEN;
  static const Color MAGENTA;
};
}
