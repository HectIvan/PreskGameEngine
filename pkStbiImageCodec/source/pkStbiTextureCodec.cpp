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
StbiTextureCodec::createResource(const String _name,
                                 const int32 _width,
                                 const int32 _height,
                                 const int32 _bpp,
                                 const uint32 _format,
                                 const uint32 _mipCount,
                                 Vector<uint8>& _data)
{
  const String textureName = Path(_name).getFileNameWithoutExtension();
  const String resourcePath = PK_RESOURCE_FOLDER + textureName + ".pkt";

  ofstream file(resourcePath, ios::out | ios::binary | ios::trunc);

  // if the file cannot be open/created, return a warning and a nullptr.
  if (!file.is_open()) {
    const String msg = "Failed to generate resource for texture " + resourcePath + ".";
    LOG_ERROR(msg, __FILE__, __LINE__);
    return nullptr;
  }

  const SIZE_T dataSize = static_cast<SIZE_T>(_width * _height * _bpp);

  // create texture resource.
  SPtr<TextureResource> textureRes = make_shared<TextureResource>();

  textureRes->fillBaseHeader(_name + "Texture", textureName, _name, resourcePath);
  textureRes->writeBaseHeader(file);

  const SIZE_T sizeInt32 = sizeof(int32);
  const SIZE_T sizeUint32 = sizeof(uint32);
  file.write(reinterpret_cast<const ANSICHAR*>(&_width), sizeInt32);
  file.write(reinterpret_cast<const ANSICHAR*>(&_height), sizeInt32);
  file.write(reinterpret_cast<const ANSICHAR*>(&_bpp), sizeInt32);
  file.write(reinterpret_cast<const ANSICHAR*>(&_format), sizeUint32);
  file.write(reinterpret_cast<const ANSICHAR*>(&_mipCount), sizeUint32);
  file.write(reinterpret_cast<ANSICHAR*>(&_data[0]), dataSize);

  file.close();

  return textureRes;
}

SPtr<TextureResource>
StbiTextureCodec::createResourceFromFile(const Path _path)
{
  const String fileName = _path.getFileNameWithoutExtension();
  const String resourcePath = PK_RESOURCE_FOLDER + fileName + ".pkt";

  if (!FileSystem::fileExists(_path)) {
    const String msg = "Texture file does not exist at path " + _path.toString() + ".";
    LOG_WARNING(msg, __FILE__, __LINE__);
    return nullptr;
  }

  // load data using stbi.
  int32 width, height, bpp;

  UANSICHAR* data;
  PK_TEXTURE_FORMAT::E format = PK_TEXTURE_FORMAT::kPK_FORMAT_R8G8B8A8_UNORM;

  const String extension = _path.getExtension();
  const String fullPath = FileSystem::getAbsolutePath(_path).string();

  // load float data for exr and hdr files.
  uint32 mipcount = 1;
  if (extension == "exr" || extension == "hdr") {
    float* dataF = stbi_loadf(fullPath.c_str(), &width, &height, &bpp, 4);
    data = reinterpret_cast<UANSICHAR*>(dataF);
    bpp = 4 * sizeof(float);
    format = PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_FLOAT;
    mipcount = 0;
  }
  // load normal data for other file types.
  else {
    data = stbi_load(fullPath.c_str(), &width, &height, &bpp, 4);
  }

  // if stbi failed to load the texture data, return a warning and a nullptr.
  if (!data) {
    const String msg = "STBI failed to load texture " + resourcePath + "."
                       + " Reason:" + stbi_failure_reason();
    LOG_WARNING(msg, __FILE__, __LINE__);
    return nullptr;
  }

  // force 4 bytes per pixel.
  if (bpp < 4) {
    bpp = 4;
  }

  const SIZE_T dataSize = static_cast<SIZE_T>(width * height * bpp);
  Vector<uint8> finalData(dataSize);
  memcpy(finalData.data(), data, dataSize);

  SPtr<TextureResource> textureRes = createResource(fileName,
                                                    width,
                                                    height,
                                                    bpp,
                                                    static_cast<uint32>(format),
                                                    mipcount,
                                                    finalData);

  if (data) { stbi_image_free(data); }

  return textureRes;
}
}
