/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkLogger.h"
#include "pkWindow.h"
#include "pkWindowDesc.h"

#if PK_PLATFORM == PK_PLATFORM_WIN32
#include <Windows.h>

#define IDI_ICON1 "icon.png"

namespace pkEngineSDK
{

LRESULT
CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void
Window::create(const PKWindowDesc& _desc, String& _name)
{
  /************************************************************************************/
  WNDCLASSEXA wcex;
  m_hInstance = InstanceHandle();
  memset(&wcex, 0, sizeof(wcex));
  wcex.cbSize = sizeof(WNDCLASSEX);
  // CS_HREDRAW Redraws the entire window if a movement or size adjustment changes the width of the client area.
  // CS_VREDRAW Redraws the entire window if a movement or size adjustment changes the height of the client area.
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc; // window procedure
  wcex.cbClsExtra = 0; // The number of extra bytes to allocate following the window-class structure. 
  wcex.cbWndExtra = 0; // The number of extra bytes to allocate following the window instance.
  wcex.hInstance = m_hInstance;
  wcex.hCursor = LoadCursorA(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);// register class
  wcex.lpszMenuName = nullptr;
  wcex.lpszClassName = "WindowClass";
  if (!RegisterClassEx(&wcex)) {
    g_Logger().print("Failed to register a window class");
    return;
  }
  /************************************************************************************/

  /**
  * Create the window.
  **/
  m_width = _desc.width;
  m_height = _desc.height;
  // adjust window width and height
  RECT rc = { 0, 0, static_cast<LONG>(_desc.width), static_cast<LONG>(_desc.height) };
  AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
  // create the window
  m_windowH = CreateWindowExA(0,
                              "WindowClass",
                              _name.c_str(),
                              WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT,
                              CW_USEDEFAULT,
                              rc.right - rc.left,
                              rc.bottom - rc.top,
                              nullptr,
                              nullptr,
                              m_hInstance,
                              nullptr);
  /**
  * Check if creation failed. 
  **/
  if (!m_windowH) {
    g_Logger().print("Failed to create the window.");
    return;
  }
  SetWindowLongPtrW(m_windowH, 0, reinterpret_cast<LONG_PTR>(this));
  ShowWindow(m_windowH, 1);
}

Vector2
Window::getClientWidthHeight()
{
  RECT rc;
  GetClientRect(m_windowH, &rc);
  uint32 width = rc.right - rc.left;
  uint32 height = rc.bottom - rc.top;
  return Vector2(static_cast<float>(width), static_cast<float>(height));
}

LRESULT
CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hdc;
  switch (message)
  {
  case WM_PAINT:
  {
    hdc = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    break;
  }
  case WM_DESTROY:
  {
    PostQuitMessage(0);
    break;
  }
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}
}

#endif // PK_PLATFORM_WIN32

// #if PK_PLATFORM == PK_PLATFORM_OSX
// for a future file specific to IOS
// #endif // PK_PLATFORM_OSX

/**
* General function definitions
**/
namespace pkEngineSDK
{

void
Window::setSize(uint32 _width, uint32 _height)
{
  setWidth(_width);
  setHeight(_height);
}

void
Window::setSize(Vector2 _size)
{
  setWidth(static_cast<uint32>(_size.x));
  setHeight(static_cast<uint32>(_size.y));
}

void
Window::setWidth(uint32 _width)
{
  m_width = _width;
  RECT rc = { 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };
  AdjustWindowRect(&rc, GetWindowLong(m_windowH, GWL_STYLE), false);
}

void
Window::setHeight(uint32 _height)
{
  m_height = _height;
  RECT rc = { 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };
  AdjustWindowRect(&rc, GetWindowLong(m_windowH, GWL_STYLE), false);
}

Vector2
Window::getSize() const
{
  return Vector2(static_cast<float>(m_width), 
                 static_cast<float>(m_height));
}
}