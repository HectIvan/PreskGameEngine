/*************************************************************************/
/**
 * @file    pkWindow.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    30/09/2024
 * @brief   Window for the Presk Game Engine.
 *
 * @bug    no knon bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"
#include "pkVector2.h"

/*********************************************/
/**
* #if's
**/
/*********************************************/
#if PK_PLATFORM == PK_PLATFORM_WIN32
struct HWND__;
typedef HWND__* HWND;
using WindowHandle = HWND;

struct HINSTANCE__;
typedef HINSTANCE__* HINST;
using InstanceHandle = HINST;
#endif

namespace pkEngineSDK
{

struct PKWindowDesc;

class PK_CORE_EXPORT Window
{
 public:
  Window() = default;
  virtual ~Window() = default;

  /**
   * @brief Create window.
   * @param _desc Window description.
   */
  void
  create(const PKWindowDesc& _desc);
  
  /**
   * @brief Set the size of the window.
   * @param _x Width of the window.
   * @param _y Height of the window.
   */
  void
  setSize(uint32 _width, uint32 _height);

  /**
   * @brief Set the size of the window.
   * @param _size Size of the window.
   */
  void
  setSize(Vector2 _size);

  /**
   * @brief Set the width of the window.
   * @param _x Width that the window will hold.
   */
  void
  setWidth(uint32 _width);

  /**
   * @brief Set the height of the window.
   * @param _y Height that the window will hold.
   */
  void
  setHeight(uint32 _height);

  /**
   * @brief Get the current size of the window.
   * @return The size of the window.
   */
  Vector2
  getSize() const;

  /**
   * @brief Get the current width of the window.
   * @return the current width of the window as an uint32
   */
  uint32
  getWidth() { return m_width; }

  /**
   * @brief Get the current height of the window.
   * @return The current height of the window as an uint32
   */
  uint32
  getHeight() { return m_height; }

  /**
   * @brief Set the handle of the current window.
   * @param _newHandle The new handle to be set to the current window.
   */
  void
  setWindowHandle(const WindowHandle& _newHandle) { m_windowH = _newHandle; }

  /**
   * @brief Get the handle of the current window.
   * @return The window handle member.
   */
  WindowHandle
  getWindowHandle() const { return m_windowH; }

  /**
   * @brief Set the handle of the current instance.
   * @param _hInstance New instance.
   */
  void
  setInstanceHandle(const InstanceHandle& _hInstance) { m_hInstance = _hInstance; }

  /**
   * @brief Get the handle of the current instance.
   * @return The current instance handle.
   */
  InstanceHandle
  getInstanceHandle() const { return m_hInstance; }

  /**
   * @brief Get width and height of the client rect for later use.
   * @return The client width and height.
   */
  Vector2
  getClientWidthHeight() const;

  bool m_isFocused;

 private:
  uint32 m_width;
  uint32 m_height;

  WindowHandle m_windowH;
  InstanceHandle m_hInstance;
};
}
