#include "pkUInterface.h"
#include "pkGraphicsAPI.h"

using pkEngineSDK::g_GraphicAPI;

#if PK_PLATFORM == PK_PLATFORM_WIN32
#include "imgui_impl_win32.h"

namespace pkEngineSDK
{

struct ImGui_ImplWin32_Data
{
  HWND                        hWnd;
  HWND                        MouseHwnd;
  int                         MouseTrackedArea;   // 0: not tracked, 1: client area, 2: non-client area
  int                         MouseButtonsDown;
  ImGuiMouseCursor            LastMouseCursor;

  ImGui_ImplWin32_Data()      { memset(static_cast<void*>(this), 0, sizeof(*this)); }
};

void
UInterface::windowNewFrame()
{
  ImGui_ImplWin32_NewFrame();
}

void
UInterface::windowShutDown()
{
  ImGui_ImplWin32_Shutdown();
}

void
UInterface::initWin(const WindowHandle& _hWnd)
{
  ImGui_ImplWin32_Init(_hWnd);
}
#endif
}