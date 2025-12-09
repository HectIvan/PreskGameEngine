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
  _file.read(reinterpret_cast<ANSICHAR*>(&baseHeader.IDSize), sizeof(SIZE_T));
  baseHeader.ID.resize(baseHeader.IDSize);
  _file.read(reinterpret_cast<ANSICHAR*>(&baseHeader.ID[0]), baseHeader.IDSize);
  _file.read(reinterpret_cast<ANSICHAR*>(&baseHeader.nameSize), sizeof(SIZE_T));
  baseHeader.name.resize(baseHeader.nameSize);
  _file.read(reinterpret_cast<ANSICHAR*>(&baseHeader.name[0]), baseHeader.nameSize);
  _file.read(reinterpret_cast<ANSICHAR*>(&baseHeader.pathSize), sizeof(SIZE_T));
  baseHeader.path.resize(baseHeader.pathSize);
  _file.read(reinterpret_cast<ANSICHAR*>(&baseHeader.path[0]), baseHeader.pathSize);

  m_id = baseHeader.ID;
  m_name = baseHeader.name;
  m_resourcePath = baseHeader.path;

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
  strcpy_s(header.path, PK_RESOURCE_PATH_SIZE, _resourcePath);

  _file.write(reinterpret_cast<const ANSICHAR*>(header.ID.uuidToString().c_str()),
                                                PK_RESOURCE_ID_SIZE);
  _file << header.name << header.path;
}
}
