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
typedef HINSTANCE__ HINST;
using InstanceHandle = HINST;
#endif

namespace pkEngineSDK {

class PKWindowDesc;

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
  * Set width.
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
  * Sets the height of the window.
  *
  * @param _y
  * Height that the window will hold.
  **/
  void
  setHeight(uint32 _height) { m_height = _height; }

  /**
  * Get width and height of the client rect
  **/
  virtual Vector2
  getClientWidthHeight() = 0;

  uint32 m_width;
  uint32 m_height;

  WindowHandle m_windowH;
  InstanceHandle m_hInstance;
};
}