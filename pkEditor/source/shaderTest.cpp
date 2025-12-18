#include "ActorInspector.h"
#include "pkAssetResourceManager.h"
#include "pkColor.h"
#include "pkGPUResourceManager.h"
#include "pkGraphicsAPI.h"
#include "pkGraphicTypes.h"
#include "pkLogger.h"
#include "pkModelCodec.h"
#include "pkModelResource.h"
#include "pkPlatformMath.h"
#include "pkPath.h"
#include "pkRendererManager.h"
#include "pkSceneManager.h"
#include "pkShaderManager.h"
#include "pkTextureCodec.h"
#include "pkTextureManager.h"
#include "pkTimeManager.h"
#include "shaderTest.h"

using pkEngineSDK::AssetResourceManager;
using pkEngineSDK::ANSICHAR;
using pkEngineSDK::BaseResource;
using pkEngineSDK::GraphicsAPI;
using pkEngineSDK::g_AssetResourceManager;
using pkEngineSDK::g_GraphicAPI;
using pkEngineSDK::g_Logger;
using pkEngineSDK::g_ModelCodec;
using pkEngineSDK::g_SceneManager;
using pkEngineSDK::g_ShaderManager;
using pkEngineSDK::g_TextureManager;
using pkEngineSDK::g_TextureCodec;
using pkEngineSDK::Logger;
using pkEngineSDK::LogMSG;
using pkEngineSDK::LOG_MSG_TYPE::E;
using pkEngineSDK::LOG_MSG_TYPE::kError;
using pkEngineSDK::LOG_MSG_TYPE::kLog;
using pkEngineSDK::LOG_MSG_TYPE::kWarning;
using pkEngineSDK::Math;
using pkEngineSDK::ModelCodec;
using pkEngineSDK::ModelResource;
using pkEngineSDK::RESOURCE_TYPE::kShader;
using pkEngineSDK::SceneManager;
using pkEngineSDK::ShaderManager;
using pkEngineSDK::stringToLower;
using pkEngineSDK::TextureCodec;
using pkEngineSDK::TextureManager;
using pkEngineSDK::TextureResource;
using pkEngineSDK::UUID;

#if PK_PLATFORM == PK_PLATFORM_WIN32
#include "imgui_impl_win32.h"
// to do: delete this and replace with own
#include "Windows.h"

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
ShaderTest::onInit()
{
  //start the interface
  UInterface::startUp();
  g_uInterface().init();
  g_uInterface().initWin(m_window.getWindowHandle());

  SceneManager& sceneMan = g_SceneManager();
  SPtr<Scene> activeScene = sceneMan.getActiveScene();

  // create camera
  m_cameraSpeed = 20.0f;
  m_camera = activeScene->instantiate("Main Camera");
  m_camera->addComponent(make_shared<Camera>());
  Vector3 camPos = Vector3(0.0f, 0.0f, -30.0f);
  m_camera->getComponent<Camera>()->init(m_window.getWidth(),
                                         m_window.getHeight(),
                                         3.1416f / 4.0f,
                                         3.0f,
                                         5000.0f,
                                         camPos, // position
                                         Vector3::FORWARD + camPos * -1.0f, // target
                                         Vector3(0.0f, 1.0f, 0.0f)); // up vector
  // camera sensitivity
  m_sensX = 0.3f;
  m_sensY = 0.3f;

  // create light
  m_light = activeScene->instantiate("Light");
  m_light->setPosition(0.0f, 1000.0f, 0.0f);
  m_light->addComponent(make_shared<Light>());
  SPtr<Light> lightCom = m_light->getComponent<Light>();

  // add camera component
  m_light->addComponent(make_shared<Camera>());
  m_light->getComponent<Camera>()->init(1920 * 2.0f,
                                        1080 * 2.0f,
                                        3.1416f / 4.0f,
                                        3.0f,
                                        5000.0f,
                                        lightCom->m_position, // position
                                        lightCom->m_direction, // target
                                        Vector3::FORWARD,
                                        pkEngineSDK::CAMERA_PROJ::kPerspective); // up vector);

  m_IBL = true;
  m_vSync = false;
  m_IBLIntensity = 1.0f;
  // luminance blur
  m_blurRadius = 18.0f;
  m_blurStrength = 1.0f;
  m_lumThreshold = 6.0f;
  // emissive blur
  m_emissiveBlur = 5.0f;
  m_emissiveStrength = 30.0f;
  // exposure
  m_exposure = 1.0f;
  // testure size
  m_imgTextureSize = 45.0f;

  m_sActorIndex = 0;
  m_fpsSize = 20;

  m_showErrors = true;
  m_showWarnings = false;
  m_showActions = false;

  // m_eyeIcon = g_TextureManager().loadTexture(Path("resources/white-eye-icon.pkt"));

  /**
   * User Interface.
   */
  float alpha = 0.4f;
  // scene graph
  Vector2 winRect = m_window.getClientWidthHeight();
  m_sceneGraphWin.name = activeScene->m_name.c_str();
  m_sceneGraphWin.position = Vector2(0.0f, 0.0f);
  m_sceneGraphWin.size = Vector2(winRect.x * 0.1f, winRect.y * 0.8f);
  m_sceneGraphWin.alpha = alpha;
  // logger window
  m_loggerWin.name = "Logger";
  m_loggerWin.size = Vector2(winRect.x, winRect.y * 0.2f);
  m_loggerWin.position = Vector2(0.0f, winRect.y * 0.8f);
  m_loggerWin.alpha = alpha;
  // right window
  m_rightWin.name = "Inspector";
  m_rightWin.size = Vector2(400.0f, winRect.y * 0.8f);
  m_rightWin.position = Vector2(winRect.x - 400.0f, 0.0f);
  m_rightWin.alpha = alpha;
}

