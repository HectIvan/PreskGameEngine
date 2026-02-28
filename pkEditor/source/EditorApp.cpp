#include "pkAssetResourceManager.h"
#include "pkColor.h"
#include "pkModelManager.h"
#include "pkGraphicsAPI.h"
#include "pkGraphicTypes.h"
#include "pkLogger.h"
#include "pkModelCodec.h"
#include "pkModelResource.h"
#include "pkPlatformMath.h"
#include "pkRendererManager.h"
#include "pkSceneManager.h"
#include "pkShaderManager.h"
#include "pkTextureCodec.h"
#include "pkTextureManager.h"
#include "pkTimeManager.h"
#include "EditorApp.h"
#include "pkMaterialManager.h"
#include "pkEventQueue.h"
#include "TransformInspector.h"

using pkEngineSDK::Quaternion;
using pkEngineSDK::BaseResource;
using pkEngineSDK::Camera;
using pkEngineSDK::CameraDesc;
using pkEngineSDK::Color;
using pkEngineSDK::EventQueue;
using pkEngineSDK::g_EventManager;
using pkEngineSDK::g_Logger;
using pkEngineSDK::g_ModelManager;
using pkEngineSDK::g_RenderManager;
using pkEngineSDK::g_SceneManager;
using pkEngineSDK::g_ShaderManager;
using pkEngineSDK::g_TextureManager;
using pkEngineSDK::g_TimeManager;
using pkEngineSDK::int32;
using pkEngineSDK::Light;
using pkEngineSDK::Logger;
using pkEngineSDK::LogMSG;
using pkEngineSDK::LOG_MSG_TYPE::E;
using pkEngineSDK::LOG_MSG_TYPE::kError;
using pkEngineSDK::LOG_MSG_TYPE::kWarning;
using pkEngineSDK::LOG_MSG_TYPE::kLog;
using pkEngineSDK::make_shared;
using pkEngineSDK::PK_ROT_TYPE::kDegrees;
using pkEngineSDK::Math;
using pkEngineSDK::Model;
using pkEngineSDK::ModelManager;
using pkEngineSDK::ModelResource;
using pkEngineSDK::PlatformPointer;
using pkEngineSDK::PKWindowDesc;
using pkEngineSDK::TextureManager;
using pkEngineSDK::to_string;
using pkEngineSDK::RendererManager;
using pkEngineSDK::SceneManager;
using pkEngineSDK::Shader;
using pkEngineSDK::ShaderManager;
using pkEngineSDK::UUID;
using pkEngineSDK::Vector3;

#if PK_PLATFORM == PK_PLATFORM_WIN32
#include "pkWindow.h"

// Win32 message handler your application need to call.
// - Intentionally commented out in a '#if 0' block to avoid dragging dependencies on <windows.h> from this helper.
// - You should COPY the line below into your .cpp code to forward declare the function and then you can call it.
// - Call from your application's message handler. Keep calling your message handler unless this function returns TRUE.

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
                                                             UINT msg,
                                                             WPARAM wParam,
                                                             LPARAM lParam);
#endif

