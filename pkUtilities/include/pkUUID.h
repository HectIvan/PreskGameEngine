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
#include "uuid.h"

namespace pkEngineSDK
{

class PK_UTILITY_EXPORT UUID
{
 public:
  UUID() = default;
  UUID(const uuids::uuid& _uuid);
  ~UUID() = default;

  /**
   * @brief Compare operator.
   * @param _other UUID to compare to.
   * @return Wether the UUID is the same or not.
   */
  bool
  operator==(const UUID& _other) const {
    return (m_uuid == _other.m_uuid);
  }

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
   * @return String of the ID.
   */
  String
  toString() const;

 public:
  static const String PK_NAME_GEN_SEED;
  static const UUID PK_DEFAULT_UUID;
  
  uuids::uuid m_uuid;
 private:
};
}

// to do: how could i do this without exposing the uuid?
namespace std {
template<>
struct hash<pkEngineSDK::UUID>
{
  SIZE_T
  operator()(const pkEngineSDK::UUID& _key) const {
    return hash<uuids::uuid>()(_key.m_uuid);
  }
};
}
