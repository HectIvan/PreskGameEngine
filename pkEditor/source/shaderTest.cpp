#include "ActorInspector.h"
#include "pkLogger.h"
#include "pkGraphicsAPI.h"
#include "pkGraphicTypes.h"
#include "pkPlatformMath.h"
#include "pkPath.h"
#include "pkRendererManager.h"
#include "pkGPUResourceManager.h"
#include "pkSceneManager.h"
#include "pkTextureManager.h"
#include "pkTimeManager.h"
#include "shaderTest.h"
#include "pkColor.h"

using pkEngineSDK::Color;
using pkEngineSDK::CBBlur;
using pkEngineSDK::CBFloat;
using pkEngineSDK::CBVector2x2;
using pkEngineSDK::CBVector3;
using pkEngineSDK::CBSSAO;
using pkEngineSDK::CreateCBCamera;
using pkEngineSDK::CreateCBLight;
using pkEngineSDK::D_BUFFERS::kDB_Base;
using pkEngineSDK::D_BUFFERS::kDB_Light;
using pkEngineSDK::EventQueue;
using pkEngineSDK::GraphicsAPI;
using pkEngineSDK::G_BUFFERS::kGB_Albedo;
using pkEngineSDK::G_BUFFERS::kGB_Normal;
using pkEngineSDK::g_EventManager;
using pkEngineSDK::g_GraphicAPI;
using pkEngineSDK::g_uInterface;
using pkEngineSDK::g_Logger;
using pkEngineSDK::g_RenderManager;
using pkEngineSDK::g_GPUResourceManager;
using pkEngineSDK::g_SceneManager;
using pkEngineSDK::g_TextureManager;
using pkEngineSDK::g_TimeManager;
using pkEngineSDK::int32;
using pkEngineSDK::Light;
using pkEngineSDK::Logger;
using pkEngineSDK::LogMSG;
using pkEngineSDK::LOG_MSG_TYPE::E;
using pkEngineSDK::LOG_MSG_TYPE::kError;
using pkEngineSDK::LOG_MSG_TYPE::kLog;
using pkEngineSDK::LOG_MSG_TYPE::kWarning;
using pkEngineSDK::Material;
using pkEngineSDK::Math;
using pkEngineSDK::Matrix4;
using pkEngineSDK::Path;
using pkEngineSDK::PASS_TYPE::kP_Base;
using pkEngineSDK::PASS_TYPE::kP_EmissiveBlur;
using pkEngineSDK::PASS_TYPE::kP_EmissiveHBlur;
using pkEngineSDK::PASS_TYPE::kP_IBR;
using pkEngineSDK::PASS_TYPE::kP_Luminance;
using pkEngineSDK::PASS_TYPE::kP_LumBlur;
using pkEngineSDK::PASS_TYPE::kP_LumBlurH;
using pkEngineSDK::PASS_TYPE::kP_Shadow;
using pkEngineSDK::PASS_TYPE::kP_ShadowQuad;
using pkEngineSDK::PASS_TYPE::kP_SkyBox;
using pkEngineSDK::PASS_TYPE::kP_SSAO;
using pkEngineSDK::PASS_TYPE::kP_Tone;
using pkEngineSDK::PlatformPointer;
using pkEngineSDK::PKWindowDesc;
using pkEngineSDK::PK_TREENODE_FLAGS::kPK_DefaultOpen;
using pkEngineSDK::RendererManager;
using pkEngineSDK::GPUResourceManager;
using pkEngineSDK::Scene;
using pkEngineSDK::SceneManager;
using pkEngineSDK::SPtr;
using pkEngineSDK::String;
using pkEngineSDK::TimeManager;
using pkEngineSDK::TextureManager;
using pkEngineSDK::to_string;
using pkEngineSDK::uint32;
using pkEngineSDK::UInterface;
using pkEngineSDK::Vector4;
using pkEngineSDK::Model;

