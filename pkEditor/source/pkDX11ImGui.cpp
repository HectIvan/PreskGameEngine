#include "pkUInterface.h"
#include "pkGraphicsAPI.h"
#include "externals/imgui_impl_dx11.h"
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
UInterface::setCurrentContext()
{
  ImGui::SetCurrentContext(ImGui::GetCurrentContext());
}

void
UInterface::newFrameAPI()
{
  ImGui_ImplDX11_NewFrame();
}

bool
UInterface::createButtonImage(const char* _name, SPtr<Texture>& _pTexture, Vector2 _size)
{
  ImTextureID texture = reinterpret_cast<ImTextureID>(_pTexture->getRawData());
  ImVec2 size = ImVec2(_pTexture->getWidth(), _pTexture->getHeight());
  String name = "##" + String(_name);
  return ImGui::ImageButton(name.c_str(), texture, ImVec2(_size.x, _size.y));
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
UInterface::SetNextWindowAlpha(float _alpha)
{
  ImGui::SetNextWindowBgAlpha(_alpha);
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