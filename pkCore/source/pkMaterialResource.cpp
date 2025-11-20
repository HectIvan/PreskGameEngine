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
  const String noID = UUID::PK_DEFAULT_UUID;
  m_isLoaded = false;
  m_albedoID = noID;
  m_albedoColor = Vector3(1.0f);
  m_normalID = noID;
  m_aoID = noID;
  m_roughnessID = noID;
  m_roughValue = 1.0f;
  m_metallicID = noID;
  m_metallicValue = 1.0f;
  m_emissiveID = noID;
  m_emissiveColor = Vector3(1.0f);
}

void
MaterialResource::load()
{
  if (m_isLoaded) {
    return;
  }

  Logger& log = g_Logger();
  ifstream file(m_resourcePath, ios::in | ios::binary);
  // if the direcory cannot be opened, return a warning and a nullptr.
  if (!file.is_open()) {
    const String msg = "Failed to open material resource at directory " + m_resourcePath + ".";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kWarning);
    return;
  }

  m_isLoaded = true;

  // load the base resource header
  loadBaseHeader(file);
  MaterialAssetHeader matHeader;

  // read albedo data
  file.read(reinterpret_cast<ANSICHAR*>(&matHeader.albedoIDSize), sizeof(SIZE_T));
  m_albedoID.resize(matHeader.albedoIDSize);
  file.read(reinterpret_cast<ANSICHAR*>(&m_albedoID[0]), matHeader.albedoIDSize);
  file.read(reinterpret_cast<ANSICHAR*>(&m_albedoColor), sizeof(Vector3));

  // read normal data
  file.read(reinterpret_cast<ANSICHAR*>(&matHeader.normalIDSize), sizeof(SIZE_T));
  m_normalID.resize(matHeader.normalIDSize);
  file.read(reinterpret_cast<ANSICHAR*>(&m_normalID[0]), matHeader.normalIDSize);

  // read ao data
  file.read(reinterpret_cast<ANSICHAR*>(&matHeader.aoIDSize), sizeof(SIZE_T));
  m_aoID.resize(matHeader.aoIDSize);
  file.read(reinterpret_cast<ANSICHAR*>(&m_aoID[0]), matHeader.aoIDSize);

  // read roughness data
  file.read(reinterpret_cast<ANSICHAR*>(&matHeader.roughnessIDSize), sizeof(SIZE_T));
  m_roughnessID.resize(matHeader.roughnessIDSize);
  file.read(reinterpret_cast<ANSICHAR*>(&m_roughnessID[0]), matHeader.roughnessIDSize);
  file.read(reinterpret_cast<ANSICHAR*>(&m_roughValue), sizeof(float));

  // read metallic data
  file.read(reinterpret_cast<ANSICHAR*>(&matHeader.metallicIDSize), sizeof(SIZE_T));
  m_metallicID.resize(matHeader.metallicIDSize);
  file.read(reinterpret_cast<ANSICHAR*>(&m_metallicID[0]), matHeader.metallicIDSize);
  file.read(reinterpret_cast<ANSICHAR*>(&m_metallicValue), sizeof(float));

  // read emissive data
  file.read(reinterpret_cast<ANSICHAR*>(&matHeader.emissiveIDSize), sizeof(SIZE_T));
  m_emissiveID.resize(matHeader.emissiveIDSize);
  file.read(reinterpret_cast<ANSICHAR*>(&m_emissiveID[0]), matHeader.emissiveIDSize);
  file.read(reinterpret_cast<ANSICHAR*>(&m_emissiveColor), sizeof(Vector3));

  file.close();
}
}
