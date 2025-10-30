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
#include "imgui.h"
#include "pkModule.h"
#include "pkPrerequisitesCore.h"
#include "pkTexture.h"
#include "pkVector2.h"
#include "pkWindow.h"

namespace pkEngineSDK
{

// Extracted from imgui.h
namespace PK_TREENODE_FLAGS 
{
  enum E {
    kPK_None = 0,
    kPK_Selected = 1 << 0,   // Draw as selected
    kPK_Framed = 1 << 1,   // Draw frame with background (e.g. for CollapsingHeader)
    kPK_AllowOverlap = 1 << 2,   // Hit testing to allow subsequent widgets to overlap this one
    kPK_NoTreePushOnOpen = 1 << 3,   // Don't do a TreePush() when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack
    kPK_NoAutoOpenOnLog = 1 << 4,   // Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes)
    kPK_DefaultOpen = 1 << 5,   // Default node to be open
    kPK_OpenOnDoubleClick = 1 << 6,   // Open on double-click instead of simple click (default for multi-select unless any _OpenOnXXX behavior is set explicitly). Both behaviors may be combined.
    kPK_OpenOnArrow = 1 << 7,   // Open when clicking on the arrow part (default for multi-select unless any _OpenOnXXX behavior is set explicitly). Both behaviors may be combined.
    kPK_Leaf = 1 << 8,   // No collapsing, no arrow (use as a convenience for leaf nodes).
    kPK_Bullet = 1 << 9,   // Display a bullet instead of arrow. IMPORTANT: node can still be marked open/close if you don't set the _Leaf flag!
    kPK_FramePadding = 1 << 10,  // Use FramePadding (even for an unframed text node) to vertically align text baseline to regular widget height. Equivalent to calling AlignTextToFramePadding() before the node.
    kPK_SpanAvailWidth = 1 << 11,  // Extend hit box to the right-most edge, even if not framed. This is not the default in order to allow adding other items on the same line without using AllowOverlap mode.
    kPK_SpanFullWidth = 1 << 12,  // Extend hit box to the left-most and right-most edges (cover the indent area).
    kPK_SpanTextWidth = 1 << 13,  // Narrow hit box + narrow hovering highlight, will only cover the label text.
    kPK_SpanAllColumns = 1 << 14,  // Frame will span all columns of its container table (text will still fit in current column)
    kPK_NavLeftJumpsBackHere = 1 << 15,  // (WIP) Nav: left direction may move to this TreeNode() from any of its child (items submitted between TreeNode and TreePop)
  };
}

struct UIWindow {
  Vector2 size;
  Vector2 position;
  const char* name;
  float alpha;

  void
  setNewSizePos(Vector2 _newPos, Vector2 _newSize, Vector2 _winRect) {
    Vector2 newPos = _newPos;
    // newPos.clampEach(0, _winRect.x, 0, size.y);
    Vector2 newSize = _newSize;
    newSize.clampEach(1, _winRect.x, 1, _winRect.y);

    position = newPos;
    size = newSize;
  }
};

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
   * @brief set the position of the window being created.
   * @param _x Window position in X.
   * @param _y Window position in Y.
   */
  void
  setNextWindowPos(float _x, float _y);

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
   * @brief set the docking space to the current viewport.
   */
  void
  createSpace();

  /**
   * @brief Begin a tab bar in the UI.
   * @param _name Name of the tab bar.
   * @return Wether the tab bar was created or not.
   */
  bool
  beginTabBar(const char* _name);

  /**
   * @brief End a tab bar in the UI.
   */
  void
  endTabBar();

  /**
   * @brief Begin the drag and drop process.
   * @return If the drag and drop has been initiated.
   */
  bool
  beginDragDropSource();

  /**
   * @brief Set what data will be dragged.
   * @param _name Name of the payload.
   * @param _data Payload Data.
   * @param _dataSize Data Size.
   */
  void
  setDragDropPayload(const char* _name, const void* _data, const SIZE_T _dataSize);

  /**
   * @brief End the drag and drop process.
   */
  void
  endDragDropSource();

  /**
   * @brief Begin the drag drop target.
   */
  void
  beginDragDropTarget();

  /**
   * @brief Accept a drag drop payload.
   * @param _type Payload type.
   */
  bool
  acceptDragDropPayload(const char* _type);

  /**
   * @brief End the drag drop process.
   */
  void
  endDragDropTarget();

  /**
   * @brief Create a selectable widget.
   * @param _name Name of the widget.
   * @param _size Size of the widget.
   * @return If clicked this frame.
   */
  bool
  selectable(const char* _name, const Vector2 _size = Vector2(0.0f));

  /**
   * @brief Begin a UI tab.
   * @param _name Name of the tab.
   * @return Wether the tab is open or not.
   */
  bool
  beginTabItem(const char* _name);

  /**
   * @brief end the UI tab.
   */
  void
  endTabItem();

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
   * @param _options All options to use.
   */
  bool
  beginCombo(const char* _name, int32& _previewVal, const Vector<String>& _options);

  /**
   * @brief Create a selectable for a dropdown combo.
   * @param _name Name of the selectable.
   * @param _selected option to search for.
   * @return If it was selected.
   */
  bool
  selectable(const char* _name, const String _selected);

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
   * @brief Create a collapsing header.
   * @param _name Name of the header.
   * @param _flags Tree node flags (default at 0).
   * @return If the header is open.
   */
  bool
  collapsingHeader(const char* _name,
                   PK_TREENODE_FLAGS::E _flags = PK_TREENODE_FLAGS::kPK_None);

  /**
   * @brief Push a color style.
   * @param _mainColor Main color.
   * @param _hoverColor Color when hovering.
   * @param _activeColor Color when active.
   */
  void
  PushStyleColor(Color _mainColor, Color _hoverColor, Color _activeColor);

  /**
   * @brief Pop style colors.
   * @param _count Ammount of colors pushed.
   */
  void
  popStyleColor(uint32 _count);

  /**
   * @brief Create a child window.
   * @param _name Name of the window.
   * @param _size Size of the window.
   * @param _border Will it have a border.
   * @return Wether it was opened.
   */
  bool
  beginChild(const char* _name,
             Vector2 _size = Vector2(0.0f, 0.0f),
             const bool _border = true);

  /**
   * @brief Create a child window.
   * @param _name Name of the window.
   * @param _x Width of the window (if 0.0f, it will use the parent width).
   * @param _y Height of the window (if 0.0f, it will use the parent height).
   * @param _border Will it have a border.
   * @return Wether it was opened.
   */
  bool
  beginChild(const char* _name,
             float _x = 0.0f,
             float _y = 0.0f,
             const bool _border = true);

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
   * @brief Check if the item is hovered.
   * @return Hovered.
   */
  bool
  isItemHovered();

  /**
   * @brief Add tooltip to item.
   * @param _text Text to display.
   */
  void
  setTooltip(const char* _text);

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
  setNextWindowAlpha(float _alpha);

  /**
   * @brief Set parameters for the next window.
   */
  void
  setNextWindowParams(UIWindow& _windowPrefab);
  
  /**
   * @brief End the window creation process.
   */
  void
  endWindowCreate();

  /**
   * @brief Get the window size.
   * @return Size.
   */
  Vector2
  getWindowSize();

  /**
   * @brief Get the window position.
   * @return Position.
   */
  Vector2
  getWindowPos();

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

 public:
  Vector2 m_winSize;
  Vector2 m_winPos;
};

UInterface&
g_uInterface();
}