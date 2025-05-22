/*****************************************************************************/
/**
 * @file    pkInterface.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    01/04/2025
 * @brief
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
#include "pkWindow.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT Interface
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
   * @brief 
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
}