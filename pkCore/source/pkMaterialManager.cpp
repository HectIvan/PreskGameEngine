/*****************************************************************************/
/**
 * @file    pkMaterialManager.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    04/11/2025
 * @brief   Manager used for creating and storing materials.
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
#include "pkMaterialManager.h"
#include "pkMaterialResource.h"
#include "pkTextureManager.h"
#include "pkShaderManager.h"

namespace pkEngineSDK
{

void
MaterialManager::init()
{
  m_defaultMaterial = newMaterial(m_defaultMatName);
}

SPtr<Material>
MaterialManager::newMaterial(String _name)
{
  // create the material.
  if (_name == m_defaultMatName) {
    _name = "<Invalid Name>";
  }
  SPtr<Material> pMatComp = make_shared<Material>(_name);
  // return the material.
  return pMatComp;
}

SPtr<Material>
MaterialManager::loadMaterial(const UUID& _ID)
{
  AssetResourceManager& assetMan = g_AssetResourceManager();
  TextureManager& textureMan = g_TextureManager();
  // check if material has been stored in memory.
  SPtr<Material> material = getMaterial(_ID);
  if (material) {
    return material;
  }
  // get the resource and check if its loaded.
  SPtr<BaseResource> resource = assetMan.getResource(_ID);
  if (!resource) {
    const String msg = "Resource not found. ID: " + _ID.toString() + ".";
    LOG_WARNING(msg, __FILE__, __LINE__);
    return nullptr;
  }
  if (!resource->m_isLoaded) {
    resource->load();
  }

  SPtr<MaterialResource> matResource = reinterpret_pointer_cast<MaterialResource>(resource);
  if (!matResource) {
    const String msg = "Resource is not  amaterial resource: " + _ID.toString() + ".";
    LOG_WARNING(msg, __FILE__, __LINE__);
    return nullptr;
  }

  material = make_shared<Material>();

  const UUID diffId = matResource->m_albedoID;
  const UUID normId = matResource->m_normalID;
  const UUID aoId = matResource->m_aoID;
  const UUID roughId = matResource->m_roughnessID;
  const UUID metalId = matResource->m_metallicID;
  const UUID emissId = matResource->m_emissiveID;

  material->setName(resource->m_name);
  material->m_shader = g_ShaderManager().getShader(matResource->m_shaderID);

  material->m_albedo = textureMan.createTexture(diffId);
  material->m_normal = textureMan.createTexture(normId);
  material->m_oclussion = textureMan.createTexture(aoId);
  material->m_roughness = textureMan.createTexture(roughId);
  material->m_metallic = textureMan.createTexture(metalId);
  material->m_emissive = textureMan.createTexture(emissId);

  material->m_properties.ColorMultiply = matResource->m_albedoColor;
  material->m_properties.roughnessMultiply = matResource->m_roughValue;
  material->m_properties.metallicMultiply = matResource->m_metallicValue;
  material->m_properties.EmissiveMultiply = matResource->m_emissiveColor;

  insertMaterial(_ID, material);

  return material;
}

SPtr<Material>
MaterialManager::getMaterial(const UUID& _ID)
{
  // search if the texture has been stored before
  auto it = m_materials.find(_ID);
  if (it != m_materials.end()) {
    return it->second;
  }
  return nullptr;
}

void
MaterialManager::insertMaterial(const UUID& _ID, const SPtr<Material>& _pMaterial)
{
  m_materials.insert({ _ID, _pMaterial });
}

PK_CORE_EXPORT MaterialManager&
g_MaterialManager()
{
  return MaterialManager::instance();
}
}