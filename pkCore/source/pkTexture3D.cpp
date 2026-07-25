/*****************************************************************************/
/**
 * @file    pkTexture3D.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    22/11/2025
 * @brief   3D texture for the engine.
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkTexture3D.h"
#include "pkLogger.h"

namespace pkEngineSDK
{
Texture3D::Texture3D(const uint32 _width, const uint32 _height, const uint32 _depth)
{
  m_width = _width;
  m_height = _height;
  m_slices.resize(_depth);
}

Texture3D::Texture3D(const uint32 _width,
                     const uint32 _height,
                     const Vector<SPtr<Texture>> _textures)
{
  // check if the texture slices meet the width and height sent as parameters.
  const uint32 texCount = toUint32(_textures.size());
  for (uint32 i = 0; i < texCount; ++i) {
    SPtr<Texture> texture = _textures[i];
    if (texture->getWidth() != _width || texture->getHeight() != _height) {
      LOG_WARNING("Texture slice size does not match 3D texture size.", __FILE__, __LINE__);
      return;
    }
  }

  // fill the texture3D data.
  m_width = _width;
  m_height = _height;
  m_slices = _textures;
}

void
Texture3D::addSlice(const SPtr<Texture>& _pTexture, const uint32 _slice)
{
  // check for out of bounds.
  if (_slice < 0 || _slice >= m_slices.size()) {
    LOG_WARNING("Index out of bounds when adding slice to 3D texture.", __FILE__, __LINE__);
    return;
  }
  // check for size match.
  if (_pTexture->getWidth() != m_width || _pTexture->getHeight() != m_height) {
    LOG_WARNING("Texture slice size does not match 3D texture size.", __FILE__, __LINE__);
    return;
  }

  m_slices[_slice] = _pTexture;
}

SPtr<Texture>
Texture3D::getSlice(const uint32 _slice)
{
  // check for out of bounds.
  if (_slice < 0 || _slice >= m_slices.size()) {
    LOG_WARNING("Index out of bounds when getting slice from 3D texture.", __FILE__, __LINE__);
    return nullptr;
  }
  return m_slices[_slice];
}
}

