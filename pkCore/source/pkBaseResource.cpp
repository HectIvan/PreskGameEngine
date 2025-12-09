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
  Logger& log = g_Logger();

  const String path = _path.toString();
  ifstream file(path, ios::in | ios::binary);

  // if the direcory cannot be opened, return a warning and a nullptr.
  if (!file.is_open()) {
    const String msg = "Failed to load header resource at path: " + path + ".";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kWarning);
    return "";
  }

  loadBaseHeader(file);

  file.close();

  return true;
}

BaseHeader
BaseResource::loadBaseHeader(ifstream& _file)
{
  // generate the base resource header.
  BaseHeader baseHeader;

  // write the base resource header.
  _file.read(reinterpret_cast<ANSICHAR*>(&baseHeader.ID), sizeof(UUID));
  _file.read(reinterpret_cast<ANSICHAR*>(&baseHeader.name[0]), PK_RESOURCE_NAME_SIZE);
  _file.read(reinterpret_cast<ANSICHAR*>(&baseHeader.originalPath[0]), PK_RESOURCE_PATH_SIZE);
  _file.read(reinterpret_cast<ANSICHAR*>(&baseHeader.path[0]), PK_RESOURCE_PATH_SIZE);

  m_id = baseHeader.ID;
  strcpy_s(m_name, PK_RESOURCE_NAME_SIZE, baseHeader.name);
  strcpy_s(m_originalPath, PK_RESOURCE_PATH_SIZE, baseHeader.originalPath);
  strcpy_s(m_resourcePath, PK_RESOURCE_PATH_SIZE, baseHeader.path);

  return baseHeader;
}

void
BaseResource::writeBaseHeader(ofstream& _file,
                              const UUID& _ID,
                              const ANSICHAR* _fileName,
                              const ANSICHAR* _resourcePath)
{
  BaseHeader header;
  header.ID = _ID;
  strcpy_s(header.name, PK_RESOURCE_NAME_SIZE, _fileName);
  strcpy_s(header.originalPath, PK_RESOURCE_PATH_SIZE, _fileName);
  strcpy_s(header.path, PK_RESOURCE_PATH_SIZE, _resourcePath);

  _file.write(reinterpret_cast<const ANSICHAR*>(&header.ID), sizeof(UUID));
  _file << header.name << header.originalPath << header.path;
}
}
