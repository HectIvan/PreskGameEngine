#include "pkTextureResource.h"
#include "pkLogger.h"

namespace pkEngineSDK
{

using pkEngineSDK::ANSICHAR;

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
  file.read(reinterpret_cast<ANSICHAR*>(&texHeader.width), sizeof(int32));
  file.read(reinterpret_cast<ANSICHAR*>(&texHeader.height), sizeof(int32));
  file.read(reinterpret_cast<ANSICHAR*>(&texHeader.bpp), sizeof(int32));
  file.read(reinterpret_cast<ANSICHAR*>(&texHeader.format), sizeof(uint32));
  file.read(reinterpret_cast<ANSICHAR*>(&texHeader.mipMapCount), sizeof(uint32));

  m_width = texHeader.width;
  m_height = texHeader.height;
  m_bpp = texHeader.bpp;
  m_format = texHeader.format;
  m_mipMapCount = texHeader.mipMapCount;
  const SIZE_T dataSize = static_cast<SIZE_T>(m_width * m_height * m_bpp);
  m_data.resize(dataSize);  

  file.read(reinterpret_cast<ANSICHAR*>(&m_data[0]), dataSize);

  file.close();

  m_isLoaded = true;

  return;
}
}
