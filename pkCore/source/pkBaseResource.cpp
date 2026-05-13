#include "pkBaseResource.h"
#include "pkLogger.h"
#include "pkFileSystem.h"
#include "pkUUID.h"

namespace pkEngineSDK
{

bool
BaseResource::softLoad(const Path& _path)
{
  if (!FileSystem::fileExists(_path)) {
    return false;
  }

  const String path = _path.toString();
  ifstream file(path, ios::in | ios::binary);

  // if the direcory cannot be opened, return a warning and a nullptr.
  if (!file.is_open()) {
    const String msg = "Failed to load header resource at path: " + path + ".";
    LOG_WARNING(msg, __FILE__, __LINE__);
    return false;
  }

  loadBaseHeader(file);

  file.close();

  return true;
}

void
BaseResource::fillBaseHeader(const UUID& _id,
                             const String& _name,
                             const String& _originalPath,
                             const String& _resourcePath)
{
  m_id = _id;
  strcpy_s(m_name, PK_RESOURCE_NAME_SIZE, _name.c_str());
  strcpy_s(m_originalPath, PK_RESOURCE_PATH_SIZE, _originalPath.c_str());
  strcpy_s(m_resourcePath, PK_RESOURCE_PATH_SIZE, _resourcePath.c_str());
}

void
BaseResource::loadBaseHeader(ifstream& _file)
{
  _file.read(reinterpret_cast<ANSICHAR*>(&m_id), sizeof(UUID));
  _file.read(reinterpret_cast<ANSICHAR*>(&m_name), PK_RESOURCE_NAME_SIZE);
  _file.read(reinterpret_cast<ANSICHAR*>(&m_originalPath), PK_RESOURCE_PATH_SIZE);
  _file.read(reinterpret_cast<ANSICHAR*>(&m_resourcePath), PK_RESOURCE_PATH_SIZE);
}

void
BaseResource::writeBaseHeader(ofstream& _file)
{
  _file.write(reinterpret_cast<const ANSICHAR*>(&m_id), sizeof(UUID));
  _file.write(reinterpret_cast<const ANSICHAR*>(m_name), PK_RESOURCE_NAME_SIZE);
  _file.write(reinterpret_cast<const ANSICHAR*>(m_originalPath), PK_RESOURCE_PATH_SIZE);
  _file.write(reinterpret_cast<const ANSICHAR*>(m_resourcePath), PK_RESOURCE_PATH_SIZE);
}

// void
// BaseResource::writeBaseHeader(ofstream& _file,
//                               const UUID& _ID,
//                               const ANSICHAR* _fileName,
//                               const ANSICHAR* _originalPath,
//                               const ANSICHAR* _resourcePath)
// {
//   _file.write(reinterpret_cast<const ANSICHAR*>(&_ID), sizeof(UUID));
//   _file << _fileName << _originalPath << _resourcePath;
// }
}
