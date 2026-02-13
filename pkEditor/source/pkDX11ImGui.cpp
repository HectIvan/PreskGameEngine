#include "pkUInterface.h"
#include "pkGraphicsAPI.h"
#include "imgui_impl_dx11.h"
#include "pkPlatformMath.h"
#include "pkTexture.h"

namespace pkEngineSDK
{
  
void
UInterface::initWithAPI()
{
  // get the api
  GraphicsAPI& api = GraphicsAPI::instance();
  // initialize with the directX api
  ImGui_ImplDX11_Init(api.getDevice());
}

void
UInterface::newFrameAPI()
{
  ImGui_ImplDX11_NewFrame();
}

void
UInterface::render()
{
  // ImGuiIO& io = ImGui::GetIO();
  // Rendering
  ImGui::Render();
  ImGui_ImplDX11_RenderDrawData(static_cast<ImDrawData*>(getDrawData()));
}
}
