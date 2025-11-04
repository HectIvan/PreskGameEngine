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

namespace pkEngineSDK
{

void
MaterialManager::init()
{

}

SPtr<Material>
MaterialManager::loadMaterial(const String& _ID)
{
  Logger& log = g_Logger();
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
    const String msg = "Resource not found. ID: " + _ID + ".";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }
  if (!resource->m_isLoaded) {
    resource->load();
  }

  SPtr<MaterialResource> matResource = reinterpret_pointer_cast<MaterialResource>(resource);
  if (!matResource) {
    const String msg = "Resource is not  amaterial resource: " + _ID + ".";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }

  material = make_shared<Material>();

  const String diffId = matResource->m_diffuseID;
  const String normId = matResource->m_normalID;
  const String aoId = matResource->m_aoID;
  const String roughId = matResource->m_roughnessID;
  const String metalId = matResource->m_metallicID;
  const String emissId = matResource->m_emissiveID;

  material->m_diffuse = textureMan.loadTexture(diffId);
  material->m_normal = textureMan.loadTexture(normId);
  material->m_occlusion = textureMan.loadTexture(aoId);
  material->m_roughness = textureMan.loadTexture(roughId);
  material->m_metallic = textureMan.loadTexture(metalId);
  material->m_emissive = textureMan.loadTexture(emissId);

  material->m_properties.ColorMultiply = matResource->m_diffuseColor.rgb();
  material->m_properties.roughnessMultiply = matResource->m_roughValue;
  material->m_properties.metallicMultiply = matResource->m_metallicValue;
  material->m_properties.EmissiveMultiply = matResource->m_emissiveColor.rgb();

  return material;
}

SPtr<Material>
MaterialManager::getMaterial(const String& _ID)
{
  // search if the texture has been stored before
  auto it = m_materials.find(_ID);
  if (it != m_materials.end()) {
    return it->second;
  }
  return nullptr;
}

void
MaterialManager::insertLoadedMaterial(const String& _ID, const SPtr<Material>& _pMaterial)
{
  m_materials.insert({ _ID, _pMaterial });
}

PK_CORE_EXPORT MaterialManager&
g_MaterialManager()
{
  return MaterialManager::instance();
}
}