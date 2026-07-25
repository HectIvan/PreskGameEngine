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
#include "pkAssetResourceManager.h"
#include "pkLogger.h"
#include "pkMaterialCodec.h"
#include "pkMaterial.h"
#include "pkMaterialResource.h"
#include "pkPath.h"
#include "pkShader.h"
#include "pkShaderManager.h"
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

  ShaderManager& sm = g_ShaderManager();

  const String materialName = _pMaterial->getNameS();
  const String filePath = PK_RESOURCE_FOLDER + materialName + ".pkmat";
  ofstream file(filePath, ios::out | ios::binary | ios::trunc);

  if (!file.is_open()) {
    const String msg = "Failed to create material resource " + materialName + ".";
    LOG_WARNING(msg, __FILE__, __LINE__);
    return nullptr;
  }

  SPtr<MaterialResource> matResource = make_shared<MaterialResource>();

  const UUID id = MaterialResource::generateID(materialName.c_str());
  matResource->fillBaseHeader(id, materialName, "", filePath);
  matResource->writeBaseHeader(file);

  // check for each texture and see if they are valid to use.
  SPtr<Texture> albedo = _pMaterial->m_albedo.lock();
  if (albedo) {
    matResource->m_albedoID = albedo->getID();
    matResource->m_albedoColor = _pMaterial->m_properties.ColorMultiply;
  }
  SPtr<Texture> normal = _pMaterial->m_normal.lock();
  if (normal) {
    matResource->m_normalID = normal->getID();
  }
  SPtr<Texture> ao = _pMaterial->m_oclussion.lock();
  if (ao) {
    matResource->m_aoID = ao->getID();
  }
  SPtr<Texture> roughness = _pMaterial->m_roughness.lock();
  if (roughness) {
    matResource->m_roughnessID = roughness->getID();
    matResource->m_roughValue = _pMaterial->m_properties.roughnessMultiply;
  }
  SPtr<Texture> metallic = _pMaterial->m_metallic.lock();
  if (metallic) {
    matResource->m_metallicID = metallic->getID();
    matResource->m_metallicValue = _pMaterial->m_properties.metallicMultiply;
  }
  SPtr<Texture> emissive = _pMaterial->m_emissive.lock();
  if (emissive) {
    matResource->m_emissiveID = emissive->getID();
    matResource->m_emissiveColor = _pMaterial->m_properties.EmissiveMultiply;
  }

  const SIZE_T IDSize = sizeof(UUID);
  const SIZE_T V3Size = sizeof(Vector3);
  const SIZE_T FSize = sizeof(float);

  // if there isnt a shader assigned, search and assign the default shader.
  SPtr<Shader> shader = _pMaterial->m_shader.lock();
  if (!shader) {
    shader = sm.getShader(sm.m_defaultShaderKey);
    _pMaterial->setShader(shader);
  }
  file.write(reinterpret_cast<const ANSICHAR*>(&shader->m_id), sizeof(UUID));

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