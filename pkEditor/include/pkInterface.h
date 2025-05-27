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
#include "pkVector2.h"

namespace pkEngineSDK
{
  
class PK_CORE_EXPORT Interface : public Module<Interface>
{
 public:
  Interface() = default;
  virtual ~Interface() = default;

  // /**
  //  * @brief Initialize the Interface for the editor
  //  */
  // void
  // init(const WindowHandle& _hWnd);
  // 
  // /**
  //  * @brief Initialize Imgui through the current api.
  //  */
  // void
  // initAPI();
  // 
  // /**
  //  * @brief initialize the interface with the desired API.
  //  */
  // void
  // windowNewFrame();
  // 
  // /**
  //  * @brief
  //  */
  // void
  // windowShutDown();
  // 
  // /**
  //  * @brief 
  //  */
  // void
  // newFrameAPI();
  // 
  // /**
  //  * brief 
  //  */
  // void
  // endFrame();
  // 
  // /**
  //  * @brief Creates a new interface window.
  //  * @param _name Name of the window.
  //  */
  // void
  // startWindowCreate(const char* _name);
  // 
  // /**
  //  * @brief Create a text in the interface window.
  //  * @param _text What text to display.
  //  */
  // void
  // createText(const char* _text);
  // 
  // /**
  //  * @brief Create a float slider.
  //  * @param _name Name of the slider.
  //  * @param _param What parameter is to be modified.
  //  * @param _min Minimum value the parameter can have.
  //  * @param _max Maximum value the parameter can have.
  //  */
  // void
  // createSliderF(const char* _name, float& _param, const float _min, const float _max);
  // 
  // /**
  //  * @brief Create a Vector2 slider.
  //  * @param _name Name of the slider.
  //  * @param _param What parameter is to be modified.
  //  * @param _min Minimum value the parameter can have.
  //  * @param _max Maximum value the parameter can have.
  //  */
  // void
  // createSliderVector2(const char* _name, Vector2& _param, const float _min, const float _max);
  // 
  // /**
  //  * @brief Create a Vector3 slider.
  //  * @param _name Name of the slider.
  //  * @param _param What parameter is to be modified.
  //  * @param _min Minimum value the parameter can have.
  //  * @param _max Maximum value the parameter can have.
  //  */
  // void
  // createSliderVector3(const char* _name, Vector3& _param, const float _min, const float _max);
  // 
  // /**
  //  * @brief Create a checkbox.
  //  * @param _name Name of the checkbox.
  //  * @param _param What parameter to change.
  //  */
  // void
  // createCheckBox(const char* _name, bool& _param);
  // 
  // /**
  //  * @brief End the window creation process.
  //  */
  // void
  // endWindowCreate();
  // 
  // /**
  //  * @brief 
  //  */
  // void*
  // getDrawData(); 
  // 
  // /**
  //  * @brief 
  //  */
  // void
  // render();
};

// PK_CORE_EXPORT Interface&
// g_interface();
}