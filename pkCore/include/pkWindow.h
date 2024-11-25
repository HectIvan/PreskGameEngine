/************************************************************************/
/**
* @pkWindow pkWindow.h
* @Hector Ivan Muñoz Ceballos
* @date 30/09/2024
* @Window file for the Presk Game Engine.
*
* This file will contain the window used for the engine
*
* @bug No bug known.
*
* @HectIvan 30/09/2024
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"
#include "pkVector2.h"

#if PK_PLATFORM == PK_PLATFORM_WIN32
/*********************************************/
/**
* #if's
**/
/*********************************************/
struct HWND__;
typedef HWND__* HWND;
using WindowHandle = HWND;

struct HINSTANCE__;
typedef HINSTANCE__* HINST;
using InstanceHandle = HINST;
#endif

namespace pkEngineSDK {


class PK_CORE_EXPORT PKWindowDesc;

class Window
{
 public:
  Window() = default;
  virtual ~Window() = default;

  /**
  * Create window.
  * 
  * @param _desc
  * Window description.
  * 
  * @param _name
  * Name of the window.
  **/
  void
  create(PKWindowDesc& _desc, String _name, InstanceHandle& hInstance);
  
  /**
  * Set size.
  *
  * Sets the size of the window.
  *
  * @param _x
  * Width of the window.
  *
  * @param _y
  * Height of the window.
  **/
  void
  setSize(uint32 _width, uint32 _height);

  /**
  * Set size.
  *
  * Sets the size of the window.
  *
  * @param _size
  * Size of the window.
  **/
  void
  setSize(Vector2 _size);

  /**
  * Set width.
  * 
  * Sets the width of the window for when its created or resized.
  *
  * Sets the width of the window.
  *
  * @param _x
  * Width that the window will hold.
  **/
  void
  setWidth(uint32 _width) { m_width = _width; }

  /**
  * Set Height.
  * 
  * Sets the height of the window for when its created or resized.
  *
  * Sets the height of the window.
  *
  * @param _y
  * Height that the window will hold.
  **/
  void
  setHeight(uint32 _height) { m_height = _height; }

  /**
  * Get size.
  * 
  * Gets the current size of the window.
  * 
  * @return
  * The size of the window.
  **/
  Vector2
  getSize() const;

  /**
  * Get width.
  * 
  * Gets the current width of the window.
  * 
  * @return
  * the current width of the window as an uint32
  **/
  uint32
  getWidth() { return m_width; }

  /**
  * Get height.
  * 
  * Gets the current height of the window.
  * 
  * @return
  * The current height of the window as an uint32
  **/
  uint32
  getHeight() { return m_height; }

  /**
  * Set window handle.
  * 
  * Sets the handle of the current window.
  * 
  * @param _newHandle
  * The new handle to be set to the current window.
  **/
  void
  setWindowHandle(WindowHandle _newHandle) { m_windowH = _newHandle; }

  /**
  * Get window handle.
  * 
  * Gets the handle of the current window.
  * 
  * @return
  * The window handle member.
  **/
  WindowHandle
  getWindowHandle() const { return m_windowH; }

  /**
  * Set instance handle.
  * 
  * Sets the handle of the current instance.
  * 
  * @param _hInstance
  * New instance.
  **/
  void
  setInstanceHandle(InstanceHandle _hInstance) { m_hInstance = _hInstance; }

  /**
  * Get instance handle.
  * 
  * Gets the handle of the current instance
  * 
  * @return
  * The current instance handle.
  **/
  InstanceHandle
  getInstanceHandle() const { return m_hInstance; }

  /**
  * Get width and height of the client rect for later use
  **/
  Vector2
  getClientWidthHeight();

 private:
  uint32 m_width;
  uint32 m_height;

  WindowHandle m_windowH;
  InstanceHandle m_hInstance;
};
}