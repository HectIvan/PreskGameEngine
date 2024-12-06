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

namespace pkEngineSDK
{

struct PK_UTILITY_EXPORT Color final
{
  Color() = default;
  Color(uint8 r, uint8 g, uint8 b, uint8 a = 255)
  {
    _color.R = r;
    _color.G = g;
    _color.B = b;
    _color.A = a;

  }
  Color(const Color& other)
  {
    _color.fullColor = other._color.fullColor;
  }
  Color(uint8 other)
  {
    _color.fullColor = other;
  }
  virtual ~Color() = default;

  operator uint8() const
  {
    return _color.fullColor;
  }

  /**
  * Gets the red value of the color.
  * 
  * @return
  * The red value as an uint8.
  **/
  uint8
  getR() { return _color.R; }

  /**
  * Sets the red value of the color.
  *
  * @param _r
  * The red value as an uint8.
  **/
  void
  setR(uint8 _r) { _color.R = _r; }

  /**
  * Gets the green value of the color.
  *
  * @return
  * The green value as an uint8.
  **/
  uint8
  getG() { return _color.G; }

  /**
  * Sets the green value of the color.
  *
  * @param _g
  * The green value as an uint8.
  **/
  void
  setG(uint8 _g) { _color.G = _g; }

  /**
  * Gets the blue value of the color.
  *
  * @return
  * The blue value as an uint8.
  **/
  uint8
  getB() { return _color.B; }

  /**
  * Sets the blue value of the color.
  *
  * @param _b
  * The blue value as an uint8.
  **/
  void
  setB(uint8 _b) { _color.B = _b; }

  /**
  * Gets the alpha value of the color.
  *
  * @return
  * The alpha value as an uint8.
  **/
  uint8
  getA() { return _color.A; }

  /**
  * Sets the alpha value of the color.
  *
  * @param _a
  * The alpha value as an uint8.
  **/
  void
  setA(uint8 _a) { _color.A = _a; }

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
};
}