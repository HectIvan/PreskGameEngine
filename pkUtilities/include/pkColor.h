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
  Color(uint8 r, uint8 g, uint8 b, uint8 a = 255)
  {
    _color.R = r;
    _color.G = g;
    _color.B = b;
    _color.A = a;
  }
  // constructor with one set color & alpha
  Color(uint8 rgb, uint8 a)
  {
    _color.R = rgb;
    _color.G = rgb;
    _color.B = rgb;
    _color.A = a;
  }
  // construct color with another
  Color(const Color& other)
  {
    _color.fullColor = other._color.fullColor;
  }
  // construct color from one single value
  Color(uint8 other)
  {
    _color.fullColor = other;
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

  union
  {
    uint8 fullColor;
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
