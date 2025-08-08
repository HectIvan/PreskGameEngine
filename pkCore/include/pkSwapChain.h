/************************************************************************/
/**
* @pkSwapChain pkSwapChain.h
* @Hector Ivan Muñoz Ceballos
* @date 17/12/2024
* @Swap Chain file for the Presk Game Engine.
*
* This file contains the Swap Chain class of the engine
*
* @bug No bug known.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"
#include "pkTexture.h"

// to do: swap chain creation failes to create more than 1 render target.
namespace pkEngineSDK
{

class SwapChain
{
 public:
  SwapChain() = default;
  virtual ~SwapChain() = default;

  /**
   * @get the back buffer texture.
   * @param _index Position of the buffer.
   * @return Texture of the back buffer.
   */
  virtual SPtr<Texture>&
  getBuffer(uint32 _index) = 0;

  /**
   * @brief Get the back buffer width.
   * @return The buffer width.
   */
  FORCEINLINE uint32
  getWidth() const { return m_width; }

  /**
   * @brief Get the back buffer height.
   * @return The buffer height.
   */
  FORCEINLINE uint32
  getHeight() const { return m_height; }

  /**
   * @brief Get the swap chain size.
   * @return The swap chain size.
   */
  FORCEINLINE Vector2
  getSize() const
  {
    return Vector2(static_cast<float>(m_width), static_cast<float>(m_height));
  }

  /**
   * @brief Get the size of the swap chain according to one of its buffers.
   * @param _index Index to search for.
   * @return Buffer size.
   */
  FORCEINLINE Vector2
  getSize(uint32 _index) const
  {
    return m_buffers[_index]->getSize();
  }

  /**
   * @brief Set the back buffer width.
   * @param _width Width of the buffer.
   */
  FORCEINLINE void
  setWidth(uint32 _width) { m_width = _width; }

  /**
   * @brief Set the back buffer height.
   * @param _width Height of the buffer.
   */
  FORCEINLINE void
  setHeight(uint32 _height) { m_height = _height; }

 public:
  Vector<SPtr<Texture>> m_buffers;
  const uint32 m_bufferCount = 1;

  uint32 m_width;
  uint32 m_height;
};
}
