/*****************************************************************************/
/**
 * @file    pkMaterialCodec.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    27/10/2025
 * @brief   Codec for saving materials.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkLogger.h"
#include "pkMaterialCodec.h"
#include "pkMaterial.h"
#include "pkMaterialResource.h"
#include "pkPath.h"
#include "pkTexture.h"
#include "pkUUID.h"

namespace pkEngineSDK
{

SPtr<MaterialResource>
MaterialCodec::createResource(const SPtr<Material>& _pMaterial)
{
  if (!_pMaterial) {
    return nullptr;
  }

  const String materialName = _pMaterial->getNameS();
  const String filePath = "resources/" + materialName + ".pkmat";
  ofstream file(filePath, ios::out | ios::binary | ios::trunc);

  if (!file.is_open()) {
    const String msg = "Failed to create material resource " + materialName + ".";
    LOG_WARNING(msg, __FILE__, __LINE__);
    return nullptr;
  }

  SPtr<MaterialResource> matResource = make_shared<MaterialResource>();

  matResource->fillBaseHeader(materialName + "Material", materialName, "", filePath);
  matResource->writeBaseHeader(file);

  // check for each texture and see if they are valid to use.
  if (_pMaterial->m_albedo) {
    matResource->m_albedoID = *_pMaterial->m_albedo->getID();
    matResource->m_albedoColor = _pMaterial->m_properties.ColorMultiply;
  }
  if (_pMaterial->m_normal) {
    matResource->m_normalID = *_pMaterial->m_normal->getID();
  }
  if (_pMaterial->m_oclussion) {
    matResource->m_aoID = *_pMaterial->m_oclussion->getID();
  }
  if (_pMaterial->m_roughness) {
    matResource->m_roughnessID = *_pMaterial->m_roughness->getID();
    matResource->m_roughValue = _pMaterial->m_properties.roughnessMultiply;
  }
  if (_pMaterial->m_metallic) {
    matResource->m_metallicID = *_pMaterial->m_metallic->getID();
    matResource->m_metallicValue = _pMaterial->m_properties.metallicMultiply;
  }
  if (_pMaterial->m_emissive) {
    matResource->m_emissiveID = *_pMaterial->m_emissive->getID();
    matResource->m_emissiveColor = _pMaterial->m_properties.EmissiveMultiply;
  }


  const SIZE_T IDSize = sizeof(UUID);
  const SIZE_T V3Size = sizeof(Vector3);
  const SIZE_T FSize = sizeof(float);

  // albedo write
  file.write(reinterpret_cast<const ANSICHAR*>(&matResource->m_albedoID), IDSize);
  file.write(reinterpret_cast<const ANSICHAR*>(&matResource->m_albedoColor), V3Size);
  // normal write
  file.write(reinterpret_cast<const ANSICHAR*>(&matResource->m_normalID), IDSize);
  // AO write
  file.write(reinterpret_cast<const ANSICHAR*>(&matResource->m_aoID), IDSize);
  // roughness write
  file.write(reinterpret_cast<const ANSICHAR*>(&matResource->m_roughnessID), IDSize);
  file.write(reinterpret_cast<const ANSICHAR*>(&matResource->m_roughValue), FSize);
  // metallic write
  file.write(reinterpret_cast<const ANSICHAR*>(&matResource->m_metallicID), IDSize);
  file.write(reinterpret_cast<const ANSICHAR*>(&matResource->m_metallicValue), FSize);
  // emissive write
  file.write(reinterpret_cast<const ANSICHAR*>(&matResource->m_emissiveID), IDSize);
  file.write(reinterpret_cast<const ANSICHAR*>(&matResource->m_emissiveColor), V3Size);

  file.close();

  matResource->m_isLoaded = true;

  return matResource;
}

PK_CORE_EXPORT MaterialCodec&
g_MaterialCodec()
{
  return MaterialCodec::instance();
}
}