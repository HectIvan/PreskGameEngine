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

TextureResource*
StbiTextureCodec::loadTextureFromFile(const Path _path)
{
  Logger& log = g_Logger();
  bool canCreateResource = true;

  String texturePath = "resources/" + _path.getFileNameWithoutExtension() + ".pkt";

  fstream file(texturePath, ios::out | ios::binary);

  // if the file cannot be open/created, return a warning and a nullptr.
  if (!file.is_open()) {
    canCreateResource = false;
    String msg = "Failed to generate resource for texture " + texturePath + ".";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    // return nullptr;
  }

  // load data using stbi image.
  int32 width, height, bpp;
  unsigned char* data = stbi_load(_path.toString().c_str(), &width, &height, &bpp, 4);

  // if stbi failed to load the texture data, return a warning and a nullptr.
  if (!data) {
    String msg = "STBI failed to load texture at directory " + texturePath + ".";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }

  SIZE_T dataSize = static_cast<SIZE_T>(width) * static_cast<SIZE_T>(height) * 4; // to do: remove magic numbers.
  // create texture resource.
  TextureResource* textureRes = new TextureResource();
  textureRes->m_width = width;
  textureRes->m_height = height;
  textureRes->m_bpp = bpp;
  textureRes->m_format = PK_TEXTURE_FORMAT::kPK_FORMAT_R8G8B8A8_UNORM; // to do: make this dynamic.
  textureRes->m_data = new unsigned char[dataSize];
  memcpy(textureRes->m_data, data, dataSize);

  // write the data into the pkt file.
  if (canCreateResource) {
    TextureAssetHeader texHeader;
    texHeader.width = textureRes->m_width;
    texHeader.height = textureRes->m_height;
    texHeader.bpp = textureRes->m_bpp;
    texHeader.format = textureRes->m_format;
    texHeader.dataSize = dataSize;
    file.write(reinterpret_cast<const char*>(&texHeader), sizeof(TextureAssetHeader));
    file.write(reinterpret_cast<const char*>(&data), dataSize);
    file.close();
  }

  if (data) { stbi_image_free(data); }

  return textureRes;
}
}
