/*****************************************************************************/
/**
 * @file    pkUInterface.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    01/04/2025
 * @brief   User Interface for the Editor app.
  *****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkUInterface.h"
#include "pkPlatformMath.h"

#define IMGUI_ENABLE_VIEWPORTS
#define IMGUI_HAS_DOCK

namespace pkEngineSDK
{

void
setPreskStyle()
{
  // Presk style from ImThemes
  ImGuiStyle& style = ImGui::GetStyle();

  style.Alpha = 0.8f;
  style.DisabledAlpha = 0.3f;
  style.WindowPadding = ImVec2(11.8f, 2.2f);
  style.WindowRounding = 0.0f;
  style.WindowBorderSize = 1.0f;
  style.WindowMinSize = ImVec2(20.0f, 32.0f);
  style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
  style.WindowMenuButtonPosition = ImGuiDir_Left;
  style.ChildRounding = 10.0f;
  style.ChildBorderSize = 1.0f;
  style.PopupRounding = 0.0f;
  style.PopupBorderSize = 1.0f;
  style.FramePadding = ImVec2(10.0f, 3.0f);
  style.FrameRounding = 3.6f;
  style.FrameBorderSize = 0.0f;
  style.ItemSpacing = ImVec2(1.4f, 2.5f);
  style.ItemInnerSpacing = ImVec2(1.4f, 2.5f);
  style.CellPadding = ImVec2(5.4f, 2.0f);
  style.IndentSpacing = 20.0f;
  style.ColumnsMinSpacing = 3.5f;
  style.ScrollbarSize = 10.0f;
  style.ScrollbarRounding = 0.0f;
  style.GrabMinSize = 10.0f;
  style.GrabRounding = 2.0f;
  style.TabRounding = 5.2f;
  style.TabBorderSize = 1.0f;
  // style.TabMinWidthForCloseButton = 0.0f;
  style.ColorButtonPosition = ImGuiDir_Left;
  style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
  style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

  style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
  style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.49803922f, 0.49803922f, 0.49803922f, 1.0f);
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0726114f, 0.07683488f, 0.07725322f, 0.94f);
  style.Colors[ImGuiCol_ChildBg] = ImVec4(1e-6f, 9.9999e-7f, 9.9999e-7f, 0.0f);
  style.Colors[ImGuiCol_PopupBg] = ImVec4(0.078431375f, 0.078431375f, 0.078431375f, 0.94f);
  style.Colors[ImGuiCol_Border] = ImVec4(0.6730646f, 0.6730678f, 0.87124467f, 0.5f);
  style.Colors[ImGuiCol_BorderShadow] = ImVec4(1e-6f, 9.9999e-7f, 9.9999e-7f, 0.0f);
  style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15686277f, 0.47843137f, 0.474291f, 0.54f);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.48971248f, 0.8573908f, 0.9055794f, 0.4f);
  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.0f, 0.93563676f, 1.0f, 0.67f);
  style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2735361f, 0.4151504f, 0.42489272f, 1.0f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2369541f, 0.57076293f, 0.57510734f, 1.0f);
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1e-6f, 9.9999e-7f, 9.9999e-7f, 0.51f);
  style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.07565068f, 0.15879828f, 0.15812226f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(1e-6f, 9.9999e-7f, 9.9999e-7f, 0.53f);
  style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30980393f, 0.30980393f, 0.30980393f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40784314f, 0.40784314f, 0.40784314f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.50980395f, 0.50980395f, 0.50980395f, 1.0f);
  style.Colors[ImGuiCol_CheckMark] = ImVec4(0.8423806f, 0.89561385f, 0.9527897f, 1.0f);
  style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.28160402f, 0.58143f, 0.6309013f, 1.0f);
  style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.38785022f, 0.74566793f, 0.80686694f, 1.0f);
  style.Colors[ImGuiCol_Button] = ImVec4(0.0f, 1.0f, 0.9613733f, 0.4f);
  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.7411765f, 1.0f, 0.9882353f, 0.5882353f);
  style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.7411765f, 1.0f, 0.9882353f, 0.78431374f);
  style.Colors[ImGuiCol_Header] = ImVec4(0.31759655f, 1.0f, 0.7100518f, 0.31f);
  style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.31764704f, 1.0f, 0.70297575f, 0.5882353f);
  style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.31764707f, 1.0f, 0.70980394f, 0.78431374f);
  style.Colors[ImGuiCol_Separator] = ImVec4(9.9999e-7f, 9.9999e-7f, 1e-6f, 0.5f);
  style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803923f, 0.6903476f, 0.7490196f, 0.78f);
  style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.11926909f, 0.75107294f, 0.7429378f, 1.0f);
  style.Colors[ImGuiCol_ResizeGrip] = ImVec4(9.9999e-7f, 9.999999e-7f, 1e-6f, 0.2f);
  style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.12997107f, 0.7134198f, 0.72103006f, 0.67f);
  style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.12941177f, 0.7137255f, 0.72156864f, 0.9490196f);
  style.Colors[ImGuiCol_Tab] = ImVec4(0.10243328f, 0.28755367f, 0.28040308f, 0.862f);
  style.Colors[ImGuiCol_TabHovered] = ImVec4(0.11140381f, 0.54077256f, 0.531637f, 0.8627451f);
  style.Colors[ImGuiCol_TabActive] = ImVec4(0.19607842f, 0.6722208f, 0.6784314f, 1.0f);
  style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667f, 0.14004883f, 0.14509805f, 0.9724f);
  style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13333336f, 0.42352942f, 0.41979298f, 1.0f);
  style.Colors[ImGuiCol_PlotLines] = ImVec4(1.0f, 0.99999f, 0.99999f, 1.0f);
  style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.19495662f, 0.54077256f, 0.5363201f, 1.0f);
  style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.14890677f, 0.73819745f, 0.7306093f, 1.0f);
  style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.0f, 1.0f, 0.98712444f, 1.0f);
  style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.28283814f, 0.28284124f, 0.35622317f, 1.0f);
  style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.30980393f, 0.34548515f, 0.34901962f, 1.0f);
  style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.16810034f, 0.3111664f, 0.33476394f, 1.0f);
  style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
  style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.35f);
  style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.0f, 1.0f, 0.98712444f, 0.9f);
  style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.25882354f, 0.85634935f, 0.9764706f, 1.0f);
  style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
  style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
  style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
}
  
void
UInterface::init(const WindowHandle& _hWnd)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); static_cast<void>(io);
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | io.ConfigWindowsMoveFromTitleBarOnly;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // initalize the UI with the desired Graphic API
  initWithAPI();

  // ImGui::StyleColorsDark();
  ImGui::StyleColorsClassic();
  setPreskStyle();

  // initialize with the window.
  initWin(_hWnd);
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
UInterface::createSpace()
{
  // ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  // ImGuiViewport* viewport = ImGui::GetMainViewport();

  // ImGuiID dockspace_id = ImGui::GetID("MainDockspace");

  static bool built = false;
  if (!built) {
    // ImGui::dockBuilderRemoveNode(dockspace_id); // clear any previous layout
    // ImGui::dockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None); // add empty node
    // ImGui::dockBuilderSetNodeSize(dockspace_id, viewport->Size);

    // ImGuiID main = dockspace_id;
    // ImGuiID left;
    // ImGuiID right;
    // ImGuiID bottom;
    // ImGuiID center;

    // ImGui::dockBuilderSplitNode(main, ImGuiDir_Left, 0.2f, &left, &main);
    // ImGui::dockBuilderSplitNode(main, ImGuiDir_Right, 0.2f, &right, &main);
    // ImGui::dockBuilderSplitNode(main, ImGuiDir_Down, 0.2f, &bottom, &center);
    // 
    // ImGui::dockBuilderDockWindow("Scene Graph", left);
    // ImGui::dockBuilderDockWindow("Inspector", right);
    // ImGui::dockBuilderDockWindow("Logger", bottom);
    // ImGui::dockBuilderDockWindow("Viewport", center);
    // 
    // ImGui::dockBuilderFinish(dockspace_id);
  }
  built = true;


  ImGui::DockSpaceOverViewport(ImGui::GetID("MainDockspace"), ImGui::GetMainViewport());
}

bool
UInterface::beginTable(const ANSICHAR* _name, const int32 _columns)
{
  return ImGui::BeginTable(_name, _columns);
}

void
UInterface::tableSetupColumn(const ANSICHAR* _name)
{
  ImGui::TableSetupColumn(_name);
}

void
UInterface::tableHeadersRow()
{
  ImGui::TableHeadersRow();
}

void
UInterface::tableNextRow()
{
  ImGui::TableNextRow();
}

void
UInterface::tableSetColumnIndex(const uint32 _index)
{
  ImGui::TableSetColumnIndex(_index);
}

void UInterface::tableJumpRow()
{
  tableNextRow();
  tableNextColumn();
}

void
UInterface::tableNextColumn()
{
  ImGui::TableNextColumn();
}

void
UInterface::endTable()
{
  ImGui::EndTable();
}

bool
UInterface::beginTabBar(const ANSICHAR* _name)
{
  return ImGui::BeginTabBar(_name);
}

void
UInterface::endTabBar()
{
  ImGui::EndTabBar();
}

bool
UInterface::beginDragDropSource()
{
  return ImGui::BeginDragDropSource();
}

void
UInterface::setDragDropPayload(const ANSICHAR* _name, const void* _data, const SIZE_T _dataSize)
{
  ImGui::SetDragDropPayload(_name, _data, _dataSize);
}

void
UInterface::endDragDropSource()
{
  ImGui::EndDragDropSource();
}

bool
UInterface::beginDragDropTarget()
{
  return ImGui::BeginDragDropTarget();
}

void*
UInterface::acceptDragDropPayload(const ANSICHAR* _type)
{
  const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_type);
  if (payload) {
    return payload->Data;
  }
  return nullptr;
}

void
UInterface::endDragDropTarget()
{
  ImGui::EndDragDropTarget();
}

bool
UInterface::beginTabItem(const ANSICHAR* _name)
{
  return ImGui::BeginTabItem(_name);
}

void
UInterface::endTabItem()
{
  ImGui::EndTabItem();
}

void
UInterface::startWindowCreate(const ANSICHAR* _name)
{
  ImGui::Begin(_name, nullptr, ImGuiWindowFlags_HorizontalScrollbar);
}

void
UInterface::startWindowCreate(const UIWindow& _winDesc)
{
  ImGui::SetNextWindowSize(ImVec2(_winDesc.size.x, _winDesc.size.y), ImGuiCond_Always);
  ImGui::SetNextWindowPos(ImVec2(_winDesc.position.x, _winDesc.position.y), ImGuiCond_Always);
  ImGui::Begin(_winDesc.name, nullptr, ImGuiWindowFlags_HorizontalScrollbar);
}

void
UInterface::createText(const ANSICHAR* _text)
{
  ImGui::Text(_text);
}

// static int32
// InputTextCallback(ImGuiInputTextCallbackData* data)
// {
//   if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
//   {
//     auto str = static_cast<String*>(data->UserData);
//     str->resize(data->BufTextLen);
//     data->Buf = str->data();
//   }
//   return 0;
// }

bool
UInterface::createInputText(const ANSICHAR* _name, String* _param)
{
  _param->resize(256);
  const bool changed = ImGui::InputText(_name, _param->data(), _param->size());
  _param->resize(strlen(_param->data()));
  return changed;
}

bool
UInterface::createInputF(const ANSICHAR* _name, float& _param, float _step, float _largeStep)
{
  return ImGui::InputFloat(_name, &_param, _step, _largeStep);
}

bool
UInterface::createInputUInt32(const ANSICHAR* _name,
                              uint32& _param,
                              const uint32 _step,
                              const uint32 _largeStep)
{
  return ImGui::InputInt(_name, reinterpret_cast<int32*>(&_param), _step, _largeStep);
}

bool
UInterface::createInputFClamp(const ANSICHAR* _name,
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
UInterface::createInputVector2(const ANSICHAR* _name, Vector2& _param)
{
  float v[2] = { _param.x, _param.y };
  const bool change = ImGui::InputFloat2(_name, v);
  _param.x = v[0];
  _param.y = v[1];
  return change;
}

bool UInterface::createInputVector2Clamp(const ANSICHAR* _name,
                                         Vector2& _param,
                                         float _min,
                                         float _max)
{
  const bool change = createInputVector2(_name, _param);
  _param.clamp(_min, _max);
  return change;
}

bool
UInterface::createInputVector3(const ANSICHAR* _name, Vector3& _param)
{
  float v[3] = { _param.x, _param.y, _param.z };
  const bool change = ImGui::InputFloat3(_name, v);
  _param.x = v[0];
  _param.y = v[1];
  _param.z = v[2];
  return change;
}

Vector3
UInterface::createInputVector3Ret(const ANSICHAR* _name, Vector3 _param)
{
  float v[3] = { _param.x, _param.y, _param.z };
  ImGui::InputFloat3(_name, v);
  _param.x = v[0];
  _param.y = v[1];
  _param.z = v[2];
  return _param;
}

bool
UInterface::createInputVector3Clamp(const ANSICHAR* _name,
                                    Vector3& _param,
                                    float _min,
                                    float _max)
{
  const bool change = createInputVector3(_name, _param);
  _param.clamp(_min, _max);
  return change;
}

bool
UInterface::createInputVector4(const ANSICHAR* _name, Vector4& _param)
{
  float v[4] = { _param.x, _param.y, _param.z, _param.w };
  const bool change = ImGui::InputFloat4(_name, v);
  _param.x = v[0];
  _param.y = v[1];
  _param.z = v[2];
  _param.w = v[3];
  return change;
}

bool
UInterface::createInputVector4Clamp(const ANSICHAR* _name,
                                    Vector4& _param,
                                    float _min,
                                    float _max)
{
  const bool change = createInputVector4(_name, _param);
  _param.clamp(_min, _max);
  return change;
}

void
UInterface::createSliderF(const ANSICHAR* _name,
                          float& _param,
                          const float _min,
                          const float _max)
{
  ImGui::SliderFloat(_name, &_param, _min, _max);
}

void
UInterface::createSliderVector2(const ANSICHAR* _name,
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
UInterface::createSliderVector3(const ANSICHAR* _name,
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
UInterface::createDrag(const ANSICHAR* _name,
                        uint32& _value,
                        const uint32 _speed,
                        const uint32 _min,
                        const uint32 _max)
{
  return ImGui::DragInt(_name, reinterpret_cast<int32*>(&_value), _speed, _min, _max);
}

bool
UInterface::createDrag(const ANSICHAR* _name,
                        int32& _value,
                        const int32 _speed,
                        const int32 _min,
                        const int32 _max)
{
  return ImGui::DragInt(_name, &_value, _speed, _min, _max);
}

bool
UInterface::createDrag(const ANSICHAR* _name,
                        float& _value,
                        float _speed,
                        float _min,
                        float _max)
{
  return ImGui::DragFloat(_name, &_value, _speed, _min, _max);
}

bool
UInterface::createDrag(const ANSICHAR* _name,
                        Vector3& _value,
                        float _speed,
                        float _min,
                        float _max)
{
  float value[3] = { _value.x, _value.y, _value.z };
  const bool result = ImGui::DragFloat3(_name, value, _speed, _min, _max);
  _value.x = value[0];
  _value.y = value[1];
  _value.z = value[2];
  return result;
}

void
UInterface::createCheckBox(const ANSICHAR* _name, bool& _param)
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
    const FColor tC = _normal.toFColor();
    const FColor hC = _hover.toFColor();
    const FColor cC = _active.toFColor();
    const ImVec4 tColor = ImVec4(tC.r, tC.g, tC.b, tC.a);
    const ImVec4 hColor = ImVec4(hC.r, hC.g, hC.b, hC.a);
    const ImVec4 cColor = ImVec4(cC.r, cC.g, cC.b, cC.a);

    ImGui::PushStyleColor(ImGuiCol_Button, tColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, cColor);
    const bool result = ImGui::Button(_name.c_str());
    ImGui::PopStyleColor(3);
    return result;
  }
  return ImGui::Button(_name.c_str());
}

Vector2
UInterface::getItemPosition()
{
  const ImVec2 topLeft = ImGui::GetItemRectMin();
  return Vector2(topLeft.x, topLeft.y);
}

Vector2 UInterface::getItemSize()
{
  const ImVec2 topLeft = ImGui::GetItemRectMin();
  const ImVec2 bottomRight = ImGui::GetItemRectMax();

  return Vector2(bottomRight.x - topLeft.x, bottomRight.y - topLeft.y);
}

bool
UInterface::beginCombo(const ANSICHAR* _name,
                       int32& _previewVal,
                       const Vector<String>& _options)
{
  // make a string of const ANSICHAR*
  Vector<const ANSICHAR*> cstrs;
  for (const auto& option : _options) {
    cstrs.push_back(option.c_str());
  }

  return ImGui::Combo(_name, &_previewVal, cstrs.data(), cstrs.size());
}

bool
UInterface::selectable2(const ANSICHAR* _name,
                        const Vector2 _size,
                        Color _base,
                        Color _hover,
                        Color _active)
{
  PushStyleColor(_base, _hover, _active);
  const bool result = ImGui::Selectable(_name, false, 0, ImVec2(_size.x, _size.y));
  ImGui::PopStyleColor(3);
  return result;
}

bool
UInterface::selectable(const ANSICHAR* _name,
                       const String _selected,
                       Color _base,
                       Color _hover,
                       Color _active)
{
  PushStyleColor(_base, _hover, _active);
  const bool result = ImGui::Selectable(_name, _selected == _name);
  ImGui::PopStyleColor(3);
  return result;
}

bool
UInterface::beginPopUpItem(const ANSICHAR* _name)
{
  return ImGui::BeginPopupContextItem(_name);
}

bool
UInterface::menuItem(const ANSICHAR* _name)
{
  return ImGui::MenuItem(_name);
}

void
UInterface::endPopUpItem()
{
  ImGui::EndPopup();
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
UInterface::createButtonImage(const ANSICHAR* _name,
                              const SPtr<Texture>& _pTexture,
                              const Vector2 _size)
{
  if (_pTexture) {
    const ImTextureID texture = reinterpret_cast<ImTextureID>(_pTexture->getRawData());
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
    const ImTextureID texture = reinterpret_cast<ImTextureID>(_pTexture->getRawData());
    ImGui::Image(texture, ImVec2(_size.x, _size.y));
  }
}

bool
UInterface::colorEdit(const ANSICHAR* _name, FColor& _color)
{
  float color[4] = { _color.r, _color.g, _color.b, _color.a };
  const bool result = ImGui::ColorEdit4(_name, color);
  _color.r = color[0];
  _color.g = color[1];
  _color.b = color[2];
  _color.a = color[3];
  return result;
}

bool
UInterface::colorEdit(const ANSICHAR* _name, Color& _color)
{
  FColor fColor = _color.toFColor();
  const bool result = colorEdit(_name, fColor);
  _color = fColor.toColor();
  return result;
}

bool
UInterface::colorEdit(const ANSICHAR* _name, Vector3& _color)
{
  FColor fColor = FColor(_color.x, _color.y, _color.z);
  const bool result = colorEdit(_name, fColor);
  _color = Vector3(fColor.r, fColor.g, fColor.b);
  return result;
}

void
UInterface::plotLines(const ANSICHAR* _name,
                      const float _values[],
                      const uint32& _size,
                      const int32& _valuesOffset,
                      const ANSICHAR* _overlayText,
                      const float& _scaleMin,
                      const float& _scaleMax)
{
  ImGui::PlotLines(_name, _values, _size, _valuesOffset, _overlayText, _scaleMin, _scaleMax);
}

bool
UInterface::collapsingHeader(const ANSICHAR* _name, PK_TREENODE_FLAGS::E _flags)
{
  return ImGui::CollapsingHeader(_name, static_cast<ImGuiTreeNodeFlags>(_flags));
}

void
UInterface::PushStyleColor(const Color& _mainColor,
                           const Color& _hoverColor,
                           const Color& _activeColor)
{
  // convert from [0, 255] range to [0, 1]
  const FColor mainCol = _mainColor.toFColor();
  const FColor hoverCol = _hoverColor.toFColor();
  const FColor activeCol = _activeColor.toFColor();
  // convert to ImVec4
  const ImVec4 header = ImVec4(mainCol.r, mainCol.g, mainCol.b, mainCol.a);
  const ImVec4 hover = ImVec4(hoverCol.r, hoverCol.g, hoverCol.b, hoverCol.a);
  const ImVec4 active = ImVec4(activeCol.r, activeCol.g, activeCol.b, activeCol.a);
  // push styles
  ImGui::PushStyleColor(ImGuiCol_Header, header);
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered, hover);
  ImGui::PushStyleColor(ImGuiCol_HeaderActive, active);
}

void
UInterface::popStyleColor(const uint32& _count)
{
  ImGui::PopStyleColor(_count);
}

bool
UInterface::beginChild(const ANSICHAR* _name, Vector2 _size, const bool& _border)
{
  // if the size is zero (default) use the parent size.
  if (_size == Vector2::ZERO) {
    _size = m_winSize;
  }
  return ImGui::BeginChild(_name, ImVec2(_size.x, _size.y), _border);
}

bool
UInterface::beginChild(const ANSICHAR* _name, float _x, float _y, const bool _border)
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
UInterface::setTooltip(const ANSICHAR* _text)
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
}

void
UInterface::endWindowCreate()
{
  ImGui::End();
}

Vector2
UInterface::getWindowSize()
{
  const ImVec2 size = ImGui::GetWindowSize();
  return Vector2(size.x, size.y);
}

Vector2
UInterface::getWindowPos()
{
  const ImVec2 pos = ImGui::GetWindowPos();
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
