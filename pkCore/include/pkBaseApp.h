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
#include "pkCBuffers.h"
#include "pkEventQueue.h"
#include "pkLight.h"
#include "pkPrerequisitesCore.h"
#include "pkRendererManager.h"
#include "pkGPUResourceManager.h"
#include "pkTimeManager.h"
#include "pkWindow.h"

using pkEngineSDK::PASS_TYPE::kP_Base;
using pkEngineSDK::PASS_TYPE::kP_EmissiveHBlur;
using pkEngineSDK::PASS_TYPE::kP_EmissiveBlur;
using pkEngineSDK::PASS_TYPE::kP_IBL;
using pkEngineSDK::PASS_TYPE::kP_Shadow;
using pkEngineSDK::PASS_TYPE::kP_ShadowQuad;
using pkEngineSDK::PASS_TYPE::kP_SkyBox;
using pkEngineSDK::PASS_TYPE::kP_SSAO;
using pkEngineSDK::PASS_TYPE::kP_Tone;
using pkEngineSDK::PASS_TYPE::kP_Merge;
using pkEngineSDK::PASS_TYPE::kP_Luminance;
using pkEngineSDK::PASS_TYPE::kP_LumBlurH;
using pkEngineSDK::PASS_TYPE::kP_LumBlur;

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
  ApplicationRun(bool _run) { m_run = _run; }

 public:
  // system
  Window m_window;

  float m_fixedTimer;

 protected:
  bool m_run = true;
  bool m_vSync;

  bool m_ssao = true;
  bool m_IBL = true;
};
}
