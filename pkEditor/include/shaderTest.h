/*****************************************************************************/
/**
 * @file    pkShaderTest.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    13/01/2025
 * @brief   Shader specific derived class.
 *
 * @bug    No known bugs.
 */
/*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkActor.h"
#include "pkBaseApp.h"
#include "pkCamera.h"
#include "pkConstantBuffer.h"
#include "pkLight.h"
#include "pkPass.h"
#include "pkPrerequisitesCore.h"
#include "pkScene.h"
#include "pkTexture.h"
#include "pkUInterface.h"
#include "pkVector2.h"
#include "pkVector3.h"
#include "pkWindowDesc.h"

using pkEngineSDK::Actor;
using pkEngineSDK::BaseApp;
using pkEngineSDK::Camera;
using pkEngineSDK::CBView;
using pkEngineSDK::CBProjection;
using pkEngineSDK::CBTransform;
using pkEngineSDK::CBLight;
using pkEngineSDK::CBCamera;
using pkEngineSDK::int32;
using pkEngineSDK::make_shared;
using pkEngineSDK::ConstantBuffer;
using pkEngineSDK::Light;
using pkEngineSDK::Pass;
using pkEngineSDK::Path;
using pkEngineSDK::Scene;
using pkEngineSDK::Shader;
using pkEngineSDK::SPtr;
using pkEngineSDK::String;
using pkEngineSDK::Texture;
using pkEngineSDK::uint32;
using pkEngineSDK::UIWindow;
using pkEngineSDK::Vector;
using pkEngineSDK::Vector2;
using pkEngineSDK::Vector3;

struct ShaderType
{
  Path path;
  String name;
  SPtr<Shader> shader;
};

class ShaderTest : public BaseApp
{
 public:
  ShaderTest() = default;
  virtual ~ShaderTest() = default;

  /**
   * @brief Initialize the shader test.
   */
  void
  onInit() override;

  /**
   * @brief Initialize Editor window.
   */
  void
  initWin() override;

  /**
   * @brief Application input. 
   */
  void
  input();

  /**
   * @brief Update the UI.
   */
  void
  uInterfaceUpdate();

  /**
   * @brief Show the log type in the UI.
   */
  void
  showLogType(bool& _active, uint32 _type);

  /**
   * @brief Update the shader test.
   * @param _deltatime Time between frames.
   */
  void
  onUpdate() override;

  /**
   * @brief On render specific app render function.
   * @param _scene Scene to render
   */
  void
  onRender() override;

 public:
  SPtr<Actor> m_selectedActor;
  int32 m_sActorIndex;
  Vector2 m_lastCursorPos;

  Vector<float> m_fpsList;
  uint32 m_fpsSize;

  // light source
  SPtr<Actor> m_light;
  SPtr<Actor> m_camera;

  // camera sensitivity
  float m_sensX;
  float m_sensY;
  float m_blurRadius;
  float m_blurStrength;
  float m_lumThreshold;
  // camera movement speed
  float m_cameraSpeed;

  // emissive blur
  float m_emissiveBlur;
  float m_emissiveStrength;

  // UI texture image size
  float m_imgTextureSize;

  // low window log/resources search.
  bool m_currentTab;
  bool m_showErrors;
  bool m_showWarnings;
  bool m_showActions;

  // ssao parameters.
  float m_ssaoSampleRad = 3.0f;
  float m_ssaoScale = 1.0f;
  float m_ssaoBias = 0.01f;
  float m_ssaoIntensity = 2.0f;

  float m_IBRIntensity;
  float m_exposure;


  String m_searchMesh;

  UIWindow m_sceneGraphWin;
  UIWindow m_loggerWin;
  UIWindow m_rightWin;

  SPtr<Texture> m_eyeIcon;
};
