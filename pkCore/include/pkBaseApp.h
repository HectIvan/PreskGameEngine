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
#include "pkModelManager.h"
#include "pkTimeManager.h"
#include "pkWindow.h"

using pkEngineSDK::PASS_TYPE::kP_Base;
using pkEngineSDK::PASS_TYPE::kP_EmissiveHBlur;
using pkEngineSDK::PASS_TYPE::kP_EmissiveBlur;
using pkEngineSDK::PASS_TYPE::kP_LightPositions;
using pkEngineSDK::PASS_TYPE::kP_Light;
using pkEngineSDK::PASS_TYPE::kP_LightTransparency;
using pkEngineSDK::PASS_TYPE::kP_SkyBox;
using pkEngineSDK::PASS_TYPE::kP_SSAO;
using pkEngineSDK::PASS_TYPE::kP_Tone;
using pkEngineSDK::PASS_TYPE::kP_Transparency;
using pkEngineSDK::PASS_TYPE::kP_Luminance;
using pkEngineSDK::PASS_TYPE::kP_LumBlurH;
using pkEngineSDK::PASS_TYPE::kP_LumBlur;

using pkEngineSDK::PK_ROT_TYPE::kDegrees;
using pkEngineSDK::PK_ROT_TYPE::kRadians;

using pkEngineSDK::Actor;
using pkEngineSDK::Camera;
using pkEngineSDK::CBBlur;
using pkEngineSDK::CBCamera;
using pkEngineSDK::CBFloat;
using pkEngineSDK::CBLight;
using pkEngineSDK::CBProjection;
using pkEngineSDK::CBSSAO;
using pkEngineSDK::CBTransform;
using pkEngineSDK::CBVector2x2;
using pkEngineSDK::CBVector3;
using pkEngineSDK::CBView;
using pkEngineSDK::Color;
using pkEngineSDK::ConstantBuffer;
using pkEngineSDK::D_BUFFERS::kDB_Base;
using pkEngineSDK::D_BUFFERS::kDB_Light;
using pkEngineSDK::EventQueue;
using pkEngineSDK::ModelManager;
using pkEngineSDK::G_BUFFERS::kGB_Albedo;
using pkEngineSDK::G_BUFFERS::kGB_Normal;
using pkEngineSDK::g_EventManager;
using pkEngineSDK::g_ModelManager;
using pkEngineSDK::g_RenderManager;
using pkEngineSDK::g_TimeManager;
using pkEngineSDK::int32;
using pkEngineSDK::Light;
using pkEngineSDK::Material;
using pkEngineSDK::Matrix4;
using pkEngineSDK::make_shared;
using pkEngineSDK::Model;
using pkEngineSDK::Pass;
using pkEngineSDK::Path;
using pkEngineSDK::PlatformPointer;
using pkEngineSDK::PKWindowDesc;
using pkEngineSDK::RendererManager;
using pkEngineSDK::Scene;
using pkEngineSDK::Shader;
using pkEngineSDK::SPtr;
using pkEngineSDK::String;
using pkEngineSDK::Texture;
using pkEngineSDK::TimeManager;
using pkEngineSDK::to_string;
using pkEngineSDK::uint32;
using pkEngineSDK::Vector;
using pkEngineSDK::Vector2;
using pkEngineSDK::Vector3;
using pkEngineSDK::Vector4;

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
  bool m_run = true;
  bool m_vSync;

  bool m_ssao = true;
  bool m_IBL = true;
};
}
