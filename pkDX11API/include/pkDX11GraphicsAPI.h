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
  DX11GraphicsAPI(const Window& _window);
  virtual ~DX11GraphicsAPI() = default;

  /**
  * Initialize the api.
  * 
  * @param _wHnd
  * Window handle
  **/
  void
  initApi(const Window& _window) override;

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
  createDeviceAndSwapChain(uint32& _width,
                           uint32& _height,
                           WindowHandle& _wHnd,
                           uint32& _numDriverTypes,
                           D3D_DRIVER_TYPE _driverTypes[],
                           uint32& _createDeviceFlags,
                           D3D_FEATURE_LEVEL _featureLevels[],
                           uint32& _numFeatureLevels);

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
  * Set the sampler state.
  **/
  void
  setSampler() override;

  SPtr<VertexBuffer>
  createVertexBuffer(const Vector<SimpleVertex>& _vertex,
                     uint32 _usage = 0);

  void
  setVertexBuffer(SPtr<VertexBuffer>& _pVertexB,
                  uint32 _start = 0,
                  uint32 _bufferCount = 1,
                  uint32 _offset = 0);

  /**
  * Create an IndexBuffer
  **/
  SPtr<IndexBuffer>
  createIndexBuffer(const Vector<uint32>& _index,
                    uint32 _usage = 0);

  void
  setIndexBuffer(SPtr<IndexBuffer>& _pIndexB,
                 uint32 _format = 42,
                 uint32 _offset = 0);

  /**
  * Draw the indexed data.
  * 
  * @param _indexCount
  * The ammount of index to draw.
  * 
  * @param _startIndexLocation
  * Which index will be the starting point.
  * 
  * @param _baseVertexLocation
  * Which vertex will be the starting point.
  **/
  void
  drawIndexed(uint32 _indexCount,
              uint32 _startIndexLocation,
              uint32 _baseVertexLocation) override;

  void
  updateCamera(Camera* _pCamera) override;

  /**
  * Clear the render target fiew and fill the
  * screen with a new color.
  * 
  * @param _color
  * New screen color.
  **/
  void
  clearRenderTargetView(float _color[]) override;

  /**
  * Clear the depth buffer.
  **/
  void
  clearDepthBuffer(float _depth) override;

  /**
  * Update the light and world constant buffers.
  **/
  void
  updateWorldAndLightCB() override;

  /**
  * Set the shaders of the api.
  **/
  void
  setShaders() override;

  /**
  * Set the Vertex Shader constant buffers.
  **/
  void
  VSSetConstantBuffers() override;

  /**
  * Set the Pixel Shader constant buffers.
  **/
  void
  PSSetConstantBuffers() override;

  /**
  * Present the result to the screen.
  * 
  * @param _syncInterval
  * If vertical sync is enabled.
  * 
  * @param _flags
  * Swap chain presentation options.
  **/
  void
  present(uint32 _syncInterval, uint32 _flags) override;
  
 public:
  // window
  Window window;
  
  // shaders
  DX11PixelShader pixelShader;
  DX11VertexShader vertexShader;

  // Render target view
  ID3D11RenderTargetView* pRTargetView;

  // swap chain
  IDXGISwapChain* pSwapChain;

  // depth stencil
  ID3D11Texture2D* pDepthStencil;
  SPtr<DX11DepthStencilView> pDepthSView;

  // sampler state
  DX11SamplerState* pSamplerLinear;

  // mesh color
  Vector4 vMeshColor;

  // world matrix
  Matrix4 world;

  // light source
  Light light;

  // constant buffers
  DX11ConstantBuffer buffer;
  DX11ConstantBuffer cBView;
  DX11ConstantBuffer cBProjection;
  DX11ConstantBuffer cBWorld;
  DX11ConstantBuffer cbLight;

 private:
  // api device
  DX11Device* m_pDevice;
};
}
