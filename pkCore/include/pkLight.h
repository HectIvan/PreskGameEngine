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
  Light() {
    m_type = pkEngineSDK::LIGHT_TYPE::kDirectional;
    m_spotCutoff = 0.90f;
    m_spotExponent = 32.0f;
    m_lightDir = Vector3(0, 1.0f, 0);
    m_lightPos = Vector3(0.0f, 50.0f, 0.0f);
    m_lightColor = Vector3(1.0f);
    m_shadowIntensity = 0.65f;
    m_specIntensity = 1.0f;
  }
  virtual ~Light() = default;

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
  float m_type;
  float m_spotCutoff;
  float m_spotExponent;
  float m_shadowIntensity;
  float m_specIntensity;
  Vector3 m_lightDir = Vector3::FORWARD;
  Vector3 m_lightPos;
  Vector3 m_lightColor = Vector3(1.0f);
  Vector2 unused;
};
}