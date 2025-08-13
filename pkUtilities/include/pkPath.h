/*****************************************************************************/
/**
 * @file    pkPath.h
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    09/06/2025
 * @brief   Path structure for the game engine.
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

// #if PK_PLATFORM == PK_PLATFORM_WIN32
// #include <Windows.h>
// 
// #endif

namespace pkEngineSDK
{

class PK_UTILITY_EXPORT Path
{
 public:
  Path();
  Path(const Path&);
  Path(Path&);

  Path& operator=(const Path&) = default;

  Path(const String& _path) : m_path(_path) {}

  virtual ~Path() = default;

  /**
   * @brief Gets the path.
   */
  const String&
  getPath() const
  {
    return m_path;
  }

  FORCEINLINE WString
  getPathWStr()
  {
    WString_Conv<Codecvt_utf8<WCHAR>> converter;
    WString str = converter.from_bytes(m_path.c_str());
    return str;
  }

  /**
   * @brief Set the path.
   * @param _path Path to set.
   */
  void
  setPath(String _path)
  {
    m_path = _path;
  }

  /**
   * @brief gets a string of the path.
   * @return String of the path.
   */
  String
  toString() const
  {
    return m_path;
  }

  /**
   * @brief Get the name of a file in a path.
   * @return Name of the file.
   */
  String
  getFileName() const
  {
    SIZE_T lastSlash = m_path.find_last_of("/\\");
    if (lastSlash == String::npos) {
      return m_path; // no directory
    }
    return m_path.substr(lastSlash + 1);
  }

  /**
   * @brief Get name of the file without the extension.
   * @return File name without extension.
   */
  String
  getFileNameWithoutExtension() const
  {
    String fileName = getFileName();
    SIZE_T lastDot = fileName.find_last_of(".");
    if (lastDot == String::npos) {
      return fileName; // no extension
    }
    return fileName.substr(0, lastDot);
  }

  /**
   * @brief Get directory of the path.
   * @return path directory.
   */
  String
  getDirectory() const
  {
    SIZE_T lastSlash = m_path.find_last_of("/\\");
    if (lastSlash == String::npos) {
      return ""; // no directory
    }
    return m_path.substr(0, lastSlash);
  }

  WString
  getDirectoryWStr() const
  {
    SIZE_T lastSlash = m_path.find_last_of("/\\");
    if (lastSlash == String::npos) {
      return L""; // no directory
    }
    WString_Conv<Codecvt_utf8<WCHAR>> converter;
    String path = m_path.substr(0, lastSlash);
    WString str = converter.from_bytes(path);
    return str;

    /**
#if PK_PLATFORM == PK_PLATFORM_WIN32
    const char* pathcstr = path.c_str();
    int32 len = MultiByteToWideChar(CP_UTF8, 0, pathcstr, -1, nullptr, 0);
    WCHAR* wch = new WCHAR[len];
    MultiByteToWideChar(CP_UTF8, 0, pathcstr, -1, wch, len);
    WString str(wch);
    return str;
#endif
    */
  }

  /**
   * @brief Get the extension of the path.
   * @return Extension type.
   */
  String
  getExtension() const
  {
    SIZE_T lastDot = m_path.find_last_of(".");
    if (lastDot == String::npos) {
      return ""; // no extension
    }
    return m_path.substr(lastDot + 1);
  }

 protected:
  String m_path;
};
}
