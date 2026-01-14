/*****************************************************************************/
/**
 * @file    pkMaterialResource.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    27/10/2025
 * @brief   Resource for engine materials.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
 * Includes
 */
/*********************************************/
#include "pkBaseResource.h"
#include "pkColor.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT MaterialResource : public BaseResource
{
 public:
  MaterialResource();
  virtual ~MaterialResource() = default;

  /**
   * @brief Load the material resource.
   */
  void
  load() override;

  /**
   * @brief Unload the material resource.
   */
  void
  unload() override;

  /**
   * @brief Get the resource type.
   * @return The resource type.
   */
  RESOURCE_TYPE::E
  getType() const override { return RESOURCE_TYPE::kMaterial; }

  /**
   * @brief Get the resource type as a string.
   * @return The resource type string.
   */
  String
  getTypeString() const override { return "Material"; }

 public:
   UUID m_albedoID;
   Vector3 m_albedoColor;
   UUID m_normalID;
   UUID m_aoID;
   UUID m_roughnessID;
   float m_roughValue;
   UUID m_metallicID;
   float m_metallicValue;
   UUID m_emissiveID;
   Vector3 m_emissiveColor;
};
}
