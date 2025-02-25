#include "pkImGui.h"

#if PK_PLATFORM == PK_PLATFORM_WIN32

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
initWin32(WindowHandle& _hWnd)
{
  _hWnd = _hWnd; // temporary (prevent warning)
}

void
ImguiEngine::init(WindowHandle& _hWnd)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); static_cast<void>(io);
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsDark();

}
}
#endif