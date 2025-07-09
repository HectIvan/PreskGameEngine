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
  initWithAPI();

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

  /**
   * @brief Set the size of the window being created.
   * @param _x Window width.
   * @param _y Window height.
   */
  void
  setNewWindowSize(float _x, float _y);

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
   * @brief Create an input text.
   * @param _name Name of the input.
   * @param _param Value to change.
   * @return Wether there was a change or not.
   */
  bool
  createInputText(const char* _name, String& _param);

  /**
   * @brief Create an input float.
   * @param _name Name of the input.
   * @param _param Value to change.
   * @param _step How much to increment when an arrow is clicked.
   * @param _largeStep How much to increment when an arrow is held down.
   * @return Wether there was a change or not.
   */
  bool
  createInputF(const char* _name,
               float& _param,
               float _step = 5.0f,
               float _largeStep = 10.0f);

  /**
   * @brief Create an input float.
   * @param _name Name of the input.
   * @param _param Value to change.
   * @param _min Minimum value.
   * @param _max Maximum value.
   * @param _step How much to increment when an arrow is clicked.
   * @param _largeStep How much to increment when an arrow is held down.
   * @return Wether there was a change or not.
   */
  bool
  createInputFClamp(const char* _name,
                    float& _param,
                    float _min,
                    float _max,
                    float _step = 5.0f,
                    float _largeStep = 10.0f);

  /**
   * @brief Create an input Vector2.
   * @param _name Name of the input.
   * @param _param Value to change.
   * @return Wether there was a change or not.
   */
  bool
  createInputVector2(const char* _name, Vector2& _param);

  /**
   * @brief Create an input Vector2 with clamp.
   * @param _name Name of the input.
   * @param _param Value to change.
   * @param _min Minimum value.
   * @param _max Maximum value.
   * @return Wether there was a change or not.
   */
  bool
  createInputVector2Clamp(const char* _name, Vector2& _param, float _min, float _max);

  /**
   * @brief Create an input Vector3.
   * @param _name Name of the input.
   * @param _param Value to change.
   * @return Wether there was a change or not.
   */
  bool
  createInputVector3(const char* _name, Vector3& _param);

  /**
   * @brief Create an input Vector3 with clamp.
   * @param _name Name of the input.
   * @param _param Value to change.
   * @param _min Minimum value.
   * @param _max Maximum value.
   * @return Wether there was a change or not.
   */
  bool
  createInputVector3Clamp(const char* _name, Vector3& _param, float _min, float _max);

  /**
   * @brief Create an input Vector4.
   * @param _name Name of the input.
   * @param _param Value to change.
   * @return Wether there was a change or not.
   */
  bool
  createInputVector4(const char* _name, Vector4& _param);

  /**
   * @brief Create an input Vector4 with clamp.
   * @param _name Name of the input.
   * @param _param Value to change.
   * @param _min Minimum value.
   * @param _max Maximum value.
   * @return Wether there was a change or not.
   */
  bool
  createInputVector4Clamp(const char* _name, Vector4& _param, float _min, float _max);

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
  Vector3
  createSliderVector3(const char* _name, Vector3 _param, const float _min, const float _max);

  /**
   * @brief Create a checkbox.
   * @param _name Name of the checkbox.
   * @param _param What parameter to change.
   */
  void
  createCheckBox(const char* _name, bool& _param);

  /**
   * @brief Create a Button.
   * @param _name Name of the button.
   */
  bool
  createButton(const char* _name);
  
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