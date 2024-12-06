#include "pkWindow.h"
#include "pkWindowDesc.h"
#include "pkGraphicsAPI.h"

int main()
{
  
  pkEngineSDK::Window window;
  pkEngineSDK::PKWindowDesc desc;
  desc.width = 1920;
  desc.height = 1080;
  window.create(desc, "window test");
  // pkEngineSDK::GraphicsAPI api;
  // api.init(window.getWidth(), window.getHeight(), window.getWindowHandle());
  return window.messageLoop();
}

//#if PK_PLATFORM == PK_PLATFORM_WIN32
/*
using pkEngineSDK::CBView;
using pkEngineSDK::CBProjection;
using pkEngineSDK::CBWorld;
using pkEngineSDK::DX11InputLayout;
using pkEngineSDK::DX11GraphicsAPI;
using pkEngineSDK::Model;
using pkEngineSDK::Light;
using pkEngineSDK::PKWindowDesc;
using pkEngineSDK::String;
using pkEngineSDK::uint32;
using pkEngineSDK::Vector2;
using pkEngineSDK::Window;

Window g_window;
DX11GraphicsAPI g_api;
DX11InputLayout g_inputLayout;

Model g_model;
Light g_light;


int wWinMain(InstanceHandle hInstance, InstanceHandle hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
  PKWindowDesc winDesc;
  winDesc.width = 1920;
  winDesc.height = 1080;
  g_window.create(winDesc, "window DX11 Win32 Test", hInstance);

  // initialize api
  g_api.init(g_window.getWindowHandle());

  // shaders
  g_api.m_pixelShader.compile();
  g_api.m_pixelShader.create(g_api.m_pDevice);

  g_api.m_vertexShader.compile();
  g_api.m_vertexShader.create(g_api.m_pDevice);

  // create and set input layout
  g_inputLayout.create(g_api.m_pDevice, g_api.m_vertexShader);
  g_inputLayout.set(g_api.m_pDevice);

  // set device topology
  g_api.m_pDevice->setPrimitiveTopology();

  // proessing the model
  String modelName = "test";
  String extension = ".fbx";
  String modelPath = modelName + extension;
  g_model.load(modelPath);
  g_model.vertexB = g_model.vertexB->create(g_api.m_pDevice, g_model.vertex, 1);
  g_model.indexB = g_model.indexB->create(g_api.m_pDevice, g_model.index, D3D11_USAGE_DEFAULT);

  // create constant buffers
  g_light.Type = pkEngineSDK::LIGHT_TYPE::kDirectional;
  g_api.m_cBView.create(g_api.m_pDevice, static_cast<uint32>(sizeof(CBView)));
  g_api.m_cBProjection.create(g_api.m_pDevice, static_cast<uint32>(sizeof(CBProjection)));
  g_api.m_cBWorld.create(g_api.m_pDevice, static_cast<uint32>(sizeof(CBWorld)));
  g_api.m_LightCB.create(g_api.m_pDevice, static_cast<uint32>(sizeof(Light)));

  // create sampler state
  g_api.createSamplerState();

  // camera initialization
  Vector2 clientSize = g_window.getClientWidthHeight();
  return 0;
}*/
//#endif