/*****************************************************************************/
/**
 * @file    pkUInterface.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    01/04/2025
 * @brief   User Interface for the Editor app.
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkUInterface.h"
#include "pkPlatformMath.h"

namespace pkEngineSDK
{
  
void
UInterface::init()
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); static_cast<void>(io);
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | io.ConfigWindowsMoveFromTitleBarOnly;
  // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags)
  {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // initalize the UI with the desired Graphic API
  initWithAPI();

  // ImGui::StyleColorsDark();
  ImGui::StyleColorsClassic();
}

void
UInterface::setCurrentContext()
{
  ImGui::SetCurrentContext(ImGui::GetCurrentContext());
}

void
UInterface::setNewWindowSize(Vector2 _size)
{
  m_winSize = _size;
  ImGui::SetNextWindowSize(ImVec2(_size.x, _size.y), ImGuiCond_Always);
}

void
UInterface::setNewWindowSize(float _x, float _y)
{
  m_winSize = Vector2(_x, _y);
  ImGui::SetNextWindowSize(ImVec2(_x, _y), ImGuiCond_Always);
}

void
UInterface::uINewFrame()
{
  ImGui::NewFrame();
}

void
UInterface::setNextWindowPos(Vector2 _pos)
{
  m_winPos = _pos;
  ImGui::SetNextWindowPos(ImVec2(_pos.x, _pos.y), ImGuiCond_Always);
}

void
UInterface::setNextWindowPos(float _x, float _y)
{
  m_winPos = Vector2(_x, _y);
  ImGui::SetNextWindowPos(ImVec2(_x, _y), ImGuiCond_Always);
}

Vector2
UInterface::getDisplaySize()
{
  ImGuiIO& io = ImGui::GetIO();
  return Vector2(io.DisplaySize.x, io.DisplaySize.y);
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

static int
InputTextCallback(ImGuiInputTextCallbackData* data)
{
  if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
  {
    auto str = static_cast<String*>(data->UserData);
    str->resize(data->BufTextLen);
    data->Buf = str->data();
  }
  return 0;
}

bool
UInterface::createInputText(const char* _name, String* _param)
{
  return ImGui::InputText(_name, _param->data(), _param->capacity() + 1);
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
UInterface::createButton(String _name,
  Color _normal,
  Color _hover,
  Color _active,
  bool _newcolor)
{
  if (_name.empty()) {
    _name = "--Default--";
  }

  if (_newcolor) {
    bool result = false;
    Vector4 tC = _normal.colorTo01();
    Vector4 hC = _hover.colorTo01();
    Vector4 cC = _active.colorTo01();
    ImVec4 tColor = ImVec4(tC.x, tC.y, tC.y, tC.w);
    ImVec4 hColor = ImVec4(hC.x, hC.y, hC.y, hC.w);
    ImVec4 cColor = ImVec4(cC.x, cC.y, cC.y, cC.w);

    ImGui::PushStyleColor(ImGuiCol_Button, tColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, cColor);
    result = ImGui::Button(_name.c_str());
    ImGui::PopStyleColor(3);
    return result;
  }
  return ImGui::Button(_name.c_str());
}

bool
UInterface::beginCombo(const char* _name,
                       int32& _previewVal,
                       const Vector<String>& _options)
{
  // make a string of const chars*
  Vector<const char*> cstrs;
  for (const auto& option : _options) {
    cstrs.push_back(option.c_str());
  }

  if (ImGui::Combo(_name, &_previewVal, cstrs.data(), cstrs.size())) {
    return true;
  }
  return false;
}

bool
UInterface::selectable(const char* _name, const String _selected)
{
  return ImGui::Selectable(_name, _selected == _name);
}

void
UInterface::endCombo()
{
  ImGui::EndCombo();
}

void
UInterface::pushID(uint32 _id)
{
  ImGui::PushID(_id);
}

void
UInterface::popID()
{
  ImGui::PopID();
}



bool
UInterface::createButtonImage(const char* _name,
                              const SPtr<Texture>& _pTexture,
                              const Vector2 _size)
{
  if (_pTexture) {
    ImTextureID texture = reinterpret_cast<ImTextureID>(_pTexture->getRawData());
    ImVec2 size = ImVec2(_pTexture->getWidth(), _pTexture->getHeight());
    String name = "##" + String(_name);
    return ImGui::ImageButton(name.c_str(), texture, ImVec2(_size.x, _size.y));
  }
  return false;
}

void
UInterface::createImage(const SPtr<Texture>& _pTexture, const Vector2 _size)
{
  if (_pTexture) {
    ImTextureID texture = reinterpret_cast<ImTextureID>(_pTexture->getRawData());
    ImGui::Image(texture, ImVec2(_size.x, _size.y));
  }
}

bool
UInterface::colorEdit(const char* _name, Color& _color)
{
  float color[4] = { static_cast<float>(_color.getR()),
                     static_cast<float>(_color.getB()),
                     static_cast<float>(_color.getG()),
                     static_cast<float>(_color.getA()) };
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
UInterface::collapsingHeader(const char* _name, PK_TREENODE_FLAGS::E _flags)
{
  return ImGui::CollapsingHeader(_name, static_cast<ImGuiTreeNodeFlags>(_flags));
}

void
UInterface::PushStyleColor(Color _mainColor, Color _hoverColor, Color _activeColor)
{
  // convert from [0, 255] range to [0, 1]
  Vector4 mainCol = _mainColor / 255.0f;
  Vector4 hoverCol = _hoverColor / 255.0f;
  Vector4 activeCol = _activeColor / 255.0f;
  // convert to ImVec4
  ImVec4 header = ImVec4(mainCol.x, mainCol.y, mainCol.z, mainCol.w);
  ImVec4 hover = ImVec4(hoverCol.x, hoverCol.y, hoverCol.z, hoverCol.w);
  ImVec4 active = ImVec4(activeCol.x, activeCol.y, activeCol.z, activeCol.w);
  // push styles
  ImGui::PushStyleColor(ImGuiCol_Header, header);
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered, hover);
  ImGui::PushStyleColor(ImGuiCol_HeaderActive, active);
}

void
UInterface::popStyleColor(uint32 _count)
{
  ImGui::PopStyleColor(_count);
}

bool
UInterface::beginChild(const char* _name, Vector2 _size, const bool _border)
{
  // if the size is zero (default) use the parent size.
  if (_size == Vector2::ZERO) {
    _size = m_winSize;
  }
  return ImGui::BeginChild(_name, ImVec2(_size.x, _size.y), _border);
}

bool
UInterface::beginChild(const char* _name, float _x, float _y, const bool _border)
{
  // if one of the values is 0, it will be replaced by the size of the parent window.
  if (_x == 0.0f) { _x = m_winSize.x; }
  if (_y == 0.0f) { _y = m_winSize.y; }
  return ImGui::BeginChild(_name, ImVec2(_x, _y), _border);
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
UInterface::isItemHovered()
{
  return ImGui::IsItemHovered();
}

void
UInterface::setTooltip(const char* _text)
{
  ImGui::SetTooltip(_text);
}

bool
UInterface::isHoveredWithItems()
{
  return (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::IsAnyItemActive());
}

bool
UInterface::isItemActive()
{
  return ImGui::IsAnyItemActive();
}

void
UInterface::sameLine()
{
  ImGui::SameLine();
}

void
UInterface::setNextWindowAlpha(float _alpha)
{
  ImGui::SetNextWindowBgAlpha(_alpha);
}

void
UInterface::setNextWindowParams(UIWindow& _windowPrefab)
{
  setNewWindowSize(_windowPrefab.size);
  setNextWindowPos(_windowPrefab.position);
  setNextWindowAlpha(_windowPrefab.alpha);
}

void
UInterface::endWindowCreate()
{
  ImGui::End();
}

Vector2
UInterface::getWindowSize()
{
  ImVec2 size = ImGui::GetWindowSize();
  return Vector2(size.x, size.y);
}

Vector2
UInterface::getWindowPos()
{
  ImVec2 pos = ImGui::GetWindowPos();
  return Vector2(pos.x, pos.y);
}

void*
UInterface::getDrawData()
{
  return ImGui::GetDrawData();
}

UInterface&
g_uInterface()
{
  return UInterface::instance();
}
}
