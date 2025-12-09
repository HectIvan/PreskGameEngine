/*****************************************************************************/
/**
 * @file    pkUUID.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    30/10/2025
 * @brief   UUID class for the game engine.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesUtilities.h"

namespace uuids
{
class uuid;
}

namespace pkEngineSDK
{

class PK_UTILITY_EXPORT UUID
{
 public:
  UUID() = default;
  UUID(uuids::uuid _uuid);
  ~UUID() = default;

  /**
   * @brief Generate a completely random UUID.
   */
  static UUID
  generateRandomUUID();

  /**
   * @brief generate a random UUID based on a string.
   */
  static UUID
  generateRandomUUIDFromString(const String _string);

  /**
   * @brief Get the UUID string.
   */
  String
  uuidToString();

  int64
  getRawUUID() { return static_cast<int64>(*m_uuid); }

 public:
  static const String PK_NAME_GEN_SEED;
  static const String PK_DEFAULT_UUID;
  
 private:
  uuids::uuid* m_uuid;
};
}