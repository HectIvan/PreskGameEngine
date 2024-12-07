/************************************************************************/
/**
* @pkDX11GraphicsAPI pkDX11GraphicsAPI.h
* @Hector Ivan Muñoz Ceballos
* @date 19/11/2024
* @Graphics api file using DirectX 11 for the Presk Game Engine.
*
* Graphics API class using DirectX 11 for the engine
*
* @bug No bug known.
*
* @HectIvan 19/11/2024
* Created the file
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11ConstantBuffer.h"
#include "pkDX11DepthStencilView.h"
#include "pkDX11Device.h"
#include "pkDX11PixelShader.h"
#include "pkDX11SamplerState.h"
#include "pkDX11VertexShader.h"
#include "pkGraphicsAPI.h"
#include "pkLight.h"
#include "pkMatrix4.h"
#include "pkPrerequisitesCore.h"
#include "pkVector4.h"


namespace pkEngineSDK
{

class Window;

using pkEngineSDK::GraphicsAPI;

class DX11GraphicsAPI : public GraphicsAPI
{
 public:
  DX11GraphicsAPI() = default;
  virtual ~DX11GraphicsAPI() = default;

  /**
  * Initialize the api.
  * 
  * @param _wHnd
  * Window handle
  **/
  void 
  init(const Window& _window) override;

  /**
  * Render the final result of the api.
  **/
  void
  render() override;

  /**
  * Create the device and swap chain.
  * 
  * @param _width
  * Client width.
  * 
  * @param _height
  * Client height.
  * 
  * @param _wHnd
  * Handler to the window.
  * 
  * @param _numDriverTypes
  * Number of available types of driver.
  * 
  * @param _createDeviceFlags
  * Flags that can be used in the creation of the device.
  * 
  * @param _featureLevels
  * Levels that can be used.
  * 
  * @param _numFeatureLevels
  * Number of feature levels.
  **/
  void
  createDeviceAndSwapChain(uint32 _width,
                           uint32 _height,
                           WindowHandle& _wHnd,
                           uint32 _numDriverTypes,
                           D3D_DRIVER_TYPE _driverTypes[],
                           uint32 _createDeviceFlags,
                           D3D_FEATURE_LEVEL _featureLevels[],
                           uint32 _numFeatureLevels);

  /**
  * Create the render target view.
  **/
  void
  createRenderTargetView();

  /**
  * Create the sampler state.
  **/
  void
  createSamplerState();


  /**
  * Create the depth stencil texture and view.
  * 
  * @param _width
  * Client width.
  * 
  * @param _height
  * Client height.
  **/
  void
  createDepthStencilTexture(uint32 _width,
                            uint32 _height);

  /**
  * Set the Client viewport.
  *
  * @param _width
  * Client width.
  *
  * @param _height
  * Client height.
  **/
  void
  setViewport(uint32 _width,
              uint32 _height);

  /**
  * Set the buffer of vertex to the model.
  **/
  void
  setVertexBuffers(Model& _model);

  /**
  * set the buffer of index to the model
  **/
  void
  setIndexBuffers(Model& _model);

  /****************************/
  /**
  * Not part of the API
  **/
  /****************************/

  /**
  * Set the shaders
  **/
  void
  setShaders();

  /**
  * Set the constant buffers for the vertex shader.
  **/
  void
  VSSetConstantBuffers();

  /**
  * Set the constant buffers for the pixel shader.
  **/
  void
  PSSetConstantBuffers();

  /**
  * Clear the depth and back buffers
  **/
  void
  clearDepthBackBuffers(float _color[], float _depth = 1.0f);

  /**
  * Draw the model.
  **/
  void
  drawIndexed(Model& model);
  
  // window
  Window m_window;

  // api device
  DX11Device* m_pDevice;
  
  // shaders
  DX11PixelShader m_pixelShader;
  DX11VertexShader m_vertexShader;

  // Render target view
  ID3D11RenderTargetView* m_pRTargetView;

  // swap chain
  IDXGISwapChain* m_pSwapChain;

  // depth stencil
  ID3D11Texture2D* m_pDepthStencil;
  SPtr<DX11DepthStencilView> m_pDepthSView;

  // sampler state
  DX11SamplerState* m_pSamplerLinear;

  // mesh color
  Vector4 m_vMeshColor;

  // world matrix
  Matrix4 m_world;

  // light source
  Light* m_light;

  // constant buffers
  DX11ConstantBuffer m_buffer;
  DX11ConstantBuffer m_cBView;
  DX11ConstantBuffer m_cBProjection;
  DX11ConstantBuffer m_cBWorld;
  DX11ConstantBuffer m_LightCB;
};
}
