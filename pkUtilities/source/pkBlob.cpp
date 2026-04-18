/*****************************************************************************/
/**
 * @file    pkBlob.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    15/11/2025
 * @brief   Information blob for the game engine.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
 * Includes
 */
 /*********************************************/
#include "pkBlob.h"

namespace pkEngineSDK
{
void
PKBlob::Release()
{
  m_blob.clear();
}

SIZE_T
PKBlob::getBufferSize() const
{
  return m_blob.size();
}

const void*
PKBlob::getBufferPointer() const
{
  return m_blob.data();
}
}
