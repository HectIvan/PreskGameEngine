/*****************************************************************************/
/**
 * @file    pkTexture.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    22/11/2024
 * @brief   Texture file for the Presk Game Engine.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once
/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPath.h"
#include "pkPrerequisitesCore.h"
#include "pkColor.h"

namespace pkEngineSDK
{

namespace PK_BIND_FLAG
{
  enum E
  {
    kPK_BIND_VERTEX_BUFFER = 0x1L,
    kPK_BIND_INDEX_BUFFER = 0x2L,
    kPK_BIND_CONSTANT_BUFFER = 0x4L,
    kPK_BIND_SHADER_RESOURCE = 0x8L,
    kPK_BIND_STREAM_OUTPUT = 0x10L,
    kPK_BIND_RENDER_TARGET = 0x20L,
    kPK_BIND_DEPTH_STENCIL = 0x40L,
    kPK_BIND_UNORDERED_ACCESS = 0x80L,
    kPK_BIND_DECODER = 0x200L,
    kPK_BIND_VIDEO_ENCODER = 0x400L
  };
}

namespace PK_USAGE
{
  enum E
  {
    kPK_USAGE_DEFAULT = 0,
    kPK_USAGE_IMMUTABLE = 1,
    kPK_USAGE_DYNAMIC = 2,
    kPK_USAGE_STAGING = 3
  };
}

namespace PK_TEXTURE_FORMAT
{
  enum E
  {
    kPK_FORMAT_UNKNOWN = 0,
    kPK_FORMAT_R32G32B32A32_TYPELESS = 1,
    kPK_FORMAT_R32G32B32A32_FLOAT = 2,
    kPK_FORMAT_R32G32B32A32_UINT = 3,
    kPK_FORMAT_R32G32B32A32_SINT = 4,
    kPK_FORMAT_R32G32B32_TYPELESS = 5,
    kPK_FORMAT_R32G32B32_FLOAT = 6,
    kPK_FORMAT_R32G32B32_UINT = 7,
    kPK_FORMAT_R32G32B32_SINT = 8,
    kPK_FORMAT_R16G16B16A16_TYPELESS = 9,
    kPK_FORMAT_R16G16B16A16_FLOAT = 10,
    kPK_FORMAT_R16G16B16A16_UNORM = 11,
    kPK_FORMAT_R16G16B16A16_UINT = 12,
    kPK_FORMAT_R16G16B16A16_SNORM = 13,
    kPK_FORMAT_R16G16B16A16_SINT = 14,
    kPK_FORMAT_R32G32_TYPELESS = 15,
    kPK_FORMAT_R32G32_FLOAT = 16,
    kPK_FORMAT_R32G32_UINT = 17,
    kPK_FORMAT_R32G32_SINT = 18,
    kPK_FORMAT_R32G8X24_TYPELESS = 19,
    kPK_FORMAT_D32_FLOAT_S8X24_UINT = 20,
    kPK_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
    kPK_FORMAT_X32_TYPELESS_G8X24_UINT = 22,
    kPK_FORMAT_R10G10B10A2_TYPELESS = 23,
    kPK_FORMAT_R10G10B10A2_UNORM = 24,
    kPK_FORMAT_R10G10B10A2_UINT = 25,
    kPK_FORMAT_R11G11B10_FLOAT = 26,
    kPK_FORMAT_R8G8B8A8_TYPELESS = 27,
    kPK_FORMAT_R8G8B8A8_UNORM = 28,
    kPK_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
    kPK_FORMAT_R8G8B8A8_UINT = 30,
    kPK_FORMAT_R8G8B8A8_SNORM = 31,
    kPK_FORMAT_R8G8B8A8_SINT = 32,
    kPK_FORMAT_R16G16_TYPELESS = 33,
    kPK_FORMAT_R16G16_FLOAT = 34,
    kPK_FORMAT_R16G16_UNORM = 35,
    kPK_FORMAT_R16G16_UINT = 36,
    kPK_FORMAT_R16G16_SNORM = 37,
    kPK_FORMAT_R16G16_SINT = 38,
    kPK_FORMAT_R32_TYPELESS = 39,
    kPK_FORMAT_D32_FLOAT = 40,
    kPK_FORMAT_R32_FLOAT = 41,
    kPK_FORMAT_R32_UINT = 42,
    kPK_FORMAT_R32_SINT = 43,
    kPK_FORMAT_R24G8_TYPELESS = 44,
    kPK_FORMAT_D24_UNORM_S8_UINT = 45,
    kPK_FORMAT_R24_UNORM_X8_TYPELESS = 46,
    kPK_FORMAT_X24_TYPELESS_G8_UINT = 47,
    kPK_FORMAT_R8G8_TYPELESS = 48,
    kPK_FORMAT_R8G8_UNORM = 49,
    kPK_FORMAT_R8G8_UINT = 50,
    kPK_FORMAT_R8G8_SNORM = 51,
    kPK_FORMAT_R8G8_SINT = 52,
    kPK_FORMAT_R16_TYPELESS = 53,
    kPK_FORMAT_R16_FLOAT = 54,
    kPK_FORMAT_D16_UNORM = 55,
    kPK_FORMAT_R16_UNORM = 56,
    kPK_FORMAT_R16_UINT = 57,
    kPK_FORMAT_R16_SNORM = 58,
    kPK_FORMAT_R16_SINT = 59,
    kPK_FORMAT_R8_TYPELESS = 60,
    kPK_FORMAT_R8_UNORM = 61,
    kPK_FORMAT_R8_UINT = 62,
    kPK_FORMAT_R8_SNORM = 63,
    kPK_FORMAT_R8_SINT = 64,
    kPK_FORMAT_A8_UNORM = 65,
    kPK_FORMAT_R1_UNORM = 66,
    kPK_FORMAT_R9G9B9E5_SHAREDEXP = 67,
    kPK_FORMAT_R8G8_B8G8_UNORM = 68,
    kPK_FORMAT_G8R8_G8B8_UNORM = 69,
    kPK_FORMAT_BC1_TYPELESS = 70,
    kPK_FORMAT_BC1_UNORM = 71,
    kPK_FORMAT_BC1_UNORM_SRGB = 72,
    kPK_FORMAT_BC2_TYPELESS = 73,
    kPK_FORMAT_BC2_UNORM = 74,
    kPK_FORMAT_BC2_UNORM_SRGB = 75,
    kPK_FORMAT_BC3_TYPELESS = 76,
    kPK_FORMAT_BC3_UNORM = 77,
    kPK_FORMAT_BC3_UNORM_SRGB = 78,
    kPK_FORMAT_BC4_TYPELESS = 79,
    kPK_FORMAT_BC4_UNORM = 80,
    kPK_FORMAT_BC4_SNORM = 81,
    kPK_FORMAT_BC5_TYPELESS = 82,
    kPK_FORMAT_BC5_UNORM = 83,
    kPK_FORMAT_BC5_SNORM = 84,
    kPK_FORMAT_B5G6R5_UNORM = 85,
    kPK_FORMAT_B5G5R5A1_UNORM = 86,
    kPK_FORMAT_B8G8R8A8_UNORM = 87,
    kPK_FORMAT_B8G8R8X8_UNORM = 88,
    kPK_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
    kPK_FORMAT_B8G8R8A8_TYPELESS = 90,
    kPK_FORMAT_B8G8R8A8_UNORM_SRGB = 91,
    kPK_FORMAT_B8G8R8X8_TYPELESS = 92,
    kPK_FORMAT_B8G8R8X8_UNORM_SRGB = 93,
    kPK_FORMAT_BC6H_TYPELESS = 94,
    kPK_FORMAT_BC6H_UF16 = 95,
    kPK_FORMAT_BC6H_SF16 = 96,
    kPK_FORMAT_BC7_TYPELESS = 97,
    kPK_FORMAT_BC7_UNORM = 98,
    kPK_FORMAT_BC7_UNORM_SRGB = 99,
    kPK_FORMAT_AYUV = 100,
    kPK_FORMAT_Y410 = 101,
    kPK_FORMAT_Y416 = 102,
    kPK_FORMAT_NV12 = 103,
    kPK_FORMAT_P010 = 104,
    kPK_FORMAT_P016 = 105,
    kPK_FORMAT_420_OPAQUE = 106,
    kPK_FORMAT_YUY2 = 107,
    kPK_FORMAT_Y210 = 108,
    kPK_FORMAT_Y216 = 109,
    kPK_FORMAT_NV11 = 110,
    kPK_FORMAT_AI44 = 111,
    kPK_FORMAT_IA44 = 112,
    kPK_FORMAT_P8 = 113,
    kPK_FORMAT_A8P8 = 114,
    kPK_FORMAT_B4G4R4A4_UNORM = 115,
    kPK_FORMAT_P208 = 130,
    kPK_FORMAT_V208 = 131,
    kPK_FORMAT_V408 = 132,
    kPK_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE = 189,
    kPK_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE = 190,
    kPK_FORMAT_FORCE_UINT = 0xffffffff
  };
}

namespace PK_RESOURCE_MISC_FLAG {
  enum E
  {
    kPK_RESOURCE_MISC_GENERATE_MIPS = 0x1L,
    kPK_RESOURCE_MISC_SHARED = 0x2L,
    kPK_RESOURCE_MISC_TEXTURECUBE = 0x4L,
    kPK_RESOURCE_MISC_DRAWINDIRECT_ARGS = 0x10L,
    kPK_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS = 0x20L,
    kPK_RESOURCE_MISC_BUFFER_STRUCTURED = 0x40L,
    kPK_RESOURCE_MISC_RESOURCE_CLAMP = 0x80L,
    kPK_RESOURCE_MISC_SHARED_KEYEDMUTEX = 0x100L,
    kPK_RESOURCE_MISC_GDI_COMPATIBLE = 0x200L,
    kPK_RESOURCE_MISC_SHARED_NTHANDLE = 0x800L,
    kPK_RESOURCE_MISC_RESTRICTED_CONTENT = 0x1000L,
    kPK_RESOURCE_MISC_RESTRICT_SHARED_RESOURCE = 0x2000L,
    kPK_RESOURCE_MISC_RESTRICT_SHARED_RESOURCE_DRIVER = 0x4000L,
    kPK_RESOURCE_MISC_GUARDED = 0x8000L,
    kPK_RESOURCE_MISC_TILE_POOL = 0x20000L,
    kPK_RESOURCE_MISC_TILED = 0x40000L,
    kPK_RESOURCE_MISC_HW_PROTECTED = 0x80000L,
    kPK_RESOURCE_MISC_SHARED_DISPLAYABLE,
    kPK_RESOURCE_MISC_SHARED_EXCLUSIVE_WRITER,
    kPK_RESOURCE_MISC_NO_SHADER_ACCESS
  };
}

class Device;

namespace TEXTURE_TYPE
{
  enum E
  {
    kDiffuse = 0,
    kNormal,
    kEmmissive,
  };
}

struct TextureDesc
{
  uint32 bpp;
  uint32 width;
  uint32 height;
  int32 format;
  int32 usage;
  int32 bindFlags;
  int32 mipLevels = 1;
  int32 shaderResourceFormat;
};

struct TextureAsset
{
  uint8* data;
  uint32 width;
  uint32 height;
  uint32 channels;
};

class PK_CORE_EXPORT Texture
{
 public:
  Texture() = default;
  virtual ~Texture() = default;

  /**
   * @brief Set the texture width.
   * @param _width Window width.
   */
  void
  setWidth(uint32 _width) { m_width = _width; }

  /**
   * @brief Set the texture height.
   * @param _height Window Height.
   */
  void
  setHeight(uint32 _height) { m_height = _height; }

  /**
   * @brief Set the texture size.
   * @param _size Size of the texture.
   */
  void
  setSize(const Vector2 _size);

  /**
   * @brief Get the texture width.
   * @return the texture width.
   */
  uint32
  getWidth() { return m_width; }

  /**
   * @brief Get the texture height.
   * @return the texture height.
   */
  uint32
  getHeight() { return m_height; }

  /**
   * @brief Get the texture size.
   * @return Texture size.
   */
  Vector2
  getSize() { return Vector2(m_width, m_height); }

  /**
   * @brief Get texture type.
   * @return Type as an uint32.
   */
  const uint32&
  getType() const { return m_type; }

  /**
   * @brief Set texture type.
   * @param _type What type of texture it will be
   */
  void
  setType(uint32 _type) { m_type = static_cast<TEXTURE_TYPE::E>(_type); }

  /**
   * @brief Set the texture name.
   * @param _name Name of the texture
   */
  void
  setName(Path _name) { m_name = _name; }

  /**
   * @brief Get the texture name.
   * @return Name of the texture.
   */
  Path&
  getName() { return m_name; }

  /**
   * @brief Get the texture name as a string.
   * @return Name of the texture;
   */
  String
  getNameS() { return m_name.toString(); }

  /**
   * @brief Get the texture name as a const char*.
   * @return Name of the texture;
   */
  const char*
  getNameCSTR() { return m_name.toString().c_str(); }

  /**
   * @brief Get the raw texture data.
   * @return The raw texture data.
   */
  virtual void*
  getRawData() { return nullptr; }

  /**
   * @brief Copy the content of the derived class from another class.
   * @param _pTexture Other texture.
   * @return If the conversion was successful.
   */
  virtual bool
  copyFrom(SPtr<Texture>& _pTexture) { return false; };

 private:
  uint32 m_id;
  TEXTURE_TYPE::E m_type;
  Path m_name;
  uint32 m_width;
  uint32 m_height;
};
}
