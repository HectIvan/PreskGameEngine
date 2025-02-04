#include "pkImGui.h"
#include "pkGraphicsAPI.h"

namespace pkEngineSDK
{

void
ImguiEngine::init(WindowHandle& _hWnd)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); static_cast<void>(io);
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsDark();
  
#if PK_PLATFORM == PK_PLATFORM_WIN32
  // Setup Platform/Renderer backends
  // cImGui_ImplWin32_Init(_hWnd);
#endif
}
}
