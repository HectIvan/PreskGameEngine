#include "pkImGui.h"
#include "pkGraphicsAPI.h"

namespace pkEngineSDK
{
void
ImguiEngine::init(WindowHandle& _hWnd)
{
  initWin(_hWnd);
  APIInit();

  ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();
}
}
