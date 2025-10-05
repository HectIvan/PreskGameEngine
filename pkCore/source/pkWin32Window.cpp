/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkLogger.h"
#include "pkWindow.h"
#include "pkWindowDesc.h"
#include "pkEventQueue.h"
#include "pkPlatformMath.h"

#if PK_PLATFORM == PK_PLATFORM_WIN32
#include <Windows.h>
#include <commdlg.h>
#pragma comment(lib, "Comdlg32.lib")

#define IDI_ICON1 "icon.png"

namespace pkEngineSDK
{

LRESULT
CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void
Window::create(const PKWindowDesc& _desc)
{
  Logger& log = g_Logger();
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
  wcex.cbWndExtra = sizeof(void*); // The number of extra bytes to allocate following the window instance.
  wcex.hInstance = m_hInstance;
  wcex.hCursor = LoadCursorA(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);// register class
  wcex.lpszMenuName = nullptr;
  wcex.lpszClassName = "WindowClass";
  if (!RegisterClassEx(&wcex)) {
    log.registerMessage("Failed to register a window class.", LOG_MSG_TYPE::kError);
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
                              _desc.name.c_str(),
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
   */
  if (!m_windowH) {
    log.registerMessage("Failed to create the window.", LOG_MSG_TYPE::kError);
    return;
  }
  auto* eventFunct = new WinFunctEvent(_desc.funct);
  SetWindowLongPtrW(m_windowH, 0, reinterpret_cast<LONG_PTR>(eventFunct));
  ShowWindow(m_windowH, 1);
  log.registerMessage("Created Window.");
}

Vector2
Window::getClientWidthHeight() const
{
  RECT rc;
  GetClientRect(m_windowH, &rc);
  uint32 width = rc.right - rc.left;
  uint32 height = rc.bottom - rc.top;
  return Vector2(width, height);
}

LRESULT
CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  EventQueue& eventManager = g_eventManager();
  WinFunctEvent* winEvent = reinterpret_cast<WinFunctEvent*>(GetWindowLongPtrW(hWnd, 0));
  if (winEvent) {
    PlatformPointer result = (*winEvent)(reinterpret_cast<PlatformPointer>(hWnd),
                                         static_cast<uint32>(message),
                                         reinterpret_cast<PlatformPointer>(wParam),
                                         reinterpret_cast<PlatformPointer>(lParam));
    if (result) {
      return 0;
    }
  }
  PAINTSTRUCT ps;
  switch (message)
  {
  case WM_PAINT:
  {
    // HDC hdc;
    // hdc = 
    BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    break;
  }
  case WM_DESTROY:
  {
    // get the function and delete it
    WinFunctEvent* eventFunct = reinterpret_cast<WinFunctEvent*>(GetWindowLongPtrW(hWnd, 0));
    delete eventFunct;
    eventFunct = nullptr;
    PostQuitMessage(0);
    break;
  }
  case WM_MOUSEWHEEL:
  {
    eventManager.scrollWheel = static_cast<int8>(GET_WHEEL_DELTA_WPARAM(wParam));
    eventManager.scrollWheel = static_cast<int8>(Math::clamp(eventManager.scrollWheel,
                                                 -1.0f,
                                                 1.0f));
    break;
  }
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}
}

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
  setSize(static_cast<uint32>(_size.x), static_cast<uint32>(_size.y));
}

void
Window::setWidth(uint32 _width)
{
  m_width = _width;
  RECT rc = { 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };
  AdjustWindowRect(&rc, GetWindowLong(m_windowH, GWL_STYLE), false);
  String message = "Set window width to: " + to_string(_width);
  g_Logger().registerMessage(message);
}

void
Window::setHeight(uint32 _height)
{
  m_height = _height;
  RECT rc = { 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };
  AdjustWindowRect(&rc, GetWindowLong(m_windowH, GWL_STYLE), false);
  String message = "Set window height to: " + to_string(_height);
  g_Logger().registerMessage(message);
}

Vector2
Window::getSize() const
{
  return Vector2(m_width, m_height);
}

String
Window::openFileFromExplorer() const
{
  OPENFILENAME ofn;       // common dialog box structure
  char szFile[MAX_PATH] = { 0 }; // buffer for file name

  // Initialize OPENFILENAME
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = NULL; // or your window handle
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = "All Files\0*.*\0Text Files\0*.TXT\0";
  ofn.nFilterIndex = 1;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  // Display the Open dialog box
  if (GetOpenFileNameA(&ofn) == TRUE)
  {
    return String(ofn.lpstrFile);
  }
  return String("");
}
}
#endif
