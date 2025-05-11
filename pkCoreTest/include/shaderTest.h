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
#include "pkPass.h"
#include "pkScene.h"
#include "pkTexture.h"
#include "pkVector2.h"
#include "pkVector3.h"

using pkEngineSDK::Actor;
using pkEngineSDK::BaseApp;
using pkEngineSDK::Pass;
using pkEngineSDK::Scene;
using pkEngineSDK::SPtr;
using pkEngineSDK::Texture;
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
  // render targets
  SPtr<Texture> m_pRTDepth;
  SPtr<Texture> m_pRTNormal;
  SPtr<Texture> m_pRTColor;

  SPtr<Pass> m_basicPass;
};