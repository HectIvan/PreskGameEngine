#pragma once

#include "externals/imgui.h"
#include "pkPrerequisitesCore.h"
#include "pkWindow.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT ImguiEngine
{
public:
  ImguiEngine() = default;
  virtual ~ImguiEngine() = default;

  /**
   * @brief Initialize ImGui for the engine
   */
  void
  init(WindowHandle& _hWnd);
};
}