void
EditorApp::onInit()
{
  //start the interface
  UInterface::startUp();
  g_uInterface().init(m_window.getWindowHandle());

  SceneManager& sceneMan = g_SceneManager();
  SPtr<Scene> activeScene = sceneMan.getActiveScene();
  m_sceneInspector.setScene(activeScene);

  // create camera
  const Vector3 camPos = Vector3(0.0f, 0.0f, -30.0f);
  CameraDesc camDescription;
  camDescription.width   = m_window.getWidth();
  camDescription.height  = m_window.getHeight();
  camDescription.eye     = camPos;

  m_camera = activeScene->instantiate("Main Camera");
  m_camera->setPosition(camPos);
  m_camera->addComponent(make_shared<Camera>(camDescription));

  // create light
  const Vector3 lightPos = Vector3(0.0f, 1000.0f, 0.0f);
  CameraDesc lightCamDesc;
  lightCamDesc.width   = 1920 * 2.0f;
  lightCamDesc.height  = 1080 * 2.0f;
  lightCamDesc.eye     = lightPos;

  m_light = activeScene->instantiate("Light");
  m_light->setPosition(lightPos);
  m_light->setRotation(90.0f, 0.0f, 0.0f);
  m_light->addComponent(make_shared<Light>());
  m_light->addComponent(make_shared<Camera>(lightCamDesc));

  // m_eyeIcon = g_TextureManager().loadTexture(Path("resources/white-eye-icon.pkt"));

  /**
   * User Interface.
   */
  // scene graph
  const Vector2 winRect = m_window.getClientWidthHeight();
  m_sceneGraphWin.name = activeScene->m_name.c_str();
  m_sceneGraphWin.position = Vector2(0.0f, 0.0f);
  m_sceneGraphWin.size = Vector2(winRect.x * 0.1f, winRect.y * 0.8f);
  // logger window
  m_loggerWin.name = "Logger";
  m_loggerWin.size = Vector2(winRect.x, winRect.y * 0.2f);
  m_loggerWin.position = Vector2(0.0f, winRect.y * 0.8f);
  // right window
  m_rightWin.name = "Inspector";
  m_rightWin.size = Vector2(400.0f, winRect.y * 0.8f);
  m_rightWin.position = Vector2(winRect.x - 400.0f, 0.0f);
}

void
EditorApp::initWin()
{
  PKWindowDesc desc;
  desc.width = 1920;
  desc.height = 1080;
  desc.posX = 0;
  desc.posY = 0;
  desc.name = "Presk Editor App";
  // imgui input
#if PK_PLATFORM == PK_PLATFORM_WIN32
  desc.funct = [](PlatformPointer _hwnd,
                  int32 _msg,
                  PlatformPointer _wParam,
                  PlatformPointer _lParam) {
    LRESULT result = ImGui_ImplWin32_WndProcHandler(reinterpret_cast<HWND>(_hwnd),
                                                    _msg,
                                                    reinterpret_cast<WPARAM>(_wParam),
                                                    reinterpret_cast<LPARAM>(_lParam));
    return reinterpret_cast<PlatformPointer>(result);
  };
#endif
  m_window.create(desc);
}

void
EditorApp::input()
{
  EventQueue& eventQueue = g_EventManager();
  UInterface& im = g_uInterface();
  // bool interfaceHovered = im.isHoveredWithItems();
  const bool itemActive = im.isItemActive();
  const float deltaTime = g_TimeManager().m_deltaTime;
  const float speed = m_cameraSpeed * deltaTime;
  // if the user wants to exit the app.
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kEsc)) {
    ApplicationRun(false);
  }
  // move forward/backward
  if (!itemActive) {
    if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kW)) {
      m_camera->moveForwardLocal(speed);
    }
    if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kS)) {
      m_camera->moveForwardLocal(-speed);
    }
    // move left/right
    if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kA)) {
      m_camera->moveRightLocal(-speed);
    }
    if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kD)) {
      m_camera->moveRightLocal(speed);
    }
    // move up/down
    if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kE)) {
      m_camera->moveUpLocal(speed);
    }
    if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kQ)) {
      m_camera->moveUpLocal(-speed);
    }
    // rotate camera
    if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kLButton)) {
      Vector2 posDif = (m_lastCursorPos - eventQueue.mousePosition) * Math::DEG2RAD;
      posDif *= Vector2(m_sensX, m_sensY);
      m_camera->rotate(posDif.x, posDif.y, 0.0f);
    }
  }
  m_lastCursorPos = eventQueue.mousePosition;
}

