#include "pkInterface.h"
#include "pkGraphicsAPI.h"
#include "externals/imgui_impl_win32.h"

using pkEngineSDK::g_GraphicAPI;

#if PK_PLATFORM == PK_PLATFORM_WIN32

namespace pkEngineSDK
{
  /*
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
Interface::windowNewFrame()
{
  ImGui_ImplWin32_NewFrame();
}

void
Interface::windowShutDown()
{
  ImGui_ImplWin32_Shutdown();
}

void
Interface::init(const WindowHandle& _hWnd)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); static_cast<void>(io);
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags)
  {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }
    
  ImGui_ImplWin32_Init(_hWnd);
  ImGui::StyleColorsDark();
  // initialize with the current api
}*/
#endif
}