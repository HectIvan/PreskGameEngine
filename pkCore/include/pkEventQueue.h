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
#include "pkPrerequisitesCore.h"
#include "pkEvent.h"

namespace pkEngineSDK {

class EventQueue
{
 public:
  EventQueue() = default;
  ~EventQueue() = default;


 private:
  Queue<Event> m_eventQueue;
};
}