void
EditorApp::uInterfaceUpdate()
{
  Logger& log = g_Logger();
  ModelManager& modelMan = g_ModelManager();
  RendererManager& rm = g_RenderManager();
  SceneManager& sm = g_SceneManager();
  ShaderManager& shaderMan = g_ShaderManager();
  TextureManager& tm = g_TextureManager();
  UInterface& im = g_uInterface();

  im.setCurrentContext();
  im.newFrameAPI();
  im.windowNewFrame();
  im.uINewFrame();

  const Vector2 winRect = m_window.getClientWidthHeight();
  const SPtr<Scene> currentScene = sm.getActiveScene();

  // --- Scene graph window --- //
  im.setNextWindowParams(m_sceneGraphWin);
  im.startWindowCreate(m_sceneGraphWin.name);
  m_sceneGraphWin.setNewSizePos(im.getWindowPos(), im.getWindowSize(), winRect);

  m_sceneInspector.setScene(currentScene);
  m_sceneInspector.createSceneGraphWindow();
  im.endWindowCreate();
  // -------------------------- //

  // -------------------------- //
  // Create log window
  im.setNextWindowParams(m_loggerWin);
  im.startWindowCreate(m_loggerWin.name);
  SPtr<Actor> selectedActor = m_sceneInspector.getActor();
  m_actorInspector.setActor(selectedActor);
  m_loggerWin.setNewSizePos(im.getWindowPos(), im.getWindowSize(), winRect);

  // Vector2 logWinSize = im.getItemSize(); // to do: there is an error when getting the height of the window.

  // -------------------------- //
  im.sameLine();
  // -------------------------- //
  // logger window.
  if (im.beginTabBar("Logger/resources")) {
    if (im.beginTabItem("Logger")) {
      im.createCheckBox("Errors", m_showErrors);
      im.sameLine();
      im.createCheckBox("Warnings", m_showWarnings);
      im.sameLine();
      im.createCheckBox("Logs", m_showActions);
      im.sameLine();
      if (im.createButton("Clear")) {
        if (m_showErrors) {
          log.clearLogsOfType(kError);
        }
        if (m_showWarnings) {
          log.clearLogsOfType(kWarning);
        }
      }
      showLogType(m_showErrors, kError);
      showLogType(m_showWarnings, kWarning);
      showLogType(m_showActions, kLog);
      im.endTabItem();
    }
    // resources window.
    if (im.beginTabItem("Resources")) {
      m_resourceInspector.createResourceWindow(m_window);
    }
    im.endTabBar();
  }
  // -------------------------- //

  im.endWindowCreate();

  // --- Transform window --- //
  im.setNextWindowParams(m_rightWin);
  im.startWindowCreate(m_rightWin.name);
  m_rightWin.setNewSizePos(im.getWindowPos(), im.getWindowSize(), winRect);
  if (im.beginTabBar("InspectorTab")) {
    // -------------------------- //
    // Actor tab
    // -------------------------- //
    if (selectedActor && im.beginTabItem("Actor")) {
      // transform window
      if (im.collapsingHeader("Transform", kPK_DefaultOpen)) {
        String name = selectedActor->getName();
        im.createText("Name:   ");
        im.sameLine();
        if (im.createInputText("##Name", &name)) {
          selectedActor->setName(name);
        }
        // activity checkbox
        im.sameLine();
        im.createCheckBox("##ActiveActor", selectedActor->isActive());
        if (m_eyeIcon) {
          im.sameLine();
          im.createImage(m_eyeIcon, Vector2(15));
        }
        // inspect actor transform matrix
        TransformInspector::inspect(selectedActor);
      }
      // ---- Components window ---- //
      if (im.collapsingHeader("Components Window", kPK_DefaultOpen)) {
        // to do: change this to a more efficient option
        const Vector<String> options = { "model", "light", "camera" };
        int32 val = -1;
        if (im.beginCombo("Components", val, options)) {
          if (im.beginDragDropTarget()) {
            const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
            if (id) {
              SPtr<Model> loadedModel = modelMan.createModel(*id);
              modelMan.insertModel(*id, loadedModel);
            }
            im.endDragDropTarget();
          }
          // if a model component is to be added.
          if (val == 0) {
            Path path = m_window.openFileFromExplorer("Model Files", "*.fbx;*.obj;*.gltf");
            if (path.toString() != "") {
              SPtr<BaseResource> resource = make_shared<ModelResource>();
              resource->softLoad(path);
              selectedActor->addComponent(modelMan.createModel(resource->m_id));
            }
          }
        }
        if (im.beginDragDropTarget()) {
          UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
          if (id) {
            SPtr<Model> model = modelMan.createModel(*id);
            selectedActor->addComponent(model);
          }
          im.endDragDropTarget();
        }
        // inspect actor components
        m_actorInspector.inspectComponents(m_selectedMaterial);
      }
      im.endTabItem();
    }
    // -------------------------- //
    // App tab.
    // -------------------------- //
    if (im.beginTabItem("App")) {
      // get framerate
      float f_fps = 1.0f / g_TimeManager().m_deltaTime;
      uint32 fps = static_cast<uint32>(f_fps);
      String fpsStr = "FPS: " + to_string(fps);
      String camSpeed = "Camera Speed: " + to_string(static_cast<uint32>(m_cameraSpeed));

      // FPS parameters
      static const uint32 fpsListSize = 100;
      static float fpsHistory[fpsListSize] = {};
      static uint32 fpsOffset = 0;

      // Record the current FPS
      fpsHistory[fpsOffset] = f_fps;
      fpsOffset = (fpsOffset + 1) % fpsListSize;
      // --- Camera window --- //
      im.createText("vSync");
      im.sameLine();
      im.createCheckBox("##vSync", m_vSync);
      // fps graph
      im.createText(fpsStr.c_str());
      im.sameLine();
      im.plotLines("##LinesFPS", fpsHistory, fpsListSize, fpsOffset);
      // editor app settings
      if (im.beginTable("Editor App")) {
        im.tableJumpRow();

        im.tableJumpRow();
        im.createText("Camera Speed");
        im.tableNextColumn();
        im.createDrag("##CamSpeed", m_cameraSpeed);
        im.tableJumpRow();
        im.createText("X Sensitivity");
        im.tableNextColumn();
        im.createDrag("##XSens", m_sensX, 0.1f);
        im.tableJumpRow();
        im.createText("Y Sensitivity");
        im.tableNextColumn();
        im.createDrag("##YSens", m_sensY, 0.1f);
        im.tableJumpRow();
      }
      im.endTable();
      im.endTabItem();
    }
    // -------------------------- //
    // Graphics tab.
    // -------------------------- //
    if (im.beginTabItem("Graphics")) {
      // table parameters
      if (im.beginTable("Graphics")) {

        // IBL
        im.tableNextColumn();
        im.createCheckBox("IBL Active", m_IBL);
        if (m_IBL) {
          // Slider for IBL intensity.
          if (im.createButtonImage("Skybox", rm.m_mainSkybox)) {
            Path path = m_window.openFileFromExplorer("Texture Files",
                                                      "*.png;*.jpeg;*.jpg;*.tga;*.hdr;*.exr");
            if (path.toString() != "") {
              // SPtr<Texture> texture = tm.loadTexture(path);
              // rm.m_mainSkybox->copyFrom(texture);
            }
          }
          if (im.beginDragDropTarget()) {
            const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
            if (id) {
              SPtr<Texture> texture = tm.loadTexture(*id);
              rm.m_mainSkybox->copyFrom(texture);
            }
            im.endDragDropTarget();
          }
          if (im.isItemHovered()) {
            im.setTooltip("Skybox");
          }
          im.tableNextColumn();
          im.createDrag("##iblIntensity", m_IBLIntensity, 0.1f, 0.0f, 1.0f);
        }

        // EXPOSURE
        im.tableJumpRow();
        im.createText("Exposure");
        im.tableNextColumn();
        im.createDrag("##Exposure", m_exposure, 0.1f, 0.0f);

        // LUMINANCE
        im.tableJumpRow();
        im.createText("----------Luminance----------");
        im.tableJumpRow();
        im.createText("Radius");
        im.tableNextColumn();
        im.createDrag("##LumRadius", m_blurRadius, 0.1f, 0.001f);
        im.tableJumpRow();
        im.createText("Strength");
        im.tableNextColumn();
        im.createDrag("##LumStrength", m_blurStrength, 0.1f, 0.001f);
        im.tableJumpRow();
        im.createText("Threshhold");
        im.tableNextColumn();
        im.createDrag("##LumThreshold", m_lumThreshold, 0.1f, 0.0f);
        im.tableJumpRow();

        // EMISSIVE
        im.createText("----------Emissive----------");
        im.tableJumpRow();
        im.createText("Radius");
        im.tableNextColumn();
        im.createDrag("##EmRadius", m_emissiveBlurRadius, 1.0f, 0.001f);
        im.tableJumpRow();
        im.createText("Strength");
        im.tableNextColumn();
        im.createDrag("##EmStrength", m_emissiveStrength, 0.1f, 0.001f);
        im.tableJumpRow();

        // SSAO
        im.createText("----------SSAO----------");
        im.tableJumpRow();
        im.createCheckBox("SSAO", m_ssao);
        im.tableJumpRow();
        if (m_ssao) {
          im.createText("Sample Radius");
          im.tableNextColumn();
          im.createDrag("##SSAO Radius", m_ssaoSampleRad, 0.1f, 0.0f);
          im.tableJumpRow();
          im.createText("Scale");
          im.tableNextColumn();
          im.createDrag("##SSAOScale", m_ssaoScale, 0.1f, 0.0f);
          im.tableJumpRow();
          im.createText("Bias");
          im.tableNextColumn();
          im.createDrag("##SSAOBias", m_ssaoBias, 0.001f, 0.0f);
          im.tableJumpRow();
          im.createText("Intensity");
          im.tableNextColumn();
          im.createDrag("##SSAOIntensity", m_ssaoIntensity, 0.1f, 0.0f);
        }
        im.tableNextRow();
      }
      im.endTable();

      // --- Post-Process window --- //
      if (im.collapsingHeader("Shaders", kPK_DefaultOpen)) {
        if (im.beginTable("Shaders table")) {
          im.tableNextColumn();
          // display all compiled shaders.
          Vector<SPtr<Shader>> shaders = shaderMan.getShaders();
          for (uint32 i = 0; i < shaders.size(); ++i) {
            SPtr<Shader> shader = shaders[i];
            String shaderName = shader->getShaderDirectory().getFileName();
            im.createText(shaderName.c_str());
            im.tableNextColumn();
            im.pushID(i);
            if (im.createButton("Compile")) {
              //shader->compileFromFile();
            }
            im.popID();
            im.tableJumpRow();
          }
        }
        im.endTable();
      }
      im.endTabItem();
    }
    // -------------------------- //
    // Material tab.
    // -------------------------- //
    if (m_selectedMaterial && im.beginTabItem(m_selectedMaterial->getName())) {
      m_materialInspector.setMaterial(m_selectedMaterial);
      m_materialInspector.createMaterialWindow(m_window);
      im.endTabItem();
    }
    if (im.beginTabItem("Quaternion Test")) {
      static Quaternion quat;
      Vector3 quatV = quat.toEuler() * Math::RAD2DEG;
      im.createText("X: ");
      im.sameLine();
      im.createDrag("##TestX", quatV.x);
      im.createText("Y: ");
      im.sameLine();
      im.createDrag("##TestY", quatV.y);
      im.createText("Z: ");
      im.sameLine();
      im.createDrag("##TestZ", quatV.z);
      quat = Quaternion::fromEuler(quatV * Math::DEG2RAD).normalized();
      if (quat.hasNan()) {
        quat = Quaternion::IDENTITY;
      }

      static Vector3 test;
      if (test == Vector3::ZERO) {
        test = Vector3::UP;
      }
      test = quat * test;
      im.createDrag("Direction:", test);

      im.endTabItem();
    }
    im.endTabBar();
  }
  im.endWindowCreate();
  // -------------------------- //
  im.render();
}

void
EditorApp::showLogType(bool& _active, uint32 _type)
{
  UInterface& im = g_uInterface();
  Logger& log = g_Logger();
  if (_active) {
    Vector<LogMSG> messages = g_Logger().getMessageLogOfType(static_cast<E>(_type));
    const uint32 messageCount = static_cast<uint32>(messages.size());
    for (uint32 i = 0; i < messageCount; ++i) {
      const String msg = log.getStringFromLog(messages[i]);
      im.createText(msg.c_str());
    }
  }
}

void
EditorApp::onUpdate()
{
  // user input
  if (m_window.m_isFocused) {
    input();
  }
}

void
EditorApp::onRender()
{
  // update the user interface
  uInterfaceUpdate();
}
