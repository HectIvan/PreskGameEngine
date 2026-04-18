/*****************************************************************************/
/**
 * @file    pkBlob.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    15/11/2025
 * @brief   Information blob for the game engine.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
 * Includes
 */
 /*********************************************/
#include "pkPrerequisitesUtilities.h"

namespace pkEngineSDK
{

class PK_UTILITY_EXPORT PKBlob
{
public:
  PKBlob() = default;

  PKBlob(const Vector<ANSICHAR>& _data) {
    SIZE_T size = _data.size();
    m_blob.resize(size);
    memcpy(m_blob.data(), _data.data(), size);
  }

  PKBlob(const void* _data, const SIZE_T& _size) {
    m_blob.resize(_size);
    memcpy(m_blob.data(), _data, _size);
  }
  virtual ~PKBlob() = default;

  /**
   * @brief Release the blob data.
   */
  void
  Release();

  /**
   * @brief Check if the blob is empty.
   */
  bool
  empty() const { return m_blob.empty(); }

  /**
   * @brief Get the size of the buffer.
   * @return Size of the buffer.
   */
  SIZE_T
  getBufferSize() const;

  /**
   * @brief Get the buffer data.
   * @return Pointer to the data.
   */
  const void*
  getBufferPointer() const;

 private:
  Vector<byte> m_blob;
};
}
