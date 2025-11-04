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
  Logger& log = g_Logger();

  const String materialName = _pMaterial->getNameS();
  const String filePath = "resources/" + materialName + ".pkmat";
  ofstream file(filePath, ios::out);

  if (!file.is_open()) {
    const String msg = "Failed to create material resource " + materialName + ".";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }

  SPtr<MaterialResource> matResource = make_shared<MaterialResource>();

  matResource->m_id = UUID::generateRandomUUID();
  matResource->m_name = materialName;
  matResource->m_resourcePath = filePath;

  matResource->m_diffuseID = _pMaterial->m_diffuse->getID();
  matResource->m_diffuseColor = _pMaterial->m_properties.ColorMultiply;
  matResource->m_normalID = _pMaterial->m_normal->getID();
  matResource->m_aoID = _pMaterial->m_occlusion->getID();
  matResource->m_roughnessID = _pMaterial->m_roughness->getID();
  matResource->m_roughValue = _pMaterial->m_properties.roughnessMultiply;
  matResource->m_metallicID = _pMaterial->m_metallic->getID();
  matResource->m_metallicValue = _pMaterial->m_properties.metallicMultiply;
  matResource->m_emissiveID = _pMaterial->m_emissive->getID();
  matResource->m_emissiveColor = _pMaterial->m_properties.EmissiveMultiply;

  // base header write
  SIZE_T idLength = matResource->m_id.length();
  SIZE_T nameLength = materialName.length();
  SIZE_T resourceLength = filePath.length();
  file.write(reinterpret_cast<const char*>(&idLength), sizeof(SIZE_T));
  file.write(reinterpret_cast<const char*>(matResource->m_id.c_str()), idLength);
  file.write(reinterpret_cast<const char*>(&nameLength), sizeof(SIZE_T));
  file.write(reinterpret_cast<const char*>(materialName.c_str()), nameLength);
  file.write(reinterpret_cast<const char*>(&resourceLength), sizeof(SIZE_T));
  file.write(reinterpret_cast<const char*>(filePath.c_str()), resourceLength);

  // diffuse write
  SIZE_T diffLength = matResource->m_diffuseID.length();
  file.write(reinterpret_cast<const char*>(&diffLength), sizeof(SIZE_T));
  file.write(reinterpret_cast<const char*>(matResource->m_diffuseID.c_str()), diffLength);
  file.write(reinterpret_cast<const char*>(&matResource->m_diffuseColor), sizeof(Color));
  // normal write
  SIZE_T normalLength = matResource->m_normalID.length();
  file.write(reinterpret_cast<const char*>(&normalLength), sizeof(SIZE_T));
  file.write(reinterpret_cast<const char*>(matResource->m_normalID.c_str()), normalLength);
  // AO write
  SIZE_T aoLength = matResource->m_aoID.length();
  file.write(reinterpret_cast<const char*>(&aoLength), sizeof(SIZE_T));
  file.write(reinterpret_cast<const char*>(matResource->m_aoID.c_str()), aoLength);
  // roughness write
  SIZE_T roughLength = matResource->m_roughnessID.length();
  file.write(reinterpret_cast<const char*>(&roughLength), sizeof(SIZE_T));
  file.write(reinterpret_cast<const char*>(matResource->m_roughnessID.c_str()), roughLength);
  file.write(reinterpret_cast<const char*>(&matResource->m_roughValue), sizeof(float));
  // metallic write
  SIZE_T metalLength = matResource->m_metallicID.length();
  file.write(reinterpret_cast<const char*>(&metalLength), sizeof(SIZE_T));
  file.write(reinterpret_cast<const char*>(matResource->m_metallicID.c_str()), metalLength);
  file.write(reinterpret_cast<const char*>(&matResource->m_roughValue), sizeof(float));
  // emissive write
  SIZE_T emissLength = matResource->m_emissiveID.length();
  file.write(reinterpret_cast<const char*>(&emissLength), sizeof(SIZE_T));
  file.write(reinterpret_cast<const char*>(matResource->m_emissiveID.c_str()), emissLength);
  file.write(reinterpret_cast<const char*>(&matResource->m_emissiveColor), sizeof(Color));

  file.close();

  return matResource;
}

PK_CORE_EXPORT MaterialCodec&
g_MaterialCodec()
{
  return MaterialCodec::instance();
}
}