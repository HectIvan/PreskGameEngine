/************************************************************************/
/**
* @pkComponent pkComponent.h
* @Hector Ivan Muñoz Ceballos
* @date 00/12/2024
* @Component for the Presk Game Engine.
*
* This file contains the Component for the engine
*
* @bug No bug known.
* 
* @HectIvan 21/11/2024
* added a way to get the current component type.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"

namespace pkEngineSDK
{

namespace COMPONENT_TYPE
{
  enum E
  {
    kModel = 0,
    kMaterial,
    kLight,
    kCamera,
    kUnknown,
  };
}

class Component
{
 public:
  Component() = default;
  virtual ~Component() = default;

  virtual COMPONENT_TYPE::E
  getType() = 0;

  /**
   * @brief Get the name of the component.
   */
  virtual const char*
  getName() = 0;
};
}
