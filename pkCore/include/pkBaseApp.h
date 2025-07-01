/*************************************************************************/
/**
 * @file    pkBaseApp.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    07/12/2024
 * @brief   Base app for the Presk Game Engine
 *
 * @bug    no knon bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkCamera.h"
#include "pkEventQueue.h"
#include "pkLight.h"
#include "pkPrerequisitesCore.h"
#include "pkRendererManager.h"
#include "pkResourceManager.h"
#include "pkTimeManager.h"
#include "pkWindow.h"

namespace pkEngineSDK
{

class Camera;

class PK_CORE_EXPORT BaseApp
{
 public:
  BaseApp() = default;
  virtual ~BaseApp() = default;

  /**
   * @brief Initialize the app.
   */
  void
  init(const char** _argv, int32 _count);

  /**
   * @brief On initializing the app.
   */
  virtual void
  onInit() {}

  /**
   * @brief Initialize the window app.
   */
  virtual void
  initWin() {};

  /**
   * @brief Initialize the api.
   * @param _argv
   * @param _count Ammount of parameters.
   */
  void
  initAPI(const char** _argv, int32 _count);

  /**
   * @brief Event message loop.
   * @param _api API to use.
   */
  void
  messageLoop();

  /**
   * @brief Update function for any child class of this base app
   */
  virtual void
  onUpdate() {}

  /**
   * @brief Fixed update of the engine
   */
  virtual void
  fixedUpdate() {}

  /**
   * @brief Base app render function.
   */
  void
  render();

  /**
   * @brief Render function for any child class of this base class
   */
  virtual void
  onRender() {}

 public:
  // system
  Window m_window;
  // EventQueue m_eventQueue;

  // camera movement speed
  float m_cameraSpeed;
  float m_baseCamSpeed;
  float m_camAccelerate;
  float m_maxCamSpeed;

  // fixed update
  float m_deltaTime = 0.0f;
  float m_fixedTimer = 0.0f;
  float m_fixedDeltaTime = 0.016f;

 protected:
  bool m_vSync;

  bool m_shadows;
  bool m_AO;
  bool m_luminance;
};
}
