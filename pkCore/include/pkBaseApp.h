/************************************************************************/
/**
* @pkBaseApp pkBaseApp.h
* @Hector Ivan Muñoz Ceballos
* @date 07/12/2024
* @Base app for the Presk Game Engine.
*
* This file contains the Base App of the engine
*
* @bug.
* User input for the camera seems to not process.
* 
* @HectIvan 09/12/2024
* Added a render and messageLoop functions. 
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include <chrono>

#include "pkEventQueue.h"
#include "pkPrerequisitesCore.h"
#include "pkWindow.h"

namespace pkEngineSDK
{

class GraphicsAPI;

using std::chrono::high_resolution_clock;
using std::chrono::duration;

class PK_CORE_EXPORT BaseApp
{
 public:
  BaseApp() = default;
  virtual ~BaseApp() = default;

  /**
  * Initialize the app.
  **/
  void
  init(const char** _argv);

  /**
  * Initialize the app window.
  **/
  void
  initWindow();

  /**
  * Initialize the api.
  **/
  void
  initAPI(const char** _argv);

  /**
  * Event message loop.
  * 
  * @param _api
  * API to use.
  **/
  void
  messageLoop(GraphicsAPI* _api);

  /**
  * Gets the time elapsed between frames.
  * 
  * @return
  * The deltaTime.
  **/
  float
  getDeltaTime(high_resolution_clock::time_point& _delta);

  /**
  * Render the scene.
  **/
  void
  render(GraphicsAPI* _api);

public:
  Window window;
  EventQueue eventQueue;

  float cameraSpeed;
};
}