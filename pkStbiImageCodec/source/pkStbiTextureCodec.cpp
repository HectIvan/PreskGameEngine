/*****************************************************************************/
/**
 * @file    pkStbiTextureCodec.h
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    24/10/2025
 * @brief   Codec for creating pkt Texures.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

 /*********************************************/
/**
* Includes
**/
/*********************************************/
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ENABLE_OPENEXR
#include "stb_image.h"

#include "pkAssetResourceManager.h"
#include "pkUUID.h"
#include "pkFileSystem.h"
#include "pkLogger.h"
#include "pkStbiTextureCodec.h"
#include "pkTexture.h"

namespace pkEngineSDK
{

extern "C" __declspec(dllexport) void
loadPlugin()
{
  TextureCodec::startUp<StbiTextureCodec>();
}

SPtr<TextureResource>
StbiTextureCodec::createResourceFromFile(const Path _path)
{
  Logger& log = g_Logger();
  bool canCreateResource = true;

  String fileName = _path.getFileNameWithoutExtension();
  String resourcePath = "resources/" + fileName + ".pkt";

  ofstream file(resourcePath, ios::out | ios::binary);

  // if the file cannot be open/created, return a warning and a nullptr.
  if (!file.is_open()) {
    canCreateResource = false;
    String msg = "Failed to generate resource for texture " + resourcePath + ".";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
  }

  // load data using stbi.
  int32 width, height, bpp;
  String extension = _path.getExtension();
  unsigned char* data;
  PK_TEXTURE_FORMAT::E format = PK_TEXTURE_FORMAT::kPK_FORMAT_R8G8B8A8_UNORM;

  const String fullPath = FileSystem::getAbsolutePath(_path).string();

  // load float data for exr and hdr files.
  if (extension == "exr" || extension == "hdr") {
    float* dataF = stbi_loadf(fullPath.c_str(), &width, &height, &bpp, 4);
    data = reinterpret_cast<unsigned char*>(dataF);
    bpp = 4 * sizeof(float);
    format = PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_FLOAT;
  }
  // load normal data for other file types.
  else {
    data = stbi_load(fullPath.c_str(), &width, &height, &bpp, 4);
  }

  // if stbi failed to load the texture data, return a warning and a nullptr.
  if (!data) {
    String msg = "STBI failed to load texture " + resourcePath + "."
                 + " Reason:" + stbi_failure_reason();
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }

  // force 4 bytes per pixel.
  if (bpp < 4) {
    bpp = 4;
  }

  SIZE_T dataSize = static_cast<SIZE_T>(width) * static_cast<SIZE_T>(height) * bpp;

  // create texture resource.
  SPtr<TextureResource> textureRes = make_shared<TextureResource>();

  textureRes->m_originalPath = _path.toString();
  textureRes->m_resourcePath = resourcePath;
  textureRes->m_name = fileName;
  textureRes->m_id = UUID::generateRandomUUID();

  textureRes->m_width = width;
  textureRes->m_height = height;
  textureRes->m_bpp = bpp;
  textureRes->m_format = static_cast<uint32>(format);
  textureRes->m_data = new unsigned char[dataSize];
  memcpy(textureRes->m_data, data, dataSize);

  // write the data into the pkt file.
  if (canCreateResource) {
    // generate the base resource header.
    textureRes->writeBaseHeader(file, textureRes->m_id, fileName, resourcePath);

    TextureAssetHeader texHeader;
    texHeader.width = textureRes->m_width;
    texHeader.height = textureRes->m_height;
    texHeader.bpp = textureRes->m_bpp;
    texHeader.format = textureRes->m_format;
    texHeader.dataSize = dataSize;
    file.write(reinterpret_cast<const char*>(&texHeader.width), sizeof(int32));
    file.write(reinterpret_cast<const char*>(&texHeader.height), sizeof(int32));
    file.write(reinterpret_cast<const char*>(&texHeader.bpp), sizeof(int32));
    file.write(reinterpret_cast<const char*>(&texHeader.format), sizeof(uint32));
    file.write(reinterpret_cast<const char*>(&texHeader.dataSize), sizeof(uint32));
    file.write(reinterpret_cast<char*>(&data[0]), dataSize);
    file.close();
  }

  if (data) { stbi_image_free(data); }

  return textureRes;
}
}