void
ShaderTest::initWin()
{
  PKWindowDesc desc;
  desc.width = 1920;
  desc.height = 1080;
  std::string name = "Game Engine Window";
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
ShaderTest::input()
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
  SPtr<Camera> cameraComp = m_camera->getComponent<Camera>();
  // move forward/backward
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kW) && !itemActive) {
    cameraComp->moveForwardLocal(speed);
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kS) && !itemActive) {
    cameraComp->moveForwardLocal(-speed);
  }
  // move left/right
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kA) && !itemActive) {
    cameraComp->moveRightLocal(-speed);
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kD) && !itemActive) {
    cameraComp->moveRightLocal(speed);
  }
  // move up/down
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kE) && !itemActive) {
    cameraComp->moveUpLocal(speed);
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kQ) && !itemActive) {
    cameraComp->moveUpLocal(-speed);
  }
  // rotate camera
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kLButton) && !itemActive) {
    Vector2 posDif = (m_lastCursorPos - eventQueue.mousePosition);
    // m_selectedActor = nullptr;
    posDif.x *= m_sensX;
    posDif.y *= m_sensY;
    m_lastCursorPos = eventQueue.mousePosition;
    posDif *= Math::DEG2RAD;
    cameraComp->rotate(-posDif.y, -posDif.x, 0.0f);
  }
  m_lastCursorPos = eventQueue.mousePosition;
}

bool
findShader(const Vector<ShaderType>& _list, const Path& _path)
{
  for (uint32 i = 0; i < _list.size(); ++i) {
    if (_list[i].path.toString() == _path.toString()) {
      return true;
    }
  }
  return false;
}

