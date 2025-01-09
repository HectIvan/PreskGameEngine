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
#include "pkDX11InputLayout.h"
#include "pkDX11PixelShader.h"
#include "pkDX11RenderTargetView.h"
#include "pkDX11SamplerState.h"
#include "pkDX11SwapChain.h"
#include "pkDX11Texture.h"
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
  createSamplerState() override;

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
  setViewport(uint32 _width, uint32 _height);

  /**
  * Set the sampler state.
  **/
  void
  setSampler(uint32 _startSlot = 0,
             uint32 _numSamplers = 1) override;

  /**
  * Set a texture to the resource view.
  * 
  * param _pTexture
  * Pointer to the texture.
  * 
  * @param _start
  * Where the setting will start.
  * 
  * @param _numViews
  * How many views are there.
  **/
  void
  setShaderResourceView(SPtr<Texture> _pTexture,
                        uint32 _start = 0,
                        uint32 _numViews = 1) override;

  /**
  * create a texture from file.
  * 
  * @param _fileName
  * Name of the texture.
  * 
  * @param _bindFlags
  * What kind of binding will it have
  **/
  SPtr<Texture>
  createTextureFromFile(String& _fileName,
                        uint32 _bindFlags,
                        bool _mipLevels,
                        uint32 _format) override;

  /**
  * Create a texture.
  * 
  * @param _data
  * Data of the image loaded.
  * 
  * @param _width
  * How wide is the texture.
  * 
  * @param _height
  * How tall is the texture.
  * 
  * @param _format
  * Format of the texture.
  * 
  * @param _usage
  * What usage will the api give the texture.
  * 
  * @param _bindFlags
  * flag for binding to the pipeline stages.
  * 
  * @param _mipLevels
  * The maximum number of mipmap levels in the texture.
  **/
  SPtr<Texture>
  createTextureDX(unsigned char* _data,
                  uint32 _bpp,
                  uint32 _width,
                  uint32 _height,
                  uint32 _format,
                  uint32 _usage,
                  uint32 _bindFlags,
                  bool _mipLevels);

  /**
  * Set input layout
  **/
  void
  setInputLayout() override;

  /**
  * Create the vertex buffer.
  * 
  * @param _vertex
  * Vector with all the vertex data needed.
  * 
  * @param _usage
  * What use will it be given to the buffer.
  **/
  SPtr<VertexBuffer>
  createVertexBuffer(const Vector<SimpleVertex>& _vertex,
                     uint32 _usage = 0) override;

  void
  setVertexBuffer(SPtr<VertexBuffer>& _pVertexB,
                  uint32 _start = 0,
                  uint32 _bufferCount = 1,
                  uint32 _offset = 0) override;

  /**
  * Create an IndexBuffer
  * 
  * @param _index
  * Vector with all the index data needed.
  * 
  * @param _usage
  * What use will it be given to the buffer.
  **/
  SPtr<IndexBuffer>
  createIndexBuffer(const Vector<uint32>& _index,
                    uint32 _usage = 0) override;

  void
  setIndexBuffer(SPtr<IndexBuffer>& _pIndexB,
                 uint32 _format = 42,
                 uint32 _offset = 0) override;

  /**
  * Create the constant buffer.
  * 
  * @param _pDevice
  * What device will create the buffer.
  * 
  * @param _size
  * Size of the constant buffer.
  * 
  * @param _pData
  * What data will the constant buffer store.
  * 
  * @param _usage
  * What usage will be given to the buffer.
  **/
  SPtr<ConstantBuffer>
  createConstantBuffer(uint32 _size,
                       const void* _pData,
                       uint32 _usage) override;

  /**
  * Update the constant buffer.
  * 
  * @param _pCBuffer
  * Pointer to the constant buffer.
  * 
  * @param _pNewData
  * New data that the buffer will store.
  * 
  * @param _size
  * Size of the data to store.
  **/
  void
  updateConstantBuffer(SPtr<ConstantBuffer> _pCBuffer,
                       const void* _pNewData,
                       uint32 _size) override;

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
  * Compile shaders.
  **/
  void
  compileShaders() override;

  /**
  * Create the pixel shader
  **/
  void
  createPShader() override;

  /**
  * Create the vertex shader
  **/
  void
  createVShader() override;

  /**
  * Create shaders.
  **/
  void
  createShaders() override;

  /**
  * Set the shaders of the api.
  **/
  void
  setShaders() override;

  /**
  * Create the Input Layout.
  **/
  void
  createInputLayout() override;

  /**
  * Set the Vertex Shader constant buffer.
  * 
  * @param _pCBuffer
  * Pointer to the constant buffer.
  * 
  * @param _startSlot
  * Index into the device's zero-based array.
  * 
  * @param _numBuffers
  * Number of buffers to set.
  **/
  void
  VSSetConstantBuffer(SPtr<ConstantBuffer> _pCBuffer,
                       uint32 _startSlot,
                       uint32 _numBuffers) override;

  /**
  * Set the Pixel Shader constant buffer.
  *
  * @param _pCBuffer
  * Pointer to the constant buffer.
  *
  * @param _startSlot
  * Index into the device's zero-based array.
  *
  * @param _numBuffers
  * Number of buffers to set.
  **/
  void
  PSSetConstantBuffer(SPtr<ConstantBuffer> _pCBuffer,
                       uint32 _startSlot,
                       uint32 _numBuffers) override;

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
  SPtr<DX11PixelShader> pixelShader;
  SPtr<DX11VertexShader> vertexShader;

  // Render target view
  SPtr<DX11RenderTargetView> pRTargetView;

  // swap chain
  SPtr<DX11SwapChain> pSwapChain;

  // input layout
  SPtr<DX11InputLayout> pInputL;

  // depth stencil
  SPtr<DX11Texture> pDepthStencil;
  SPtr<DX11DepthStencilView> pDepthSView;

  // sampler state
  SPtr<DX11SamplerState> pSamplerLinear;

  // mesh color
  Vector4 vMeshColor;

 private:
  // api device
  SPtr<DX11Device> m_pDevice;
};
}
