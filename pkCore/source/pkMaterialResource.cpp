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

  m_diffuseID = noID;
  m_diffuseColor = Color::WHITE;
  m_normalID = noID;
  m_aoID = noID;
  m_roughnessID = noID;
  m_roughValue = 1.0f;
  m_metallicID = noID;
  m_metallicValue = 1.0f;
  m_emissiveID = noID;
  m_emissiveColor = Color::WHITE;
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
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    return;
  }

  m_isLoaded = true;

  // load the base resource header
  loadBaseHeader(file);
  MaterialAssetHeader matHeader;

  // read diffuse data
  file.read(reinterpret_cast<char*>(&matHeader.diffuseIDSize), sizeof(SIZE_T));
  m_diffuseID.resize(matHeader.diffuseIDSize);
  file.read(reinterpret_cast<char*>(&m_diffuseID[0]), matHeader.diffuseIDSize);
  file.read(reinterpret_cast<char*>(&m_diffuseColor), sizeof(Color));

  // read normal data
  file.read(reinterpret_cast<char*>(&matHeader.normalIDSize), sizeof(SIZE_T));
  m_normalID.resize(matHeader.normalIDSize);
  file.read(reinterpret_cast<char*>(&m_normalID[0]), matHeader.normalIDSize);

  // read ao data
  file.read(reinterpret_cast<char*>(&matHeader.aoIDSize), sizeof(SIZE_T));
  m_aoID.resize(matHeader.aoIDSize);
  file.read(reinterpret_cast<char*>(&m_aoID[0]), matHeader.aoIDSize);

  // read roughness data
  file.read(reinterpret_cast<char*>(&matHeader.roughnessIDSize), sizeof(SIZE_T));
  m_roughnessID.resize(matHeader.roughnessIDSize);
  file.read(reinterpret_cast<char*>(&m_roughnessID[0]), matHeader.roughnessIDSize);
  file.read(reinterpret_cast<char*>(&m_roughValue), sizeof(float));

  // read metallic data
  file.read(reinterpret_cast<char*>(&matHeader.metallicIDSize), sizeof(SIZE_T));
  m_metallicID.resize(matHeader.metallicIDSize);
  file.read(reinterpret_cast<char*>(&m_metallicID[0]), matHeader.metallicIDSize);
  file.read(reinterpret_cast<char*>(&m_metallicValue), sizeof(float));

  // read emissive data
  file.read(reinterpret_cast<char*>(&matHeader.emissiveIDSize), sizeof(SIZE_T));
  m_emissiveID.resize(matHeader.emissiveIDSize);
  file.read(reinterpret_cast<char*>(&m_emissiveID[0]), matHeader.emissiveIDSize);
  file.read(reinterpret_cast<char*>(&m_emissiveColor), sizeof(Color));

  file.close();
}
}
