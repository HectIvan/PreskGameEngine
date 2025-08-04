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
#include "pkColor.h"
#include "externals/imgui.h"
#include "pkModule.h"
#include "pkPrerequisitesCore.h"
#include "pkTexture.h"
#include "pkVector2.h"
#include "pkWindow.h"

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
  createInputText(const char* _name, String* _param);

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
   * @brief Create an input Vector3 that returns the new value.
   * @param _name Name of the input.
   * @param _param Value to change.
   * @return New value.
   */
  Vector3
  createInputVector3Ret(const char* _name, Vector3 _param);

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
   * @brief Create a drag input.
   * @param _name Name of the item.
   * @param _value Value to modify.
   * @param _speed Modification speed.
   * @param _min Minimum value.
   * @param _max Maximum value.
   * @result If there was a change.
   */
  bool
  createDragF(const char* _name, float& _value, float _speed = 1.0f, float _min = -99999.9f,
              float _max = 99999.9f);

  /**
   * @brief Create a drag input.
   * @param _name Name of the item.
   * @param _value Value to modify.
   * @param _speed Modification speed.
   * @param _min Minimum value.
   * @param _max Maximum value.
   * @result If there was a change.
   */
  bool
  createDrag3(const char* _name, Vector3& _value, float _speed = 1.0f, float _min = -99999.9f,
              float _max = 99999.9f);

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
   * @param _transparent If the button color is supposed to be transparent.
   * @return If the button is clicked.
   */
  bool
  createButton(String _name,
               Color _normal = Color(0,0,0,0),
               Color _hover = Color(0,0,0,0),
               Color _active = Color(0,0,0,0),
               bool _newcolor = false);

  /**
   * @brief Start the creation of a dropdown combo.
   * @param _name Name of the dropdown.
   * @param _previewVal Preview value.
   * @return If the combo is opened.
   */
  bool
  beginCombo(const char* _name, const char* _previewVal);

  /**
   * @brief Create a selectable for a dropdown combo.
   * @param _name Name of the selectable.
   * @param _selected If the selectable is selected.
   * @return If it was selected.
   */
  bool
  selectable(const char* _name, bool* _selected = nullptr);

  /**
   * @brief End the creation of a dropdown combo.
   */
  void
  endCombo();

  /**
   * @brief create a button with an image.
   * @param _name Name of the button.
   * @param _texture Texture to use.
   * @param _size Size of the image.
   * @return If the button was pressed.
   */
  bool
  createButtonImage(const char* _name,
                    const SPtr<Texture>& _pTexture,
                    const Vector2 _size = Vector2(64));

  /**
   * @brief create an image.
   * @param _texture Texture to use.
   * @param _size Size of the image.
   */
  void
  createImage(const SPtr<Texture>& _pTexture, const Vector2 _size);

  /**
   * @brief Create a color editor.
   * @param _name Name of the editor.
   * @param _color Color to set.
   * @return Wether there was a change.
   */
  bool
  colorEdit(const char* _name, Color& _color);

  /**
   * @brief Create a color editor.
   * @param _name Name of the editor.
   * @param _color Color to set.
   * @return Wether there was a change.
   */
  bool
  colorEdit(const char* _name, Vector3& _color);

  /**
   * @brief Create a plot line.
   * @param _name Name.
   * @param _size Size of the array.
   * @param _valuesOffset Distance between values.
   * @param _overlayText Text overlayed on top.
   * @param _scaleMin Minimum Y size.
   * @param _scaleMax Maximum Y size.
   */
  void
  plotLines(const char* _name,
            float _values[],
            uint32 _size,
            int32 _valuesOffset,
            const char* _overlayText = nullptr,
            float _scaleMin = 0.0f,
            float _scaleMax = 500.0f);

  /**
   * @brief Create a child window.
   * @param _name Name of the window.
   * @return Wether it was successful.
   */
  bool
  beginChild(const char* _name);

  /**
   * @brief End the child window.
   */
  void
  endChild();

  /**
   * @brief Check if the cursor is on top of a window.
   * @return Hovered or not.
   */
  bool
  isHovered();

  /**
   * @brief Check if the cursor is on top of a window or item.
   * @return Hovered or not.
   */
  bool
  isHoveredWithItems();

  /**
   * @brief Check if an item is active.
   * @return Active or not.
   */
  bool
  isItemActive();

  /**
   * @brief Create 2 parts in the same line.
   */
  void
  sameLine();

  void
  pushID(uint32 _id);

  void
  popID();

  /**
   * @brief Set window background transparency.
   */
  void
  SetNextWindowAlpha(float _alpha);
  
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