void
ShaderTest::uInterfaceUpdate()
{
  AssetResourceManager& assetMan = g_AssetResourceManager();
  GPUResourceManager& gpuResourceMan = g_GPUResourceManager();
  ModelCodec& modelCodec = g_ModelCodec();
  RendererManager& rm = g_RenderManager();
  SceneManager& sm = g_SceneManager();
  ShaderManager& shaderMan = g_ShaderManager();
  TextureCodec& textureCodec = g_TextureCodec();
  TextureManager& tm = g_TextureManager();
  UInterface& im = g_uInterface();

  im.setCurrentContext();
  im.newFrameAPI();
  im.windowNewFrame();
  im.uINewFrame();

  Vector2 winRect = m_window.getClientWidthHeight();
  SPtr<Scene> currentScene = sm.getActiveScene();

  // --- Scene graph window --- //
  im.setNextWindowParams(m_sceneGraphWin);
  im.startWindowCreate(m_sceneGraphWin.name);
  m_sceneGraphWin.setNewSizePos(im.getWindowPos(), im.getWindowSize(), winRect);

  if (im.createButton("+")) {
    currentScene->instantiate("Actor");
    m_sActorIndex = currentScene->getActorCount() - 1;
    m_selectedActor = currentScene->getActor(m_sActorIndex);
  }
  if (im.beginDragDropTarget()) {
    const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
    if (id) {
      SPtr<Model> model = gpuResourceMan.loadPKModel(*id);
      if (model) {
        SPtr<Actor> newActor = currentScene->instantiate(model->getName());
        newActor->addComponent(model);
        m_selectedActor = newActor;
      }
    }
    im.endDragDropTarget();
  }
  if (m_selectedActor) {
    im.sameLine();
    if (im.createButton("Delete")) {
      m_selectedActor->~Actor();
      m_selectedActor = nullptr;
      currentScene->m_actors.erase(currentScene->m_actors.begin() + m_sActorIndex);
      m_sActorIndex = 0;
    }
    im.sameLine();
    if (im.createButton("^")) {
      m_selectedActor = nullptr;
      m_sActorIndex = 0;
    }
  }
  const uint32 actorCount = currentScene->getActorCount();
  for (uint32 i = 0; i < actorCount; ++i) {
    SPtr<Actor> currentActor = currentScene->getActor(i);
    if (im.createButton(currentActor->getName(),
                        Color(0, 0),
                        Color(50, 50),
                        Color(100, 50),
                        true)) {
      m_selectedActor = currentActor;
      m_sActorIndex = i;
    }
  }
  im.endWindowCreate();
  // -------------------------- //

  // -------------------------- //
  // Create log window
  im.setNextWindowParams(m_loggerWin);
  im.startWindowCreate(m_loggerWin.name);
  m_loggerWin.setNewSizePos(im.getWindowPos(), im.getWindowSize(), winRect);

  Vector2 logWinSize = im.getItemSize(); // to do: there is an error when getting the height of the window.

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
      showLogType(m_showErrors, kError);
      showLogType(m_showWarnings, kWarning);
      showLogType(m_showActions, kLog);
      im.endTabItem();
    }
    // resources window.
    if (im.beginTabItem("Resources")) {
      if (im.createButton("Model Resource")) {
        const Path path = m_window.openFileFromExplorer();
        if (path.toString() != "") {
          SPtr<BaseResource> resource = modelCodec.createResourceFromFile(path);
          if (resource) {
            assetMan.insertNewResource(resource);
          }
        }
      }
      im.sameLine();
      if (im.createButton("Texture Resource")) {
        const Path path = m_window.openFileFromExplorer();
        if (path.toString() != "") {
          SPtr<BaseResource> resource = textureCodec.createResourceFromFile(path);
          if (resource) {
            assetMan.insertNewResource(resource);
          }
        }
      }
      im.sameLine();
      im.createInputText("##Search", &m_searchResource);
      im.endTabItem();
      for (auto& asset : assetMan.getAllResources()) {
        const Path assetPath = String(asset.second->m_resourcePath);
        const String assetName = assetPath.getFileName();
        const String searchResLower = stringToLower(m_searchResource); // tolower(m_searchResource.c_str());
        const String assetNameLower = stringToLower(assetName);
        if (asset.second->getType() != kShader && 
            assetNameLower.find(searchResLower.c_str()) != String::npos) {
          if (im.createButton(assetName.c_str())) {

          }
          if (im.beginDragDropSource()) {
            const String dragText = "Dragging " + assetName;
            im.createText(dragText.c_str());
            const UUID* data = &asset.first;
            im.setDragDropPayload("RESOURCE_PAYLOAD", data, sizeof(UUID));
            im.endDragDropSource();
          }
          if (im.isItemHovered()) {
            const String tooltip = "Name: " + assetName + "\n" +
                                   "Asset ID: " + asset.first.toString() + "\n" +
                                   "Asset type: " + asset.second->getTypeString() + "\n" +
                                   "Loaded: " + (asset.second->m_isLoaded ? "Yes" : "No");
            im.setTooltip(tooltip.c_str());
          }
          Vector2 itemPos = im.getItemPosition();
          Vector2 itemSize = im.getItemSize();
          if (itemPos.x + itemSize.x < logWinSize.x) {
            im.sameLine();
          }
        }
      }
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
    // actor tab
    if (m_selectedActor && im.beginTabItem("Actor")) {
      ActorInspector inspector(m_selectedActor);
      // transform window
      im.PushStyleColor(Color(100, 255), Color(150, 255), Color(50, 255));
      if (im.collapsingHeader("Transform", kPK_DefaultOpen)) {
        String name = m_selectedActor->getName();
        im.createText("Name:   ");
        im.sameLine();
        if (im.createInputText("##Name", &name)) {
          m_selectedActor->setName(name);
        }
        // activity checkbox
        im.sameLine();
        im.createCheckBox("##ActiveActor", m_selectedActor->isActive());
        if (m_eyeIcon) {
          im.sameLine();
          im.createImage(m_eyeIcon, Vector2(15));
        }
        // inspect actor transform matrix
        inspector.Inspect();
      }
      im.popStyleColor(3);
      // ---- Components window ---- //
      im.PushStyleColor(Color(0, 120, 200, 125), Color(50, 170, 250, 125), Color(0, 60, 100, 125));
      if (im.collapsingHeader("Components Window", kPK_DefaultOpen)) {
        // to do: change this to a more efficient option
        const Vector<String> options = { "model", "light", "camera" };
        int32 val = -1;
        if (im.beginCombo("Components", val, options)) {
          // if a model component is to be added.
          if (val == 0) {
            Path path = m_window.openFileFromExplorer();
            if (path.toString() != "") {
              SPtr<BaseResource> resource = make_shared<ModelResource>();
              resource->softLoad(path);
              m_selectedActor->addComponent(gpuResourceMan.loadPKModel(resource->m_id));
              
              // const String ID = assetManager.createModelResource(path);
              // m_selectedActor->addComponent(assetManager.loadResource(path));
            }
          }
        }
        if (im.beginDragDropTarget()) {
          UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
          if (id) {
            SPtr<Model> model = gpuResourceMan.loadPKModel(*id);
            m_selectedActor->addComponent(model);
          }
          im.endDragDropTarget();
        }
        // create all components
        uint32 componentCount = static_cast<uint32>(m_selectedActor->getComponents().size());
        for (uint32 i = 0; i < componentCount; ++i) {
          inspector.createComponentWindow(m_selectedActor->getComponents()[i],
                                          m_window,
                                          m_searchMesh,
                                          m_imgTextureSize);
        }
      }
      im.popStyleColor(3);
      im.endTabItem();
    }
    // -------------------------- //
    // app tab.
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
        im.createDragF("##CamSpeed", m_cameraSpeed);
        im.tableJumpRow();
        im.createText("X Sensitivity");
        im.tableNextColumn();
        im.createDragF("##XSens", m_sensX, 0.1f);
        im.tableJumpRow();
        im.createText("Y Sensitivity");
        im.tableNextColumn();
        im.createDragF("##YSens", m_sensY, 0.1f);
        im.tableJumpRow();
        im.createText("Texture UI Image Size");
        im.tableNextColumn();
        im.createDragF("##TextureUIImageSize", m_imgTextureSize, 1.0f, 1.0f);
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
            Path path = m_window.openFileFromExplorer();
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
          im.createDragF("##iblIntensity", m_IBLIntensity, 0.1f, 0.0f, 1.0f);
        }

        // EXPOSURE
        im.tableJumpRow();
        im.createText("Exposure");
        im.tableNextColumn();
        im.createDragF("##Exposure", m_exposure, 0.1f, 0.0f);

        // LUMINANCE
        im.tableJumpRow();
        im.createText("----------Luminance----------");
        im.tableJumpRow();
        im.createText("Radius");
        im.tableNextColumn();
        im.createDragF("##LumRadius", m_blurRadius, 0.1f, 0.001f);
        im.tableJumpRow();
        im.createText("Strength");
        im.tableNextColumn();
        im.createDragF("##LumStrength", m_blurStrength, 0.1f, 0.001f);
        im.tableJumpRow();
        im.createText("Threshhold");
        im.tableNextColumn();
        im.createDragF("##LumThreshold", m_lumThreshold, 0.1f, 0.0f);
        im.tableJumpRow();

        // EMISSIVE
        im.createText("----------Emissive----------");
        im.tableJumpRow();
        im.createText("Radius");
        im.tableNextColumn();
        im.createDragF("##EmRadius", m_emissiveBlur, 1.0f, 0.001f);
        im.tableJumpRow();
        im.createText("Strength");
        im.tableNextColumn();
        im.createDragF("##EmStrength", m_emissiveStrength, 0.1f, 0.001f);
        im.tableJumpRow();

        // SSAO
        im.createText("----------SSAO----------");
        im.tableJumpRow();
        im.createCheckBox("SSAO", m_ssao);
        im.tableJumpRow();
        if (m_ssao) {
          im.createText("Sample Radius");
          im.tableNextColumn();
          im.createDragF("##SSAO Radius", m_ssaoSampleRad, 0.1f, 0.0f);
          im.tableJumpRow();
          im.createText("Scale");
          im.tableNextColumn();
          im.createDragF("##SSAOScale", m_ssaoScale, 0.1f, 0.0f);
          im.tableJumpRow();
          im.createText("Bias");
          im.tableNextColumn();
          im.createDragF("##SSAOBias", m_ssaoBias, 0.001f, 0.0f);
          im.tableJumpRow();
          im.createText("Intensity");
          im.tableNextColumn();
          im.createDragF("##SSAOIntensity", m_ssaoIntensity, 0.1f, 0.0f);
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
    im.endTabBar();
  }
  im.endWindowCreate();
  // -------------------------- //
  im.render();
}

