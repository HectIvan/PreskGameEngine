/*****************************************************************************/
/**
 * @file    pkInterface.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    01/04/2025
 * @brief   Interface for the Editor app.
 *
 * @bug    Interface class is fundamentally wrong.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "externals/imgui.h"
#include "pkPrerequisitesCore.h"
#include "pkModule.h"
#include "pkWindow.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT Interface : public Module<Interface>
{
 public:
  Interface() = default;
  virtual ~Interface() = default;

  /**
   * @brief Initialize the Interface for the editor
   */
  void
  init(const WindowHandle& _hWnd);

  /**
   * @brief Initialize Imgui through the current api.
   */
  void
  initAPI();

  /**
   * @brief initialize the interface with the desired API.
   */
  void
  windowNewFrame();

  /**
   * @brief 
   */
  void
  newFrameAPI();

  /**
   * @brief 
   */
  void
  render();
};

PK_CORE_EXPORT Interface&
g_interface();
}