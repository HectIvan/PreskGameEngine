#include "pkTextureResource.h"
#include "pkLogger.h"

namespace pkEngineSDK
{
void
TextureResource::load()
{
  if (m_isLoaded) {
    return;
  }

  Logger& log = g_Logger();

  ifstream file(m_resourcePath, ios::in | ios::binary);

  // if the direcory cannot be opened, return a warning and a nullptr.
  if (!file.is_open()) {
    const String msg = "Failed to load a texture resource at directory " + m_resourcePath + ".";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kWarning);
    return;
  }

  // load the base resource header data into the resource.
  loadBaseHeader(file);

  TextureAssetHeader texHeader;
  file.read(reinterpret_cast<char*>(&texHeader.width), sizeof(int32));
  file.read(reinterpret_cast<char*>(&texHeader.height), sizeof(int32));
  file.read(reinterpret_cast<char*>(&texHeader.bpp), sizeof(int32));
  file.read(reinterpret_cast<char*>(&texHeader.format), sizeof(uint32));
  file.read(reinterpret_cast<char*>(&texHeader.dataSize), sizeof(uint32));

  m_width = texHeader.width;
  m_height = texHeader.height;
  m_bpp = texHeader.bpp;
  m_format = texHeader.format;

  m_data = new unsigned char[texHeader.dataSize];
  file.read(reinterpret_cast<char*>(&m_data[0]), texHeader.dataSize);

  file.close();

  m_isLoaded = true;

  return;
}
}
