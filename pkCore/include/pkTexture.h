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
#include "pkPrerequisitesCore.h"
#include "pkUUID.h"

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

namespace PK_RESOURCE_USAGE
{
enum E
{
  kUSAGE_DEFAULT = 0,
  kUSAGE_IMMUTABLE = 1,
  kUSAGE_DYNAMIC = 2,
  kUSAGE_STAGING = 3
};
}

namespace PK_RESOURCE_MISC_FLAG
{
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

class PK_CORE_EXPORT Texture
{
 public:
  Texture() = default;
  virtual ~Texture() = default;

  /**
   * @brief Set the texture width.
   * @param _width Window width.
   */
  PKFORCEINLINE void
  setWidth(const uint32& _width) { m_width = _width; }

  /**
   * @brief Set the texture height.
   * @param _height Window Height.
   */
  PKFORCEINLINE void
  setHeight(const uint32& _height) { m_height = _height; }

  /**
   * @brief Set the texture size.
   * @param _size Size of the texture.
   */
  void
  setSize(const Vector2& _size);

  /**
   * @brief Get the texture width.
   * @return the texture width.
   */
  PK_NODISCARD PKFORCEINLINE uint32
  getWidth() const { return m_width; }

  /**
   * @brief Get the texture height.
   * @return the texture height.
   */
  PK_NODISCARD PKFORCEINLINE uint32
  getHeight() const { return m_height; }

  /**
   * @brief Get the texture size.
   * @return Texture size.
   */
  PK_NODISCARD PKFORCEINLINE const Vector2
  getSize() const { return Vector2(m_width, m_height); }

  /**
   * @brief Set the texture name.
   * @param _name Name of the texture
   */
  virtual void
  setName(const String& _name) = 0;

  /**
   * @brief Get the texture name.
   * @return Texture name.
   */
  virtual const String
  getName() const = 0;

  /**
   * @brief Get the raw texture data.
   * @return The raw texture data.
   */
  virtual void*
  getRawData() { return nullptr; }

  /**
   * @brief Set the texture ID.
   * @param _ID ID for the texture.
   */
  PKFORCEINLINE void
  setID(const UUID& _ID) { m_id = _ID; }

  /**
   * @brief Get the texture ID.
   * @return The texture ID.
   */
  PK_NODISCARD PKFORCEINLINE const UUID
  getID() const { return m_id; }

 private:
  UUID m_id;
  uint32 m_width;
  uint32 m_height;
};
}
