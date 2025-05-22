#include "pkInterface.h"
#include "pkGraphicsAPI.h"
#include "externals/imgui_impl_dx11.h"

namespace pkEngineSDK
{

void
Interface::initAPI()
{
  // get the api
  GraphicsAPI& api = GraphicsAPI::instance();
  // initialize with the directX api
  ImGui_ImplDX11_Init(api.getDevice());
}

void
Interface::newFrameAPI()
{
  ImGui_ImplDX11_NewFrame();
  ImGui::NewFrame();
}

void
Interface::render()
{
  ImGuiIO& io = ImGui::GetIO();
  // Rendering
  ImGui::Render();
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
}