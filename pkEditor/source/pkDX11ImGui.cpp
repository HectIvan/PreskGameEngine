#include "pkUInterface.h"
#include "pkGraphicsAPI.h"
#include "externals/imgui_impl_dx11.h"
#include "pkPlatformMath.h"

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
UInterface::setCurrentContext()
{
  ImGui::SetCurrentContext(ImGui::GetCurrentContext());
}

void
UInterface::newFrameAPI()
{
  ImGui_ImplDX11_NewFrame();
}

void
UInterface::endFrame()
{
  ImGui::EndFrame();
}

void
UInterface::startWindowCreate(const char* _name)
{
  ImGui::Begin(_name, nullptr, ImGuiWindowFlags_HorizontalScrollbar);
}

void
UInterface::createText(const char* _text)
{
  ImGui::Text(_text);
}

bool
UInterface::createInputText(const char* _name, String& _param)
{
  return ImGui::InputText(_name, strdup(_param.c_str()), sizeof(_param));
}

bool
UInterface::createInputF(const char* _name, float& _param, float _step, float _largeStep)
{
  return ImGui::InputFloat(_name, &_param, _step, _largeStep);
}

bool
UInterface::createInputFClamp(const char* _name,
                              float& _param,
                              float _min,
                              float _max,
                              float _step,
                              float _largeStep)
{
  bool change = createInputF(_name, _param, _step, _largeStep);
  _param = Math::clamp(_param, _min, _max);
  return change;
}

bool
UInterface::createInputVector2(const char* _name, Vector2& _param)
{
  float v[2] = { _param.x, _param.y };
  bool change = ImGui::InputFloat2(_name, v);
  _param.x = v[0];
  _param.y = v[1];
  return change;
}

bool UInterface::createInputVector2Clamp(const char* _name, Vector2& _param, float _min, float _max)
{
  bool change = createInputVector2(_name, _param);
  _param.clamp(_min, _max);
  return change;
}

bool
UInterface::createInputVector3(const char* _name, Vector3& _param)
{
  float v[3] = { _param.x, _param.y, _param.z };
  bool change = ImGui::InputFloat3(_name, v);
  _param.x = v[0];
  _param.y = v[1];
  _param.z = v[2];
  return change;
}

Vector3
UInterface::createInputVector3Ret(const char* _name, Vector3 _param)
{
  float v[3] = { _param.x, _param.y, _param.z };
  ImGui::InputFloat3(_name, v);
  _param.x = v[0];
  _param.y = v[1];
  _param.z = v[2];
  return _param;
}

bool
UInterface::createInputVector3Clamp(const char* _name, Vector3& _param, float _min, float _max)
{
  bool change = createInputVector3(_name, _param);
  _param.clamp(_min, _max);
  return change;
}

bool
UInterface::createInputVector4(const char* _name, Vector4& _param)
{
  float v[4] = { _param.x, _param.y, _param.z, _param.w };
  bool change = ImGui::InputFloat4(_name, v);
  _param.x = v[0];
  _param.y = v[1];
  _param.z = v[2];
  _param.w = v[3];
  return change;
}

bool
UInterface::createInputVector4Clamp(const char* _name, Vector4& _param, float _min, float _max)
{
  bool change = createInputVector4(_name, _param);
  _param.clamp(_min, _max);
  return change;
}

void
UInterface::createSliderF(const char* _name, float& _param, const float _min, const float _max)
{
  ImGui::SliderFloat(_name, &_param, _min, _max);
}

void
UInterface::createSliderVector2(const char* _name,
                               Vector2& _param,
                               const float _min,
                               const float _max)
{
  float vector[2] = { _param.x, _param.y };
  ImGui::SliderFloat2(_name, vector, _min, _max);
  _param.x = vector[0];
  _param.y = vector[1];
}

Vector3
UInterface::createSliderVector3(const char* _name,
                               Vector3 _param,
                               const float _min,
                               const float _max)
{
  float vector[3] = { _param.x, _param.y, _param.z };
  ImGui::SliderFloat3(_name, vector, _min, _max);
  _param.x = vector[0];
  _param.y = vector[1];
  _param.z = vector[2];
  return _param;
}

void
UInterface::createCheckBox(const char* _name, bool& _param)
{
  ImGui::Checkbox(_name, &_param);
}

bool
UInterface::createButton(const char* _name)
{
  return ImGui::Button(_name);
}

void
UInterface::endWindowCreate()
{
  ImGui::End();
}

void*
UInterface::getDrawData()
{
  return ImGui::GetDrawData();
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