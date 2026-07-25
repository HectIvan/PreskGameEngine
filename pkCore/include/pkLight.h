/*****************************************************************************/
/**
 * @file    pkLight.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    22/11/2024
 * @brief   Light Component file for the Game Engine.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkComponent.h"
#include "pkPrerequisitesCore.h"
#include "pkMatrix4.h"
#include "pkVector3.h"

namespace pkEngineSDK
{

class Actor;

namespace LIGHT_TYPE
{
  enum E : uint32
  {
    kDirectional = 0,
    kPoint,
    kSpot
  };
}

struct LightDesc
{
  Vector3 dir;
  float spotCutoff;
  Vector3 pos;
  float spotExp;
  Vector3 color;
  float shadowInt;
  float specInt;
  LIGHT_TYPE::E type;
  Matrix4 transf;
  Matrix4 viewProj;
};

class PK_CORE_EXPORT Light : public Component
{
 public:
  explicit
  Light(const LightDesc& _desc) :
    m_direction(_desc.dir), m_spotCutoff(_desc.spotCutoff), m_position(_desc.pos),
    m_spotExponent(_desc.spotExp), m_color(_desc.color), m_shadowIntensity(_desc.shadowInt),
    m_specIntensity(_desc.specInt), m_type(_desc.type), m_transform(_desc.transf),
    m_viewProj(_desc.viewProj)
  { }
  Light() {
    m_type = LIGHT_TYPE::kDirectional;
    m_spotCutoff = 0.10f;
    m_spotExponent = 32.0f;
    m_direction = Vector3(0, -1.0f, 0);
    m_position = Vector3(0.0f, 1500.0f, 0.0f);
    m_color = Vector3(1.0f);
    m_shadowIntensity = 0.85f;
    m_specIntensity = 1.0f;
    m_transform = Matrix4::IDENTITY;
    m_viewProj = Matrix4::IDENTITY;
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
   * @brief Update the component.
   * @param _owner The actor that owns this component.
   */
  void
  update(Actor& _owner) override;

  /**
   * @brief Get the component type of this component.
   * @return The component type.
   */
  static COMPONENT_TYPE::E
  getObjType() { return COMPONENT_TYPE::kLight; }

 public:
  Vector3 m_direction = Vector3::FORWARD;
  float m_spotCutoff;
  Vector3 m_position;
  float m_spotExponent;
  Vector3 m_color = Vector3(1.0f);
  float m_shadowIntensity;
  float m_specIntensity;
  float m_type;

  Matrix4 m_transform;
  Matrix4 m_viewProj;
};
}
