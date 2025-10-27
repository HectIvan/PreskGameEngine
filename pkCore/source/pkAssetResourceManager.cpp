/*****************************************************************************/
/**
 * @file    pkAssetResourceManager.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    18/09/2025
 * @brief   Definitions for the functions of the Asset Resource Manager used for the engine.
 *
 * @bug    No known bugs.
 *
 * @changes Created the file.
 */
 /*****************************************************************************/
#include "pkAssetResourceManager.h"
#include "pkTextureResource.h"
#include "pkPath.h"
#include "pkLogger.h"
#include "pkPrerequisitesCore.h"

namespace pkEngineSDK
{
void
AssetResourceManager::init()
{
  // To do: implement the asset resource manager.
}

SPtr<BaseResource>
AssetResourceManager::loadResource()
{
  return SPtr<BaseResource>();
}

SPtr<BaseResource>
AssetResourceManager::unloadResource()
{
  return SPtr<BaseResource>();
}

TextureResource*
AssetResourceManager::loadTextureResource(const Path _path)
{
  Logger& log = g_Logger();
  TextureResource* texResource = new TextureResource();

  ifstream file(_path.toString(), ios::in | ios::binary);

  // if the direcory cannot be opened, return a warning and a nullptr.
  if (!file.is_open()) {
    String msg = "Failed to open texture resource at directory " + _path.toString() + ".";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }

  TextureAssetHeader* texHeader = new TextureAssetHeader();
  file.read(reinterpret_cast<char*>(&texHeader->width), sizeof(int32));
  file.read(reinterpret_cast<char*>(&texHeader->height), sizeof(int32));
  file.read(reinterpret_cast<char*>(&texHeader->bpp), sizeof(int32));
  file.read(reinterpret_cast<char*>(&texHeader->format), sizeof(uint32));
  file.read(reinterpret_cast<char*>(&texHeader->dataSize), sizeof(uint32));

  texResource->m_width = texHeader->width;
  texResource->m_height = texHeader->height;
  texResource->m_bpp = texHeader->bpp;
  texResource->m_format = texHeader->format;

  texResource->m_data = new unsigned char[texHeader->dataSize];
  file.read(reinterpret_cast<char*>(&texResource->m_data[0]), texHeader->dataSize);

  delete texHeader;
  texHeader = nullptr;

  return texResource;
}

PK_CORE_EXPORT AssetResourceManager&
g_AssetResourceManager()
{
  return AssetResourceManager::instance();
}
}