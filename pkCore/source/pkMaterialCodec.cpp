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
MaterialCodec::createResource(const SPtr<Material> _pMaterial)
{
  if (!_pMaterial) {
    return nullptr;
  }
  Logger& log = g_Logger();

  const String materialName = _pMaterial->getNameS();
  const String filePath = "resources/" + materialName + ".pkmat";
  ofstream file(filePath, ios::out | ios::binary);

  if (!file.is_open()) {
    const String msg = "Failed to create material resource " + materialName + ".";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }

  SPtr<MaterialResource> matResource = make_shared<MaterialResource>();

  matResource->m_id = UUID::generateRandomUUIDFromString(materialName + " Material");
  matResource->m_name = materialName;
  matResource->m_resourcePath = filePath;

  // check for each texture and see if they are valid to use.
  if (_pMaterial->m_albedo) {
    matResource->m_albedoID = _pMaterial->m_albedo->getID();
    matResource->m_albedoColor = _pMaterial->m_properties.ColorMultiply;
  }
  if (_pMaterial->m_normal) {
    matResource->m_normalID = _pMaterial->m_normal->getID();
  }
  if (_pMaterial->m_oclussion) {
    matResource->m_aoID = _pMaterial->m_oclussion->getID();
  }
  if (_pMaterial->m_roughness) {
    matResource->m_roughnessID = _pMaterial->m_roughness->getID();
    matResource->m_roughValue = _pMaterial->m_properties.roughnessMultiply;
  }
  if (_pMaterial->m_metallic) {
    matResource->m_metallicID = _pMaterial->m_metallic->getID();
    matResource->m_metallicValue = _pMaterial->m_properties.metallicMultiply;
  }
  if (_pMaterial->m_emissive) {
    matResource->m_emissiveID = _pMaterial->m_emissive->getID();
    matResource->m_emissiveColor = _pMaterial->m_properties.EmissiveMultiply;
  }

  matResource->writeBaseHeader(file, matResource->m_id, materialName, filePath);

  // albedo write
  SIZE_T diffLength = matResource->m_albedoID.length();
  file.write(reinterpret_cast<const ANSICHAR*>(&diffLength), sizeof(SIZE_T));
  file.write(reinterpret_cast<const ANSICHAR*>(matResource->m_albedoID.c_str()), diffLength);
  file.write(reinterpret_cast<const ANSICHAR*>(&matResource->m_albedoColor), sizeof(Vector3));
  // normal write
  SIZE_T normalLength = matResource->m_normalID.length();
  file.write(reinterpret_cast<const ANSICHAR*>(&normalLength), sizeof(SIZE_T));
  file.write(reinterpret_cast<const ANSICHAR*>(matResource->m_normalID.c_str()), normalLength);
  // AO write
  SIZE_T aoLength = matResource->m_aoID.length();
  file.write(reinterpret_cast<const ANSICHAR*>(&aoLength), sizeof(SIZE_T));
  file.write(reinterpret_cast<const ANSICHAR*>(matResource->m_aoID.c_str()), aoLength);
  // roughness write
  SIZE_T roughLength = matResource->m_roughnessID.length();
  file.write(reinterpret_cast<const ANSICHAR*>(&roughLength), sizeof(SIZE_T));
  file.write(reinterpret_cast<const ANSICHAR*>(matResource->m_roughnessID.c_str()),
             roughLength);
  file.write(reinterpret_cast<const ANSICHAR*>(&matResource->m_roughValue), sizeof(float));
  // metallic write
  SIZE_T metalLength = matResource->m_metallicID.length();
  file.write(reinterpret_cast<const ANSICHAR*>(&metalLength), sizeof(SIZE_T));
  file.write(reinterpret_cast<const ANSICHAR*>(matResource->m_metallicID.c_str()),
             metalLength);
  file.write(reinterpret_cast<const ANSICHAR*>(&matResource->m_metallicValue), sizeof(float));
  // emissive write
  SIZE_T emissLength = matResource->m_emissiveID.length();
  file.write(reinterpret_cast<const ANSICHAR*>(&emissLength), sizeof(SIZE_T));
  file.write(reinterpret_cast<const ANSICHAR*>(matResource->m_emissiveID.c_str()),
             emissLength);
  file.write(reinterpret_cast<const ANSICHAR*>(&matResource->m_emissiveColor), sizeof(Vector3));

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