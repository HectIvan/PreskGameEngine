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
using std::filesystem::absolute;

namespace pkEngineSDK
{

namespace FileSystem
{

/**
 * @brief Check if the path exists.
 * @param _path Path to check for.
 * @return If the path exists or not.
 */
static bool
fileExists(const Path& _path)
{
  path fsPath(_path.toString());
  return exists(fsPath) && is_regular_file(fsPath);
}

/**
 * @brief Get the absolute path of a provided path.
 * @param _path Path to use.
 * @return Absolute path.
 */
static path
getAbsolutePath(const Path& _path)
{
  path fsPath(_path.toString());
  return absolute(fsPath);
}

/**
 * @brief Get the absolute path of a provided path.
 * @param _path Path to use.
 * @return Absolute path.
 */
static WString
getAbsolutePathWStr(const Path& _path)
{
  path fsPath(_path.toString());
  return absolute(fsPath).wstring();
}
}
}
