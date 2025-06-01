/*****************************************************************************/
/**
 * @file    pkUInterface.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    01/04/2025
 * @brief   User Interface for the Editor app.
 *
 * @bug     No known bugs.
 * 
 * to do: fix the window proportion issue
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
#include "pkVector2.h"

namespace pkEngineSDK
{

class UInterface : public Module<UInterface>
{
 public:
   UInterface() = default;
  virtual ~UInterface() = default;

  /**
   * @brief Initialize the User Interface for the editor
   */
  void
  init();
  
  /**
   * @brief Initialize the User Interface through the current api.
   */
  void
  initAPI();

  /**
   * @brief Initialize imgui with the window.
   * @param _hWnd Handle to the window to use.
   */
  void
  initWin(const WindowHandle& _hWnd);

  /**
   * @brief set the position of the window being created.
   * @param _pos Window position.
   */
  void
  setNextWindowPos(Vector2 _pos);

  /**
   * @brief Set the size of the window being created.
   * @param _size Window size.
   */
  void
  setNewWindowSize(Vector2 _size);
  
  void
  uINewFrame();

  /**
   * @brief initialize the User interface with the desired API.
   */
  void
  windowNewFrame();
  
  /**
   * @brief
   */
  void
  windowShutDown();

  /**
   * @brief Set the current context for the UI.
   */
  void
  setCurrentContext();
  
  /**
   * @brief 
   */
  void
  newFrameAPI();
  
  /**
   * brief 
   */
  void
  endFrame();
  
  /**
   * @brief Creates a new User interface window.
   * @param _name Name of the window.
   */
  void
  startWindowCreate(const char* _name);
  
  /**
   * @brief Create a text in the User interface window.
   * @param _text What text to display.
   */
  void
  createText(const char* _text);
  
  /**
   * @brief Create a float slider.
   * @param _name Name of the slider.
   * @param _param What parameter is to be modified.
   * @param _min Minimum value the parameter can have.
   * @param _max Maximum value the parameter can have.
   */
  void
  createSliderF(const char* _name, float& _param, const float _min, const float _max);
  
  /**
   * @brief Create a Vector2 slider.
   * @param _name Name of the slider.
   * @param _param What parameter is to be modified.
   * @param _min Minimum value the parameter can have.
   * @param _max Maximum value the parameter can have.
   */
  void
  createSliderVector2(const char* _name, Vector2& _param, const float _min, const float _max);
  
  /**
   * @brief Create a Vector3 slider.
   * @param _name Name of the slider.
   * @param _param What parameter is to be modified.
   * @param _min Minimum value the parameter can have.
   * @param _max Maximum value the parameter can have.
   */
  void
  createSliderVector3(const char* _name, Vector3& _param, const float _min, const float _max);
  
  /**
   * @brief Create a checkbox.
   * @param _name Name of the checkbox.
   * @param _param What parameter to change.
   */
  void
  createCheckBox(const char* _name, bool& _param);
  
  /**
   * @brief End the window creation process.
   */
  void
  endWindowCreate();

  /**
   * @brief Get the io display size.
   * @return Size of the display.
   */
  Vector2
  getDisplaySize();
  
  /**
   * @brief 
   */
  void*
  getDrawData(); 
  
  /**
   * @brief 
   */
  void
  render();
};

UInterface&
g_uInterface();
}