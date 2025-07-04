/************************************************************************/
/**
* @pkLight pkLight.h
* @Hector Ivan Muñoz Ceballos
* @date 22/11/2024
* @Light file for the Presk Game Engine.
*
* Light struct for the engine
*
* @bug No bug known.
*
* @HectIvan 22/11/2024
* Created the file
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkComponent.h"
#include "pkPrerequisitesCore.h"
#include "pkVector3.h"

namespace pkEngineSDK
{

namespace LIGHT_TYPE
{
  enum E : uint32
  {
    kDirectional = 0,
    kPoint,
    kSpot
  };
}

class PK_CORE_EXPORT Light : public Component
{
 public:
  Light() = default;
  ~Light() = default;

  /**
   * @brief Get the component type of this component.
   * @return The component type.
   */
  COMPONENT_TYPE::E
  getType() override { return COMPONENT_TYPE::kLight; }

  /**
   * @brief Get the name of the component.
   */
  const char*
  getName() override { return "Light"; }

  /**
   * @brief Get the component type of this component.
   * @return The component type.
   */
  static COMPONENT_TYPE::E
  getObjType() { return COMPONENT_TYPE::kLight; }

 public:
  float Type;
  float SpotCutoff;
  float SpotExponent;
  Vector3 LightDir = Vector3::FORWARD;
  Vector3 LightPos;
  Vector3 LightColor = Vector3(1.0f);
  Vector3 shadowColor = Vector3(0.2f);
  float unused;
};
}