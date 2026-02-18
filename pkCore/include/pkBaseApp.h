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
#include "pkActor.h"
#include "pkPrerequisitesCore.h"
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
  init(const ANSICHAR** _argv, int32 _count);

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
  initAPI(const ANSICHAR** _argv, int32 _count);

  /**
   * @brief Event message loop.
   * @param _api API to use.
   */
  void
  messageLoop();

  /**
   * @brieg Update the base app.
   */
  void
  update();

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

  /**
   * @brief Set if the app should keep running or not.
   * @param _run Run state.
   */
  void
  ApplicationRun(const bool _run) { m_run = _run; }

 public:
  // system
  Window m_window;

  float m_fixedTimer;

 protected:
  SPtr<Actor> m_light;
  SPtr<Actor> m_camera;

  bool m_run = true;
  bool m_vSync = false;
  
  bool m_ssao = true;

  bool m_IBL = true;
  float m_IBLIntensity = 1.0f;

  float m_exposure = 1.0f;

  // luminance blur
  float m_blurRadius = 0.1f;
  float m_blurStrength = 1.0f;
  float m_lumThreshold = 6.0f;

  // emissive blur
  float m_emissiveBlurRadius = 5.0f;
  float m_emissiveStrength = 30.0f;

  // ssao parameters.
  float m_ssaoSampleRad = 3.0f;
  float m_ssaoScale = 1.0f;
  float m_ssaoBias = 0.01f;
  float m_ssaoIntensity = 2.0f;
};
}
