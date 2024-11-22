/*
#include "pkDX11GraphicsAPI.h"
#include "pkDX11InputLayout.h"
#include "pkPrerequisitesCore.h"
#include "pkWindow.h"
#include "pkModel.h"
#include "pkLight.h"
#include "pkVector2.h"


#if PK_PLATFORM == PK_PLATFORM_WIN32

using pkEngineSDK::PKWindowDesc;
using pkEngineSDK::DX11InputLayout;
using pkEngineSDK::DX11GraphicsAPI;
using pkEngineSDK::Model;
using pkEngineSDK::String;
using pkEngineSDK::Light;
using pkEngineSDK::uint32;
using pkEngineSDK::CBView;
using pkEngineSDK::CBProjection;
using pkEngineSDK::CBWorld;

pkEngineSDK::Window g_window;
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
  g_api.init(g_window.m_windowH);

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
  g_model.load(modelPath, g_api.m_pDevice);
  g_model.m_vertexB = g_model.m_vertexB->create(g_api.m_pDevice, g_model.m_vertex);
  g_model.m_indexB = g_model.m_indexB->create(g_api.m_pDevice, g_model.m_index, D3D11_USAGE_DEFAULT);

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
}

#endif*/