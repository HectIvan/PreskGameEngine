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

  ifstream file(m_resourcePath, ios::in | ios::binary);

  // if the direcory cannot be opened, return a warning and a nullptr.
  if (!file.is_open()) {
    const String msg = "Failed to load a texture resource at directory " +
                       String(m_resourcePath) +
                       ".";
    LOG_WARNING(msg, __FILE__, __LINE__);
    return;
  }

  // load the base resource header data into the resource.
  loadBaseHeader(file);

  file.read(reinterpret_cast<ANSICHAR*>(&m_width), sizeof(int32));
  file.read(reinterpret_cast<ANSICHAR*>(&m_height), sizeof(int32));
  file.read(reinterpret_cast<ANSICHAR*>(&m_bpp), sizeof(int32));
  file.read(reinterpret_cast<ANSICHAR*>(&m_format), sizeof(PK_GRAPHICS_FORMAT::E));
  file.read(reinterpret_cast<ANSICHAR*>(&m_mipMapCount), sizeof(uint32));

  const SIZE_T dataSize = static_cast<SIZE_T>(m_width * m_height * m_bpp);
  m_data.resize(dataSize);
  file.read(reinterpret_cast<ANSICHAR*>(&m_data[0]), dataSize);

  file.close();

  m_isLoaded = true;

  return;
}

void
TextureResource::unload()
{
  m_isLoaded = false;
  m_width = 0;
  m_height = 0;
  m_bpp = 0;
  m_format = PK_GRAPHICS_FORMAT::kPK_FORMAT_UNKNOWN;
  m_mipMapCount = 0;
  m_data.clear();
}
}
