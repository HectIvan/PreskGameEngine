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
  /*
  ImGuiIO& io = ImGui::GetIO();
  IMGUI_CHECKVERSION();
  IM_ASSERT(io.BackendPlatformUserData == nullptr && "Already initialized a platform backend!");
  
  // Setup backend capabilities flags
  ImGui_ImplWin32_Data* bd = IM_NEW(ImGui_ImplWin32_Data)();
  io.BackendPlatformUserData = (void*)bd;
  io.BackendPlatformName = "imgui_impl_win32";
  io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
  io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

  bd->hWnd = static_cast<WindowHandle>(_hWnd);
  bd->LastMouseCursor = ImGuiMouseCursor_COUNT;
  ImGui_ImplWin32_UpdateKeyboardCodePage(io);

  // Set platform dependent data in viewport
  ImGuiViewport* main_viewport = ImGui::GetMainViewport();
  main_viewport->PlatformHandle = main_viewport->PlatformHandleRaw =
                                  static_cast<void*>(bd->hWnd);*/
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