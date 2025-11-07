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

struct MaterialAssetHeader
{
  SIZE_T diffuseIDSize;
  SIZE_T normalIDSize;
  SIZE_T aoIDSize;
  SIZE_T roughnessIDSize;
  SIZE_T metallicIDSize;
  SIZE_T emissiveIDSize;
};

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
   * @brief Get the resource type.
   * @return The resource type.
   */
  RESOURCE_TYPE::E
  getType() const override { return RESOURCE_TYPE::kMaterial; }

 public:
   String m_diffuseID;
   Color m_diffuseColor;
   String m_normalID;
   String m_aoID;
   String m_roughnessID;
   float m_roughValue;
   String m_metallicID;
   float m_metallicValue;
   String m_emissiveID;
   Color m_emissiveColor;
};
}
