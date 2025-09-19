/*****************************************************************************/
/**
 * @file    pkAssetResourceManager.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    18/09/2025
 * @brief   This file will contain the Asset Resource Manager used for the engine
 *
 * @bug    No known bugs.
 *
 * @changes Created the file.
 */
 /*****************************************************************************/
#pragma once

#include "pkPrerequisitesCore.h"

namespace pkEngineSDK
{

namespace RESOURCE_TYPE
{
  enum E {
    kUnknown = 0,
    kModel,
    kMesh,
    kMaterial,
    kShader,
    kTexture
  };
}

class PK_CORE_EXPORT BaseResource
{
public:
  BaseResource() = default;
  virtual ~BaseResource() = default;

  /**
   * @brief Get the resource type.
   * @return The resource type.
   */
  RESOURCE_TYPE::E
  getType() const { return m_type; }

  /**
   * @brief Set the resource type.
   * @param _type Type of resource.
   */
  void
  setType(const RESOURCE_TYPE::E _type) { m_type = _type; }

 private:
  RESOURCE_TYPE::E m_type = RESOURCE_TYPE::kUnknown;
};
}