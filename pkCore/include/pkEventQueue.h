/************************************************************************/
/**
* @pkEventQueue pkEventQueue.h
* @Hector Ivan Muñoz Ceballos
* @date 21/10/2024
* @Event queue file for the Presk Game Engine.
*
* This file contains the event queue class of the engine
*
* @bug No bug known.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkKey.h"
#include "pkVector2.h"

namespace pkEngineSDK
{

class EventQueue
{
 public:
  EventQueue() = default;
  virtual ~EventQueue() = default;

  /**
  * update both key inputs as well
  * as the mouse position.
  **/
  void
  poll();

  /**
  * check if the key is being pressed.
  * 
  * @param _key
  * Key to look for.
  * 
  * @return
  * If the key was pressed or not.
  **/
  bool
  iskeyPressed(KEY::E _key);

 private:
  UMap<uint32, Key> keys;
  Vector2 mousePosition;
};
}