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
#include "pkVector2.h"
#include "pkVector3.h"

using pkEngineSDK::Actor;
using pkEngineSDK::BaseApp;
using pkEngineSDK::Camera;
using pkEngineSDK::CBView;
using pkEngineSDK::CBProjection;
using pkEngineSDK::CBTransform;
using pkEngineSDK::CBLight;
using pkEngineSDK::CBCamera;
using pkEngineSDK::make_shared;
using pkEngineSDK::ConstantBuffer;
using pkEngineSDK::Light;
using pkEngineSDK::Pass;
using pkEngineSDK::Scene;
using pkEngineSDK::SPtr;
using pkEngineSDK::Texture;
using pkEngineSDK::Vector;
using pkEngineSDK::Vector2;
using pkEngineSDK::Vector3;

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
   * @brief Application input. 
   */
  void
  input();

  /**
   * @brief Update the UI.
   */
  void
  UInterfaceUpdate();

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
  Actor* m_actor;
  Vector2 m_lastCursorPos;

  // light source
  SPtr<Actor> light;

  SPtr<Actor> m_camera;
  SPtr<Camera> m_pLightCam;
  // render targets
  SPtr<Texture> m_pRTDepth;
  SPtr<Texture> m_pRTNormal;
  SPtr<Texture> m_pRTColor;

  // SPtr<ConstantBuffer> m_cBView;
  // SPtr<ConstantBuffer> m_cBProjection;
  // SPtr<ConstantBuffer> m_cBTransform;
  // SPtr<ConstantBuffer> m_cbLight;
  // SPtr<ConstantBuffer> m_cbCamera;

  SPtr<Pass> m_basicPass;

  bool m_shadows;
  bool m_AO;
};