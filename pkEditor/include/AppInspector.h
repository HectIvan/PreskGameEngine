/*****************************************************************************/
/**
 * @file    AppInspector.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    11/03/2026
 * @brief   Inspector for the Editor App.
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/

class AppInspector
{
 public:
  AppInspector() = default;
  virtual ~AppInspector() = default;

  static void
  init(float& _camSpeed, float& _sensX, float& _sensY, bool& _vSync);
};
