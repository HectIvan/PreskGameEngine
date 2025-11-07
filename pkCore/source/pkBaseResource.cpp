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
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
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
  _file.read(reinterpret_cast<char*>(&baseHeader.IDSize), sizeof(SIZE_T));
  baseHeader.ID.resize(baseHeader.IDSize);
  _file.read(reinterpret_cast<char*>(&baseHeader.ID[0]), baseHeader.IDSize);
  _file.read(reinterpret_cast<char*>(&baseHeader.nameSize), sizeof(SIZE_T));
  baseHeader.name.resize(baseHeader.nameSize);
  _file.read(reinterpret_cast<char*>(&baseHeader.name[0]), baseHeader.nameSize);
  _file.read(reinterpret_cast<char*>(&baseHeader.pathSize), sizeof(SIZE_T));
  baseHeader.path.resize(baseHeader.pathSize);
  _file.read(reinterpret_cast<char*>(&baseHeader.path[0]), baseHeader.pathSize);

  m_id = baseHeader.ID;
  m_name = baseHeader.name;
  m_resourcePath = baseHeader.path;

  return baseHeader;
}

void
BaseResource::writeBaseHeader(ofstream& _file,
                              const String& _ID,
                              const String& _fileName,
                              const String& _resourcePath)
{
  BaseHeader header;
  header.ID = _ID;
  header.IDSize = _ID.length();
  header.name = _fileName;
  header.nameSize = _fileName.length();
  header.path = _resourcePath;
  header.pathSize = _resourcePath.length();

  _file.write(reinterpret_cast<const char*>(&header.IDSize), sizeof(SIZE_T));
  _file.write(reinterpret_cast<const char*>(header.ID.c_str()), header.IDSize);
  _file.write(reinterpret_cast<const char*>(&header.nameSize), sizeof(SIZE_T));
  _file.write(reinterpret_cast<const char*>(header.name.c_str()), header.nameSize);
  _file.write(reinterpret_cast<const char*>(&header.pathSize), sizeof(SIZE_T));
  _file.write(reinterpret_cast<const char*>(header.path.c_str()), header.pathSize);
}
}