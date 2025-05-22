#include "pkInterface.h"

namespace pkEngineSDK
{
void
Interface::init(const WindowHandle& _hWnd)
{
  initAPI();

  ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();
}
}
