/*****************************************************************************/
/**
 * @file    EditorApp.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    13/01/2025
 * @brief   Editor derived app.
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
#include "ActorInspector.h"
#include "pkActor.h"
#include "pkBaseApp.h"
#include "pkCamera.h"
#include "pkConstantBuffer.h"
#include "pkLight.h"
#include "pkPass.h"
#include "pkPrerequisitesCore.h"
#include "pkScene.h"
#include "pkPath.h"
#include "pkTexture.h"
#include "pkUInterface.h"
#include "pkVector2.h"
#include "pkVector3.h"
#include "pkWindowDesc.h"
#include "MaterialInspector.h"
#include "ResourceInspector.h"
#include "SceneInspector.h"

using pkEngineSDK::Texture;
using pkEngineSDK::BaseApp;
using pkEngineSDK::g_uInterface;
using pkEngineSDK::Path;
using pkEngineSDK::PK_TREENODE_FLAGS::kPK_DefaultOpen;
using pkEngineSDK::Shader;
using pkEngineSDK::UInterface;
using pkEngineSDK::UIWindow;
using pkEngineSDK::Vector;
using pkEngineSDK::Vector2;

struct ShaderType
{
  Path path;
  String name;
  SPtr<Shader> shader;
};

class EditorApp : public BaseApp
{
 public:
   EditorApp() = default;
  virtual ~EditorApp() = default;

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
  SPtr<Material> m_selectedMaterial;
  Vector2 m_lastCursorPos = Vector2(0.0f);

  Vector<float> m_fpsList;
  uint32 m_fpsSize = 20;

  // camera sensitivity
  float m_sensX = 1.0f;
  float m_sensY = 1.0f;
  
  // camera movement speed
  float m_cameraSpeed = 20.0f;

  // low window log/resources search.
  bool m_currentTab;
  bool m_showErrors = true;
  bool m_showWarnings = false;
  bool m_showActions = false;

  UIWindow m_sceneGraphWin;
  UIWindow m_loggerWin;
  UIWindow m_rightWin;

  SPtr<Texture> m_eyeIcon;

  ActorInspector m_actorInspector;
  MaterialInspector m_materialInspector;
  ResourceInspector m_resourceInspector;
  SceneInspector m_sceneInspector;
};
