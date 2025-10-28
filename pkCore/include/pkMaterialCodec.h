/*****************************************************************************/
/**
 * @file    pkMaterialCodec.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    27/10/2025
 * @brief   Codec for saving materials.
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
#include "pkModule.h"
#include "pkPrerequisitesCore.h"
#include "pkMaterialResource.h"

namespace pkEngineSDK
{

class Material;

class PK_CORE_EXPORT MaterialCodec : public Module<MaterialCodec>
{
 public:
  MaterialCodec() = default;
  virtual ~MaterialCodec() = default;

  /**
   * @brief Save a material as a pkmat to a given path.
   * @param _path Path to save the material to.
   * @return True if the material was saved successfully, false otherwise.
   */
  MaterialResource*
  createResource(const SPtr<Material> _pMaterial);
};

PK_CORE_EXPORT MaterialCodec&
g_MaterialCodec();
}
