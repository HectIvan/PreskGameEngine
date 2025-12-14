/*****************************************************************************/
/**
 * @file    pkMaterialResource.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    04/11/2025
 * @brief   Resource for engine materials.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
 * Includes
 */
 /*********************************************/
#include "pkMaterialResource.h"
#include "pkLogger.h"
#include "pkUUID.h"

namespace pkEngineSDK
{

MaterialResource::MaterialResource()
{
  const UUID noID = UUID::PK_DEFAULT_UUID;
  m_albedoID = noID;
  m_normalID = noID;
  m_aoID = noID;
  m_roughnessID = noID;
  m_metallicID = noID;
  m_emissiveID = noID;
  m_isLoaded = false;
  m_albedoColor = Vector3(1.0f);
  m_roughValue = 1.0f;
  m_metallicValue = 1.0f;
  m_emissiveColor = Vector3(1.0f);
}

void
MaterialResource::load()
{
  if (m_isLoaded) {
    return;
  }

  ifstream file(m_resourcePath, ios::in | ios::binary);
  // if the direcory cannot be opened, return a warning and a nullptr.
  if (!file.is_open()) {
    const String msg = "Failed to open material resource at directory " +
                       String(m_resourcePath) +
                       ".";
    LOG_WARNING(msg, __FILE__, __LINE__);
    return;
  }

  m_isLoaded = true;

  // load the base resource header
  loadBaseHeader(file);

  const uint32 sizeUUID = sizeof(UUID);

  // read albedo data
  file.read(reinterpret_cast<ANSICHAR*>(&m_albedoID), sizeUUID);
  file.read(reinterpret_cast<ANSICHAR*>(&m_albedoColor), sizeof(Vector3));

  // read normal data
  file.read(reinterpret_cast<ANSICHAR*>(&m_normalID), sizeUUID);

  // read ao data
  file.read(reinterpret_cast<ANSICHAR*>(&m_aoID), sizeUUID);

  // read roughness data
  file.read(reinterpret_cast<ANSICHAR*>(&m_roughnessID), sizeUUID);
  file.read(reinterpret_cast<ANSICHAR*>(&m_roughValue), sizeof(float));

  // read metallic data
  file.read(reinterpret_cast<ANSICHAR*>(&m_metallicID), sizeUUID);
  file.read(reinterpret_cast<ANSICHAR*>(&m_metallicValue), sizeof(float));

  // read emissive data
  file.read(reinterpret_cast<ANSICHAR*>(&m_emissiveID), sizeUUID);
  file.read(reinterpret_cast<ANSICHAR*>(&m_emissiveColor), sizeof(Vector3));

  file.close();
}
}
