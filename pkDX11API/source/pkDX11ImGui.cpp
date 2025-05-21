#include "pkImGui.h"
#include "pkDX11GraphicsAPI.h"
#include "imgui_impl_dx11.h"

namespace pkEngineSDK
{

void
ImguiEngine::APIInit()
{
  // get the api
  GraphicsAPI& api = GraphicsAPI::instance();
  // initialize with the directX api
  ImGui_ImplDX11_Init(api.getDevice());
}

void
ImguiEngine::APInewFrame()
{
  ImGui_ImplDX11_NewFrame();
  ImGui::NewFrame();
}

void
ImguiEngine::render()
{
  ImGuiIO& io = ImGui::GetIO();
  // Rendering
  ImGui::Render();
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
}