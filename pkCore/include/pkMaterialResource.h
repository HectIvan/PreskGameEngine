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
  SIZE_T diffusePathSize;
  SIZE_T normalPathSize;
  SIZE_T aoPathSize;
  SIZE_T roughnessPathSize;
  SIZE_T metallicPathSize;
  SIZE_T emissivePathSize;
};

class PK_CORE_EXPORT MaterialResource : public BaseResource
{
 public:
  MaterialResource() = default;
  virtual ~MaterialResource() = default;

  void
  load() override { return; }

  /**
   * @brief Get the resource type.
   * @return The resource type.
   */
  RESOURCE_TYPE::E
  getType() const override { return RESOURCE_TYPE::kMaterial; }
 public:
   String m_diffusePath;
   Color m_diffuseColor;
   String m_normalPath;
   String m_aoPath;
   String m_roughnessPath;
   float m_roughValue;
   String m_metallicPath;
   float m_metallicValue;
   String m_emissivePath;
   Color m_emissiveColor;
};
}
