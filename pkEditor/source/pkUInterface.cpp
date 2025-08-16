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

namespace pkEngineSDK
{
  
void
UInterface::init()
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); static_cast<void>(io);
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

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
UInterface::setNewWindowSize(Vector2 _size)
{
  ImGui::SetNextWindowSize(ImVec2(_size.x, _size.y), ImGuiCond_Always);
}

void
UInterface::setNewWindowSize(float _x, float _y)
{
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
  ImGui::SetNextWindowPos(ImVec2(_pos.x, _pos.y), ImGuiCond_Always);
}


Vector2
UInterface::getDisplaySize()
{
  ImGuiIO& io = ImGui::GetIO();
  return Vector2(io.DisplaySize.x, io.DisplaySize.y);
}

UInterface&
g_uInterface()
{
  return UInterface::instance();
}
}
