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
#include <filesystem>

#include "pkPath.h"
#include "pkPrerequisitesUtilities.h"

using std::filesystem::path;
using std::filesystem::exists;
using std::filesystem::is_regular_file;
using std::filesystem::absolute;
using std::filesystem::current_path;
using std::filesystem::directory_iterator;

namespace pkEngineSDK
{

namespace FileSystem
{

/**
 * @brief Check if the path exists.
 * @param _path Path to check for.
 * @return If the path exists or not.
 */
PKFORCEINLINE static bool
fileExists(const Path& _path)
{
  const path fsPath(_path.toString());
  return exists(fsPath) && is_regular_file(fsPath);
}

/**
 * @brief Check if the path exists.
 * @param _path Path to check for.
 * @return If the path exists or not.
 */
PKFORCEINLINE static bool
fileExists(const ANSICHAR _path[])
{
  const path fsPath(_path);
  return exists(fsPath) && is_regular_file(fsPath);
}

/**
 * @brief Get the absolute path of a provided path.
 * @param _path Path to use.
 * @return Absolute path.
 */
PKFORCEINLINE static Path
getAbsolutePath(const Path& _path)
{
  const path fsPath(_path.toString());
  return absolute(fsPath).string();
}

/**
 * @brief Get the absolute path of a provided path.
 * @param _path Path to use.
 * @return Absolute path.
 */
PKFORCEINLINE static Path
getAbsolutePath(const ANSICHAR _path[])
{
  const path fsPath(_path);
  return absolute(fsPath).string();
}

/**
 * @brief Get the absolute path of a provided path.
 * @param _path Path to use.
 * @return Absolute path.
 */
PKFORCEINLINE static WString
getAbsolutePathWStr(const Path& _path)
{
  path fsPath(_path.toString());
  return absolute(fsPath).wstring();
}

/**
 * @brief Get all files from a folder.
 * @param _path Folder path where to look for.
 * @return Vector of paths
 */
PKFORCEINLINE static Vector<Path>
getFilesFromFolder(const Path _path)
{
  Path absPath = getAbsolutePath(_path);
  Vector<Path> paths;
  for (const auto& entry : directory_iterator(absPath.toString())) {
    path entryPath = entry.path();
    if (is_regular_file(entryPath)) {
      paths.push_back(entryPath.string());
    }
  }
  return paths;
}

PKFORCEINLINE static path
currentPath()
{
  return current_path();
}
}
}
