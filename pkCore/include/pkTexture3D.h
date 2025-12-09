/*****************************************************************************/
/**
 * @file    pkTexture3D.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    22/11/2025
 * @brief   3D texture for the engine.
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkTexture.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT Texture3D
{
 public:
  Texture3D() = default;

  Texture3D(const uint32 _width, const uint32 _height, const uint32 _depth);

  Texture3D(const uint32 _width,
            const uint32 _height,
            const Vector<SPtr<Texture>> _textures);

  virtual ~Texture3D() = default;

  /**
   * @brief Add a slice to the 3D texture.
   * @param _pTexture Texture slice to add.
   * @param _slice Slice index to add the texture to.
   */
  void
  addSlice(const SPtr<Texture>& _pTexture, const uint32 _slice);

  /**
   * @brief Get a slice of the 3D texture.
   * @param _slice Slice to get.
   * @return The texture slice.
   */
  SPtr<Texture>
  getSlice(const uint32 _slice);

 private:
  uint32 m_width = 0;
  uint32 m_height = 0;
  Vector<SPtr<Texture>> m_slices;
};
}
