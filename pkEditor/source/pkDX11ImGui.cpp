#include "pkUInterface.h"
#include "pkGraphicsAPI.h"
#include "externals/imgui_impl_dx11.h"
#include "pkPlatformMath.h"
#include "pkTexture.h";

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

bool
UInterface::createDragF(const char* _name, float& _value, float _speed, float _min, float _max)
{
  return ImGui::DragFloat(_name, &_value, _speed, _min, _max);
}

bool
UInterface::createDrag3(const char* _name, Vector3& _value, float _speed, float _min, float _max)
{
  float value[3] = { _value.x, _value.y, _value.z };
  bool result = ImGui::DragFloat3(_name, value, _speed, _min, _max);
  _value.x = value[0];
  _value.y = value[1];
  _value.z = value[2];
  return result;
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

bool
UInterface::beginCombo(const char* _name, const char* _previewVal)
{
  return ImGui::BeginCombo(_name, _previewVal);
}

bool
UInterface::selectable(const char* _name, bool* _selected)
{
  return ImGui::Selectable(_name, _selected);
}

void
UInterface::endCombo()
{
  ImGui::EndCombo();
}

bool
UInterface::createButtonImage(const char* _name, SPtr<Texture>& _pTexture)
{
  // auto& dxTX = reinterpret_pointer_cast<DX11Texture>(_pTexture);
  return false;
}

bool
UInterface::colorEdit(const char* _name, Color& _color)
{
  float color[4] = { static_cast<float>(_color.getR()),
                     static_cast<float>(_color.getB()),
                     static_cast<float>(_color.getG()),
                     static_cast<float>(_color.getA())};
  bool result = ImGui::ColorEdit4(_name, color);
  _color.setA(static_cast<uint8>(color[3]));
  _color.setR(static_cast<uint8>(color[0]));
  _color.setG(static_cast<uint8>(color[1]));
  _color.setB(static_cast<uint8>(color[2]));
  return result;
}

bool
UInterface::colorEdit(const char* _name, Vector3& _color)
{
  float color[3] = { _color.x, _color.y, _color.z };
  bool result = ImGui::ColorEdit3(_name, color);
  _color.x = color[0];
  _color.y = color[1];
  _color.z = color[2];
  return result;
}

void
UInterface::plotLines(const char* _name,
                      float _values[],
                      uint32 _size,
                      int32 _valuesOffset,
                      const char* _overlayText,
                      float _scaleMin,
                      float _scaleMax)
{
  ImGui::PlotLines(_name, _values, _size, _valuesOffset, _overlayText, _scaleMin, _scaleMax);
}

bool
UInterface::beginChild(const char* _name)
{
  return ImGui::BeginChild(_name);
}

void
UInterface::endChild()
{
  ImGui::EndChild();
}

bool
UInterface::isHovered()
{
  return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
}

bool
UInterface::isHoveredWithItems()
{
  return (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::IsAnyItemActive());
}

void
UInterface::sameLine()
{
  ImGui::SameLine();
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