using std::chrono::high_resolution_clock;
using std::chrono::duration;
// to do: create fileSystem.h in utilities
// create class Path

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

  // Logger& log = g_Logger();
  // TimeManager& timeMan = g_TimeManager();
  // get the resource manager
  // GPUResourceManager& resourceMan = g_GPUResourceManager();
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

  // SPtr<Actor> pistol = activeScene->instantiate("Pistol");
  // pistol->addComponent(resourceMan.loadModel(Path("models/drakefire_pistol_low.obj")));
  // pistol->setScale(30.0f);
  // pistol->setPosition(10.0f, 15.0f, 0.0f);
  // 
  // SPtr<Actor> sponza = activeScene->instantiate("Sponza");
  // sponza->addComponent(resourceMan.loadModel(Path("models/sponza.obj")));
  // 
  // SPtr<Actor> coat = activeScene->instantiate("Coat");
  // coat->addComponent(resourceMan.loadModel(Path("models/export3dcoat.obj")));
  // coat->setPosition(11.0f, 5.2f, 0.0f);

  // log.print("Loading Exterior.");
  // timeMan.startTimer();
  // SPtr<Actor> bistroInt = activeScene->instantiate("Exterior");
  // bistroInt->addComponent(resourceMan.loadModel(Path("models/bistro_exterior.pkm")));
  // timeMan.endTimer();
  // float time = timeMan.getTimerElapsed();
  // log.print(to_string(time) + " Seconds");
  // 
  // log.print("Loading Interior.");
  // timeMan.startTimer();
  // SPtr<Actor> bistroExt = activeScene->instantiate("Interior");
  // bistroExt->addComponent(resourceMan.loadModel(Path("models/bistro_interior_wine.pkm")));
  // timeMan.endTimer();
  // time = timeMan.getTimerElapsed();
  // log.print(to_string(time) + " Seconds");

  m_IBR = true;
  m_vSync = false;
  m_IBRIntensity = 1.0f;
  // luminance blur
  m_blurRadius = 1.0f;
  m_blurStrength = 20.0f;
  m_lumThreshold = 90.0f;
  // emissive blur
  m_emissiveBlur = 30.0f;
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

  m_eyeIcon = g_TextureManager().loadTexture(Path("textures/white-eye-icon.jpg"));

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
  m_rightWin.size = Vector2(400.0f, 800.0f);
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
  bool itemActive = im.isItemActive();
  float deltaTime = g_TimeManager().m_deltaTime;
  // set camera speed with deltaTime
  float speed = m_cameraSpeed * deltaTime;
  // if the user wants to exit the app.
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kEsc)) {
    ApplicationRun(false);
  }
  // move forward/backward
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kW) && !itemActive) {
    m_camera->getComponent<Camera>()->moveForwardLocal(speed);
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kS) && !itemActive) {
    m_camera->getComponent<Camera>()->moveForwardLocal(-speed);
  }
  // move left/right
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kA) && !itemActive) {
    m_camera->getComponent<Camera>()->moveRightLocal(-speed);
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kD) && !itemActive) {
    m_camera->getComponent<Camera>()->moveRightLocal(speed);
  }
  // move up/down
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kE) && !itemActive) {
    m_camera->getComponent<Camera>()->moveUpLocal(speed);
  }
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kQ) && !itemActive) {
    m_camera->getComponent<Camera>()->moveUpLocal(-speed);
  }
  // rotate camera
  if (eventQueue.iskeyPressed(pkEngineSDK::KEY::kLButton) && !itemActive) {
    Vector2 posDif = (m_lastCursorPos - eventQueue.mousePosition);
    // m_selectedActor = nullptr;
    posDif.x *= m_sensX;
    posDif.y *= m_sensY;
    m_lastCursorPos = eventQueue.mousePosition;
    posDif *= Math::DEG2RAD;
    m_camera->getComponent<Camera>()->rotate(-posDif.y, -posDif.x, 0.0f);
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
  SceneManager& sm = g_SceneManager();
  UInterface& im = g_uInterface();
  RendererManager& rm = g_RenderManager();
  TextureManager& tm = g_TextureManager();
  GPUResourceManager& resourceMan = g_GPUResourceManager();

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
  uint32 actorCount = currentScene->getActorCount();
  for (uint32 i = 0; i < actorCount; ++i) {
    SPtr<Actor> currentActor = currentScene->getActor(i);
    if (im.createButton(currentActor->getName(),
                        Color(0, 0, 0, 0),
                        Color(50, 50, 50, 50),
                        Color(100, 100, 100, 50),
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

  im.createCheckBox("Errors", m_showErrors);
  im.sameLine();
  im.createCheckBox("Warnings", m_showWarnings);
  im.sameLine();
  im.createCheckBox("Logs", m_showActions);
  showLogType(m_showErrors, kError);
  showLogType(m_showWarnings, kWarning);
  showLogType(m_showActions, kLog);
  im.endWindowCreate();

  // --- Transform window --- //
  im.setNextWindowParams(m_rightWin);
  im.startWindowCreate(m_rightWin.name);
  m_rightWin.setNewSizePos(im.getWindowPos(), im.getWindowSize(), winRect);
  if (m_selectedActor) {
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
      Vector<String> options = { "model", "light", "camera" };
      int32 val = -1;
      if (im.beginCombo("Components", val, options)) {
        if (val == 0) {
          Path path = m_window.openFileFromExplorer();
          if (path.toString().c_str() != "") {
            m_selectedActor->addComponent(resourceMan.loadModel(path));
          }
        }
      }
      // create all components
      for (uint32 i = 0; i < m_selectedActor->getComponents().size(); ++i) {
        inspector.createComponentWindow(m_selectedActor->getComponents()[i],
                                        m_window,
                                        m_searchMesh,
                                        m_imgTextureSize);
      }
    }
    im.popStyleColor(3);
  }
  // -------------------------- //

  // get framerate
  float f_fps = 1.0f / g_TimeManager().m_deltaTime;
  uint32 fps = static_cast<uint32>(f_fps);
  String fpsStr = "FPS: " + to_string(fps);
  String camSpeed = "Camera Speed: " + to_string(static_cast<uint32>(m_cameraSpeed));

  // FPS parameters
  static const int fpsListSize = 100;
  static float fpsHistory[fpsListSize] = {};
  static int fpsOffset = 0;

  // Record the current FPS
  fpsHistory[fpsOffset] = f_fps;
  fpsOffset = (fpsOffset + 1) % fpsListSize;

  // --- Camera window --- //
  if (im.collapsingHeader("Editor App", kPK_DefaultOpen)) {
    // vSync
    im.createText("vSync");
    im.sameLine();
    im.createCheckBox("##vSync", m_vSync);
    im.createText(fpsStr.c_str());
    im.sameLine();
    im.plotLines("##LinesFPS", fpsHistory, fpsListSize, fpsOffset);
    // camera speed
    im.createDragF("##CamSpeed", m_cameraSpeed);
    im.sameLine();
    im.createText("Camera Speed");
    // X Sensitivity
    im.createDragF("##XSens", m_sensX, 0.1f);
    im.sameLine();
    im.createText("X Sensitivity");
    // Y Sensitivity
    im.createDragF("##YSens", m_sensY, 0.1f);
    im.sameLine();
    im.createText("Y Sensitivity");
    im.createDragF("Texture UI Image Size", m_imgTextureSize, 1.0f, 1.0f);
  }
  // -------------------------- //

  // ------------IBL------------ //
  if (im.collapsingHeader("IBL", kPK_DefaultOpen)) {
    im.createCheckBox("IBL Active", m_IBR);
    if (m_IBR) {
      // Slider for IBL intensity.
      im.sameLine();
      im.createDragF("##iblIntensity", m_IBRIntensity, 0.1f, 0.0f, 1.0f);
      // Button for loading an HDRI image.
      if (im.createButtonImage("Skybox", rm.m_mainSkybox)) {
        Path path = m_window.openFileFromExplorer();
        if (path.toString() != "") {
          SPtr<Texture> texture = tm.loadTexture(path);
          rm.m_mainSkybox->copyFrom(texture);
        }
      }
    }
  }
  if (im.isItemHovered()) {
    im.setTooltip("Skybox");
  }
  // -------------------------- //
  
  // --- Post-Process window --- //
  im.PushStyleColor(Color(100, 0, 100, 125), Color(130, 0, 130, 125), Color(160, 0, 160, 125));
  if (im.collapsingHeader("Post-Process", kPK_DefaultOpen)) {
    im.createDragF("Exposure", m_exposure, 0.1f, 0.0f);
    // Luminance
    im.createText("Luminance");
    // Blur
    im.createDragF("##LumRadius", m_blurRadius, 0.1f, 0.001f);
    im.sameLine();
    im.createText("Radius");
    im.createDragF("##LumStrength", m_blurStrength, 0.1f, 0.001f);
    im.sameLine();
    im.createText("Strength");
    // luminance threshold
    im.createDragF("##LumThreshold", m_lumThreshold, 0.1f, 0.0f);
    im.sameLine();
    im.createText("Threshold");

    // Emissive blur pass
    im.createText("Emissive");
    im.createDragF("##EmRadius", m_emissiveBlur, 1.0f, 0.001f);
    im.sameLine();
    im.createText("Radius");
    im.createDragF("##EmStrength", m_emissiveStrength, 0.1f, 0.001f);
    im.sameLine();
    im.createText("Strength");
    // ssao
    im.createCheckBox("SSAO", m_ssao);
    if (m_ssao) {
      im.createDragF("SSAO Radius", m_ssaoSampleRad, 0.1f, 0.0f);
      im.sameLine();
      im.createText("Sample Radius");
      im.createDragF("##SSAOScale", m_ssaoScale, 0.1f, 0.0f);
      im.sameLine();
      im.createText("Scale");
      im.createDragF("##SSAOBias", m_ssaoBias, 0.001f, 0.0f);
      im.sameLine();
      im.createText("Bias");
      im.createDragF("##SSAOIntensity", m_ssaoIntensity, 0.1f, 0.0f);
      im.sameLine();
      im.createText("Intensity");
    }
  }
  if (im.collapsingHeader("Shaders", kPK_DefaultOpen)) {
    // compile shaders
    Vector<ShaderType> shaderPaths;
    for (auto it = rm.m_passes.begin(); it != rm.m_passes.end(); ++it) {
      SPtr<Shader> vShader = it->second->getVShader();
      SPtr<Shader> pShader = it->second->getPShader();
      SPtr<Shader> cShader = it->second->getCShader();
      // check if a vertex shader exists.
      if (vShader) {
        Path dir = vShader->getShaderDirectory();
        if (!findShader(shaderPaths, dir)) {
          ShaderType shaderMem;
          shaderMem.path = dir;
          shaderMem.name = dir.getFileName();
          shaderMem.shader = vShader;
          shaderPaths.push_back(shaderMem);
        }
      }
      // check if a pixel shader exist.
      if (pShader) {
        Path dir = pShader->getShaderDirectory();
        if (!findShader(shaderPaths, dir)) {
          ShaderType shaderMem;
          shaderMem.path = dir;
          shaderMem.name = dir.getFileName();
          shaderMem.shader = pShader;
          shaderPaths.push_back(shaderMem);
        }
      }
      // check if a compute shader exists.
      if (cShader) {
        Path dir = cShader->getShaderDirectory();
        if (!findShader(shaderPaths, dir)) {
          ShaderType shaderMem;
          shaderMem.path = dir;
          shaderMem.name = dir.getFileName();
          shaderMem.shader = cShader;
          shaderPaths.push_back(shaderMem);
        }
      }
    }
    // compilation buttons for each shader.
    for (uint32 i = 0; i < shaderPaths.size(); ++i) {
      const char* name = shaderPaths[i].name.c_str();
      im.createText(name);
      // im.sameLine();
      // if (im.createButton(name)) {
      //   shaderPaths[i].shader->compile();
      // }
    }
    // compilation button for all shaders.
    if (im.createButton("Compile Shaders")) {
      rm.compileShaders();
    }
  }
  im.popStyleColor(3);
  im.endWindowCreate();
  // -------------------------- //
  im.render();
}

void
ShaderTest::showLogType(bool& _active, uint32 _type)
{
  UInterface& im = g_uInterface();
  if (_active) {
    Vector<LogMSG> messages = g_Logger().getMessageLogOfType(static_cast<E>(_type));
    for (uint32 i = 0; i < messages.size(); ++i) {
      im.createText(messages[i].message.c_str());
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
  SPtr<Pass> baseShadow = rm.getPass(kP_Shadow);
  SPtr<Pass> basePass = rm.getPass(kP_Base);
  SPtr<Pass> skyBoxPass = rm.getPass(kP_SkyBox);
  SPtr<Pass> IBRPass = rm.getPass(kP_IBR);
  SPtr<Pass> quadShadows = rm.getPass(kP_ShadowQuad);
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
  CBVector2x2 shadowsParam;
  shadowsParam.vec1 = winSize; // to do: win size could change, swap this to use the specific texture size.
  shadowsParam.vec2 = Vector2(0.0f);
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
  CBVector2x2 lum;
  lum.vec1 = winSize;
  lum.vec2.x = m_lumThreshold;
  // blur parameters.
  CBBlur blur;
  blur.WinSize = winSize;
  blur.BlurDirection = Vector2(1.0f, 0.0f);
  blur.radius = m_blurRadius;
  blur.strength = m_blurStrength;
  // emissive blur parameters
  CBBlur emissiveBlur;
  emissiveBlur.WinSize = winSize;
  emissiveBlur.radius = m_emissiveBlur;
  emissiveBlur.strength = m_emissiveStrength;
  // IBR parameters.
  CBFloat IBRIntens;
  IBRIntens.value = m_IBRIntensity;
  CBVector3 viewPos;
  viewPos.vec1 = camera->m_eye.xyz();
  // exposure parameter.
  CBFloat exposure;
  exposure.value = m_exposure;
  // SSAO
  CBSSAO ssao;
  ssao.sample_rad = m_ssaoSampleRad;
  ssao.scale = m_ssaoScale;
  ssao.bias = m_ssaoBias;
  ssao.intensity = m_ssaoIntensity;
  CBVector2x2 ssaoWin;
  ssaoWin.vec1 = ssaoPass->getViewportSize();

  // data type sizes.
  uint32 m4x4Size = sizeof(Matrix4);
  uint32 cBCamSize = sizeof(CBCamera);
  uint32 cBLightSize = sizeof(CBLight);

  // update normal && base shadow pass buffers.
  api.updateConstantBuffer(basePass->getCBuffer(0), &view, m4x4Size);
  api.updateConstantBuffer(basePass->getCBuffer(1), &proj, m4x4Size);
  api.updateConstantBuffer(basePass->getCBuffer(3), &cBLight, cBLightSize);
  api.updateConstantBuffer(basePass->getCBuffer(4), &cBCamera, cBCamSize);

  api.updateConstantBuffer(baseShadow->getCBuffer(0), &lightView, m4x4Size);
  api.updateConstantBuffer(baseShadow->getCBuffer(1), &lightProj, m4x4Size);
  api.updateConstantBuffer(baseShadow->getCBuffer(3), &cBLight, cBLightSize);
  api.updateConstantBuffer(baseShadow->getCBuffer(4), &cBLightCam, cBCamSize);

  // update shadow-specular quad pass
  api.updateConstantBuffer(quadShadows->getCBuffer(0), &cBLight, cBLightSize);
  api.updateConstantBuffer(quadShadows->getCBuffer(1), &cBCamera, cBCamSize);
  api.updateConstantBuffer(quadShadows->getCBuffer(2), &cBLightCam, cBCamSize);
  api.updateConstantBuffer(quadShadows->getCBuffer(3), &lightViewProj, m4x4Size);
  api.updateConstantBuffer(quadShadows->getCBuffer(4), &shadowsParam, sizeof(Vector4));

  // skybox constant buffers.
  api.updateConstantBuffer(skyBoxPass->getCBuffer(0), &viewTransp, m4x4Size);
  api.updateConstantBuffer(skyBoxPass->getCBuffer(1), &projTransp, m4x4Size);

  // ibr constant buffers.
  api.updateConstantBuffer(IBRPass->getCBuffer(0), &IBRIntens, sizeof(Vector4));
  api.updateConstantBuffer(IBRPass->getCBuffer(1), &viewPos, sizeof(CBVector3));

  // luminance constant buffers.
  api.updateConstantBuffer(lumPass->getCBuffer(0), &lum, sizeof(CBVector2x2));
  // Emissive blur constant buffers;
  emissiveBlur.BlurDirection = Vector2(1.0f, 0.0f);
  api.updateConstantBuffer(emissHBlur->getCBuffer(0), &emissiveBlur, sizeof(CBBlur));
  emissiveBlur.BlurDirection = Vector2(0.0f, 1.0f);
  api.updateConstantBuffer(emissBlur->getCBuffer(0), &emissiveBlur, sizeof(CBBlur));
  // lum blur constant buffers
  blur.BlurDirection = Vector2(1.0f, 0.0f);
  api.updateConstantBuffer(lumBlurHPass->getCBuffer(0), &blur, sizeof(CBBlur));
  blur.BlurDirection = Vector2(0.0f, 1.0f);
  api.updateConstantBuffer(lumBlurPass->getCBuffer(0), &blur, sizeof(CBBlur));

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
