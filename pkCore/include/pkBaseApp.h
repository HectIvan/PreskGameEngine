/************************************************************************/
/**
* @pkBaseApp pkBaseApp.h
* @Hector Ivan Muñoz Ceballos
* @date 07/12/2024
* @Base app for the Presk Game Engine.
*
* This file contains the Base App of the engine
*
* @bug No bug known.
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
#include "pkEventQueue.h"
#include "pkPrerequisitesCore.h"
#include "pkWindow.h"

namespace pkEngineSDK
{

class GraphicsAPI;

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