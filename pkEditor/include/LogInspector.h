/*****************************************************************************/
/**
 * @file    ActorInspector.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    11/03/2026
 * @brief   Inspector for the Game Actor.
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


class LogInspector
{
 public:
  LogInspector() = default;
  virtual ~LogInspector() = default;

  static void
  init(bool& _showErrors, bool& _showWarnings, bool& _showActions);
};