void
ShaderTest::showLogType(bool& _active, uint32 _type)
{
  UInterface& im = g_uInterface();
  Logger& log = g_Logger();
  if (_active) {
    Vector<LogMSG> messages = g_Logger().getMessageLogOfType(static_cast<E>(_type));
    for (uint32 i = 0; i < messages.size(); ++i) {
      const String msg = log.getStringFromLog(messages[i]);
      im.createText(msg.c_str());
    }
  }
}

void
ShaderTest::onUpdate()
{
  // user input
  if (m_window.m_isFocused) {
    input();
  }

  // m_light = g_SceneManager().getActiveScene()->getActorWithComponent<Light>();

  // managers
  GraphicsAPI& api = g_GraphicAPI();
  RendererManager& rm = g_RenderManager();

  // get all passes.
  SPtr<Pass> lightPositions = rm.getPass(kP_LightPositions);
  SPtr<Pass> basePass = rm.getPass(kP_Base);
  SPtr<Pass> skyBoxPass = rm.getPass(kP_SkyBox);
  SPtr<Pass> lightQuad = rm.getPass(kP_Light);
  SPtr<Pass> lumPass = rm.getPass(kP_Luminance);
  SPtr<Pass> lumBlurHPass = rm.getPass(kP_LumBlurH);
  SPtr<Pass> lumBlurPass = rm.getPass(kP_LumBlur);
  SPtr<Pass> emissHBlur = rm.getPass(kP_EmissiveHBlur);
  SPtr<Pass> emissBlur = rm.getPass(kP_EmissiveBlur);
  SPtr<Pass> tonePass = rm.getPass(kP_Tone);
  SPtr<Pass> ssaoPass = rm.getPass(kP_SSAO);

  Vector2 winSize = api.getSwapChain()->getSize();

  // camera data
  SPtr<Camera> camera = m_camera->getComponent<Camera>();
  Matrix4 view = Matrix4::IDENTITY;
  Matrix4 proj = Matrix4::IDENTITY;
  Matrix4 invView = Matrix4::IDENTITY;
  Matrix4 invProj = Matrix4::IDENTITY;
  Matrix4 invViewProj = Matrix4::IDENTITY;
  Matrix4 viewTransp = Matrix4::IDENTITY;
  Matrix4 projTransp = Matrix4::IDENTITY;
  // main camera buffer
  CBCamera cBCamera;
  CBVector2x2 shadowsParam(winSize, Vector2(0.0f)); // to do: win size could change, swap this to use the specific texture size.
  // to do: change this to another method
  if (camera) {
    view = camera->m_view.getTransposed();
    proj = camera->m_projection.getTransposed();
    invView = view.inverse();
    invProj = proj.inverse();
    viewTransp = view.getTransposed();
    projTransp = proj.getTransposed();
    CreateCBCamera::create(cBCamera, camera);
    invViewProj = (proj * view).inverse();

    shadowsParam.vec2 = camera->m_farNear;
  }
  Vector4 SkyBoxWinSize(winSize.x, winSize.y, 0.0f, 0.0f);

  // light data
  SPtr<Light> light = m_light->getComponent<Light>();
  SPtr<Camera> lightCamera = m_light->getComponent<Camera>();
  // light buffers
  CBLight cBLight;
  CBCamera cBLightCam;

  // update shadow depth map buffers
  Matrix4 lightView = Matrix4::IDENTITY;
  Matrix4 lightProj = Matrix4::IDENTITY;
  Matrix4 lightViewProj = Matrix4::IDENTITY;
  // to do: change this to another method
  if (light) {
    lightView = lightCamera->m_view.getTransposed();
    lightProj = lightCamera->m_projection.getTransposed();
    lightViewProj = lightProj * lightView;
    CreateCBLight::create(cBLight, light);
    CreateCBCamera::create(cBLightCam, lightCamera);
  }

  // luminance parameters.
  CBVector2x2 lum(winSize, Vector2(m_lumThreshold, 0.0f));
  // blur parameters.
  CBBlur blur(winSize, Vector2(1.0f, 0.0f), m_blurRadius, m_blurStrength);
  // emissive blur parameters
  CBBlur emissiveBlur(winSize, Vector2(1.0f, 0.0f), m_emissiveBlur, m_emissiveStrength);
  // IBR parameters.
  CBFloat IBRIntens(m_IBLIntensity);
  CBVector3 viewPos(camera->m_eye.xyz());
  // exposure parameter.
  CBFloat exposure(m_exposure);
  // SSAO
  CBSSAO ssao(m_ssaoSampleRad, m_ssaoScale, m_ssaoBias, m_ssaoIntensity);
  CBVector2x2 ssaoWin(ssaoPass->getViewportSize(), Vector2(0.0f));

  // data type sizes.
  uint32 m4x4Size = sizeof(Matrix4);
  uint32 cBCamSize = sizeof(CBCamera);
  uint32 cBLightSize = sizeof(CBLight);
  uint32 cbBlurSize = sizeof(CBBlur);

  // update normal && base shadow pass buffers.
  api.updateConstantBuffer(lightPositions->getCBuffer(0), &lightView, m4x4Size);
  api.updateConstantBuffer(lightPositions->getCBuffer(1), &lightProj, m4x4Size);

  api.updateConstantBuffer(basePass->getCBuffer(0), &view, m4x4Size);
  api.updateConstantBuffer(basePass->getCBuffer(1), &proj, m4x4Size);

  // update shadow-specular quad pass
  api.updateConstantBuffer(lightQuad->getCBuffer(0), &cBLight, cBLightSize);
  api.updateConstantBuffer(lightQuad->getCBuffer(1), &cBCamera, cBCamSize);
  api.updateConstantBuffer(lightQuad->getCBuffer(2), &lightViewProj, m4x4Size);
  api.updateConstantBuffer(lightQuad->getCBuffer(3), &shadowsParam, sizeof(CBVector2x2));
  api.updateConstantBuffer(lightQuad->getCBuffer(4), &IBRIntens, sizeof(CBFloat));

  // skybox constant buffers.
  api.updateConstantBuffer(skyBoxPass->getCBuffer(0), &viewTransp, m4x4Size);
  api.updateConstantBuffer(skyBoxPass->getCBuffer(1), &projTransp, m4x4Size);

  // luminance constant buffers.
  api.updateConstantBuffer(lumPass->getCBuffer(0), &lum, sizeof(CBVector2x2));
  // Emissive blur constant buffers;
  emissiveBlur.BlurDirection = Vector2(1.0f, 0.0f);
  api.updateConstantBuffer(emissHBlur->getCBuffer(0), &emissiveBlur, cbBlurSize);
  emissiveBlur.BlurDirection = Vector2(0.0f, 1.0f);
  api.updateConstantBuffer(emissBlur->getCBuffer(0), &emissiveBlur, cbBlurSize);
  // lum blur constant buffers
  blur.BlurDirection = Vector2(1.0f, 0.0f);
  api.updateConstantBuffer(lumBlurHPass->getCBuffer(0), &blur, cbBlurSize);
  blur.BlurDirection = Vector2(0.0f, 1.0f);
  api.updateConstantBuffer(lumBlurPass->getCBuffer(0), &blur, cbBlurSize);

  api.updateConstantBuffer(tonePass->getCBuffer(0), &exposure, sizeof(CBFloat));

  api.updateConstantBuffer(ssaoPass->getCBuffer(0), &ssao, sizeof(CBSSAO));
  api.updateConstantBuffer(ssaoPass->getCBuffer(1), &ssaoWin, sizeof(CBVector2x2));
}

void
ShaderTest::onRender()
{
  // update the user interface
  uInterfaceUpdate();
}
