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

namespace pkEngineSDK
{

class PK_UTILITY_EXPORT UUID
{
 public:
  static String
  generateRandomUUID();

  static String
  generateRandomUUIDFromString(const String _string);

 public:
  static const String PK_NAME_GEN_SEED;
  static const String PK_DEFAULT_UUID;
};
}