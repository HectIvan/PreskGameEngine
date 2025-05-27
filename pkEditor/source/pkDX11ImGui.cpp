#include "pkInterface.h"
#include "pkGraphicsAPI.h"
#include "externals/imgui_impl_dx11.h"

namespace pkEngineSDK
{
  /*
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
Interface::endFrame()
{
  ImGui::EndFrame();
}

void
Interface::startWindowCreate(const char* _name)
{
  ImGui::Begin(_name);
}

void
Interface::createText(const char* _text)
{
  ImGui::Text(_text);
}

void
Interface::createSliderF(const char* _name, float& _param, const float _min, const float _max)
{
  ImGui::SliderFloat(_name, &_param, _min, _max);
}

void
Interface::createSliderVector2(const char* _name,
                               Vector2& _param,
                               const float _min,
                               const float _max)
{
  float vector[2] = { _param.x, _param.y };
  ImGui::SliderFloat2(_name, vector, _min, _max);
  _param.x = vector[0];
  _param.y = vector[1];
}

void
Interface::createSliderVector3(const char* _name,
                               Vector3& _param,
                               const float _min,
                               const float _max)
{
  float vector[3] = { _param.x, _param.y, _param.z };
  ImGui::SliderFloat3(_name, vector, _min, _max);
  _param.x = vector[0];
  _param.y = vector[1];
  _param.z = vector[2];
}

void
Interface::createCheckBox(const char* _name, bool& _param)
{
  ImGui::Checkbox(_name, &_param);
}

void
Interface::endWindowCreate()
{
  ImGui::End();
}

void*
Interface::getDrawData()
{
  return ImGui::GetDrawData();
}

void
Interface::render()
{
  ImGuiIO& io = ImGui::GetIO();
  // Rendering
  ImGui::Render();
  ImGui_ImplDX11_RenderDrawData(static_cast<ImDrawData*>(getDrawData()));
}*/
}