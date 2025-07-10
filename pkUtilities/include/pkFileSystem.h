/*****************************************************************************/
/**
 * @file    pkFileSystem.h
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    09/06/2025
 * @brief   Sile system management for the game engine.
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
#include "pkPath.h"
#include "pkPrerequisitesUtilities.h"

#include <filesystem>

using std::filesystem::path;
using std::filesystem::exists;
using std::filesystem::is_regular_file;

namespace pkEngineSDK
{

  namespace FileSystem
  {
    static bool
    fileExists(const Path& _path)
    {
      path fsPath(_path.toString());
      return exists(fsPath) && is_regular_file(fsPath);
    }
  }
}
