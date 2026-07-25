/*****************************************************************************/
/**
 * @file    pkActorResource.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    05/12/2025
 * @brief   Resource for engine actors.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
 * Includes
 */
 /*********************************************/
#include "pkActorResource.h"
#include "pkLogger.h"
#include "pkComponent.h"

namespace pkEngineSDK
{

void
ActorResource::load()
{
  if (m_isLoaded) {
    return;
  }

  Logger& log = g_Logger();

  ifstream file(m_resourcePath, ios::in | ios::binary);
  
  // if the direcory cannot be opened.
  if (!file.is_open()) {
    const String msg = "Failed to open actor resource at directory " + String(m_resourcePath) + ".";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    return;
  }

  m_isLoaded = true;
  // load the base resource header
  loadBaseHeader(file);
  const SIZE_T v3Size = sizeof(Vector3);
  // read actor data
  file.read(reinterpret_cast<ANSICHAR*>(&m_position), v3Size);
  file.read(reinterpret_cast<ANSICHAR*>(&m_rotation), v3Size);
  file.read(reinterpret_cast<ANSICHAR*>(&m_scale), v3Size);
  uint8 isActiveRaw;
  file.read(reinterpret_cast<ANSICHAR*>(&isActiveRaw), sizeof(uint8));
  m_isActive = (isActiveRaw != 0);
  file.read(reinterpret_cast<ANSICHAR*>(&m_componentCount), sizeof(uint32));

  // check for components.
  for (uint32 i = 0; i < m_componentCount; ++i) {
    COMPONENT_TYPE::E compType;
    file.read(reinterpret_cast<ANSICHAR*>(&compType), sizeof(COMPONENT_TYPE::E));

    if (compType == COMPONENT_TYPE::kUnknown) { // if the component is unknown, skip it.
      continue;
    }
    if (compType == COMPONENT_TYPE::kModel) {
      
    }
    if (compType == COMPONENT_TYPE::kLight) {
      
    }
    if (compType == COMPONENT_TYPE::kCamera) {
      
    }
  }

  file.close();
}

void
ActorResource::unload()
{
  m_position = Vector3::ZERO;
  m_rotation = Vector3::ZERO;
  m_scale = Vector3(1);
  m_isActive = true;
  m_componentCount = 0;
}
}