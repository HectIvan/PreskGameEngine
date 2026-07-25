/*****************************************************************************/
/**
 * @file    pkFileManager.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    24/07/2026
 * @brief   AFile manager to write and create new binary files.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
 * Includes
 */
 /*********************************************/
#include "pkPrerequisitesUtilities.h"
#include "pkModule.h"

namespace pkEngineSDK
{

namespace PK_FILE_TYPE
{
enum E
{
  kDefault = 0,
  kBinary,
};
}

class PK_UTILITY_EXPORT FileManager : public Module<FileManager>
{
 public:
  FileManager() = default;
  ~FileManager() = default;

  /**
   * @brief Create an fFile.
   * @param _directory directory in which to create the file.
   */
  fstream
  createFFile(const String& _directory,
              const PK_FILE_TYPE::E& _type = PK_FILE_TYPE::kDefault)
  {
    ios_base::openmode type = ios::in | ios::out;
    if (PK_FILE_TYPE::kBinary == _type) {
      type |= ios::binary;
    }
    return fstream(_directory, type);
  }

  /**
   * @brief Create an OutputFile.
   * @param _directory directory in which to create the file.
   */
  ofstream
  createOFile(const String& _directory,
              const PK_FILE_TYPE::E& _type = PK_FILE_TYPE::kDefault)
  {
    ios_base::openmode type = ios::in | ios::out;
    if (PK_FILE_TYPE::kBinary == _type) {
      type |= ios::binary;
    }
    return ofstream(_directory, type);
  }

  /**
   * @brief Create an In+putFile.
   * @param _directory directory in which to create the file.
   */
  ifstream
  createIFile(const String& _directory,
              const PK_FILE_TYPE::E& _type = PK_FILE_TYPE::kDefault)
  {
    ios_base::openmode type = ios::in | ios::out;
    if (PK_FILE_TYPE::kBinary == _type) {
      type |= ios::binary;
    }
    return ifstream(_directory, type);
  }
  
  /**
   * @brief Write on an fFile.
   * @param _file File to write on.
   * @param _data Data to write.
   */
  template<typename T>void
  write(const fstream& _file, const T& _data) const {
    _file.write(reinterpret_cast<const ANSICHAR*>(&_data), sizeof(T));
  }

  /**
   * @brief Write on an OutputFile.
   * @param _file File to write on.
   * @param _data Data to write.
   */
  template<typename T> void
  write(const ofstream& _file, const T& _data) const {
    _file.write(reinterpret_cast<const ANSICHAR*>(&_data), sizeof(T));
  }

  /**
   * @brief Read an fFile.
   * @param _file File to read.
   * @param _data Data output.
   */
  template<typename T> void
  read(const fstream& _file, const void* _data) const {
    _file.read(reinterpret_cast<ANSICHAR*>(&_data), sizeof(T));
  }

  /**
   * @brief Read an InputFile.
   * @param _file File to read.
   * @param _data Data output.
   */
  template<typename T> void
  read(const ifstream& _file, const void* _data) const {
    _file.read(reinterpret_cast<ANSICHAR*>(&_data), sizeof(T));
  }

  /**
   * @brief Close a file.
   */
  void
  close(fstream& _file) const {
    _file.close();
  }

  /**
   * @brief Close a file.
   */
  void
  close(ofstream& _file) const {
    _file.close();
  }

  /**
   * @brief Close a file.
   */
  void
  close(ifstream& _file) const {
    _file.close();
  }
};

PK_UTILITY_EXPORT FileManager&
g_FileManager() {
  return FileManager::instance();
}
}
