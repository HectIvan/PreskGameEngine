/*****************************************************************************/
/**
 * @file    pkDX11GraphicsAPI.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    19/11/2024
 * @brief   API file using DirectX 11 for the Presk Game Engine.
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
#include "pkDX11ConstantBuffer.h"
#include "pkDX11DepthStencilView.h"
#include "pkDX11Device.h"
#include "pkDX11InputLayout.h"
#include "pkDX11Texture.h"
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
   * @brief Initialize the api.
   * @param _wHnd Window handle.
   */
  void
  initApi(const Window& _window) override;

  /**
   * @brief Create the render target view.
   */
  SPtr<Texture>
  createRenderTargetView() override;

  /**
   * @brief Set the render targets to the device.
   */
  void
  setRenderTargets(Vector<SPtr<Texture>> _rTargets, SPtr<DepthStencilView> _DepthSV) override;

  /**
   * @brief Create the blend state.
   * @return Blend state pointer
   */
  SPtr<BlendState>
  createBlendState() override;

  /**
   * @brief Set the blend state.
   * @param _pBlendState Blend state to set.
   */
  void
  setBlendState(SPtr<BlendState> _pBlendState) override;

  /**
   * @brief Create a vertex shader.
   * @param _pShader Shader to create
   */
  void
  createVShader(SPtr<Shader> _pShader) override;

  /**
   * @brief Create a pixel shader.
   * @param _pShader Shader to create
   */
  void
  createPShader(SPtr<Shader> _pShader) override;

  /**
   * @brief Set the vertex shader to the device context.
   */
  void
  setVSShader(SPtr<Shader> _pShader) override;

  /**
   * @brief Set the pixel shader to the device context.
   */
  void
  setPSShader(SPtr<Shader> _pShader) override;

  /**
   * @brief Compile a shader from a specific file.
   * @param _szFileName What file we will get.
   * @param _szEntryPoint Main function of the shader.
   * @param _szShaderModel What kind of model is the shader.
   * @param _pTargetShader Shader to store the data in.
   */
  void
  compileShaderFromFile(WString _szFileName,
                        const char* _szEntryPoint,
                        const char* _szShaderModel,
                        SPtr<Shader> _pTargetShader) override;

  /**
   * @brief Create the input layout based on the shader.
   * @param _pShader Shader to use.
   */
  SPtr<InputLayout>
  createInputLayoutFromVShader(SPtr<Shader> _pShader) override;

  /**
   * @brief Create a texture.
   * @param _data Data of the image loaded.
   * @param _width How wide is the texture.
   * @param _height How tall is the texture.
   * @param _format Format of the texture.
   * @param _usage What usage will the api give the texture.
   * @param _bindFlags flag for binding to the pipeline stages.
   * @param _mipLevels The maximum number of mipmap levels in the texture.
   */
  SPtr<Texture>
  createTexture(unsigned char* _data,
                uint32 _bpp,
                uint32 _width,
                uint32 _height,
                uint32 _format,
                uint32 _usage,
                uint32 _bindFlags,
                bool _mipLevels) override;

  /**
   * @brief Create the sampler state.
   */
  SPtr<SamplerState>
  createSamplerState(const uint32 _mode, const uint32 _filter) override;

  /**
   * @brief Create the depth stencil texture and view.
   * @param _width Client width.
   * @param _height Client height.
   */
  SPtr<DepthStencilView>
  createDepthStencilView(SPtr<Texture> _depthRT) override;

  /**
   * @brief Set input layout
   */
  void
  setInputLayout(SPtr<InputLayout> _pInputLayout) override;

  /**
   * @brief Create the vertex buffer.
   * @param _vertex Vector with all the vertex data needed.
   * @param _usage What use will it be given to the buffer.
   */
  SPtr<VertexBuffer>
  createVertexBuffer(const Vector<SimpleVertex>& _vertex,
                     uint32 _usage = 0) override;

  /**
   * @brief Set data to the vertex buffer.
   * @param _pVertexB Vertex buffer.
   * @param _start Start slot of the buffer.
   * @param _bufferCount How many buffers will be set.
   * @param _offset buffer offsets.
   */
  void
  setVertexBuffer(SPtr<VertexBuffer>& _pVertexB,
                  uint32 _start = 0,
                  uint32 _bufferCount = 1,
                  uint32 _offset = 0) override;

  /**
   * @brief Create an IndexBuffer.
   * @param _index Vector with all the index data needed.
   * @param _usage What use will it be given to the buffer.
   */
  SPtr<IndexBuffer>
  createIndexBuffer(const Vector<uint32>& _index,
                    uint32 _usage = 0) override;

  /**
  * Set the index buffer.
  *
  * @param _format
  * What kind of format will the buffer use.
  *
  * @param _offset
  * Distance between blobs of data.
  **/
  void
  setIndexBuffer(SPtr<IndexBuffer>& _pIndexB,
                 uint32 _format = 42,
                 uint32 _offset = 0) override;

  /**
   * @brief Create the constant buffer.
   * @param _pDevice What device will create the buffer.
   * @param _size Size of the constant buffer.
   * @param _pData What data will the constant buffer store.
   * @param _usage What usage will be given to the buffer.
   */
  SPtr<ConstantBuffer>
  createConstantBuffer(uint32 _size,
                       const void* _pData,
                       uint32 _usage) override;

  /**
   * @brief Update the constant buffer.
   * @param _pCBuffer Pointer to the constant buffer.
   * @param _pNewData New data that the buffer will store.
   * @param _size Size of the data to store.
   */
  void
  updateConstantBuffer(SPtr<ConstantBuffer> _pCBuffer,
                       const void* _pNewData,
                       uint32 _size) override;

  /**
   * @brief Set a texture to the resource view.
   * @param _pTexture Pointer to the texture.
   * @param _start In what slot of the pixel shader will the resource be allocated.
   * @param _numViews The number of resources that will be passed
   */
  void
  setShaderResourceView(SPtr<Texture> _pTexture,
                        uint32 _start = 0,
                        uint32 _numViews = 1) override;

  /**
   * @brief Set a texture to the resource view of a pixel shader.
   * @param _pTexture Pointer to the texture.
   * @param _start In what slot of the pixel shader will the resource be allocated.
   * @param _numViews The number of resources that will be passed
   */
  void
  PSSetShaderResourceView(SPtr<Texture> _pTexture,
                          uint32 _start = 0,
                          uint32 _numViews = 1) override;

  /**
   * @brief Set a texture to the resource view of a vertex shader.
   * @param _pTexture Pointer to the texture.
   * @param _start In what slot of the pixel shader will the resource be allocated.
   * @param _numViews The number of resources that will be passed
   */
  void
  VSSetShaderResourceView(SPtr<Texture> _pTexture,
                          uint32 _start = 0,
                          uint32 _numViews = 1) override;

  /**
   * @brief Create the device and swap chain.
   * @param _width Client width.
   * @param _height Client height.
   * @param _wHnd Handler to the window.
   * @param _numDriverTypes Number of available types of driver.
   * @param _createDeviceFlags Flags that can be used in the creation of the device.
   * @param _featureLevels Levels that can be used.
   * @param _numFeatureLevels Number of feature levels.
   */
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
   * @brief Set the Client viewport.
   * @param _width Client width.
   * @param _height Client height.
   */
  void
  setViewport(uint32 _width, uint32 _height);

  /**
   * Set the sampler state.
   */
  void
  setSampler(SPtr<SamplerState> _pSamLinear,
             uint32 _startSlot = 0,
             uint32 _numSamplers = 1) override;

  /**
   * @brief Create a texture from file.
   * @param _fileName Name of the texture.
   * @param _bindFlags What kind of binding will it have.
   * @param _mipLevels If the texture has mip levels.
   * @param _format What format will the texture be.
   */
  SPtr<Texture>
  createTextureFromFile(String& _fileName,
                        uint32 _bindFlags,
                        bool _mipLevels,
                        uint32 _format) override;

  /**
   * @brief Get the device pointer.
   * @return Return te pointer to the device.
   */
  SPtr<Device>
  getDevice() override { return m_pDevice; }

  /**
   * @brief Draw the indexed data.
   * @param _indexCount The ammount of index to draw.
   * @param _startIndexLocation Which index will be the starting point.
   * @param _baseVertexLocation Which vertex will be the starting point.
   */
  void
  drawIndexed(uint32 _indexCount,
              uint32 _startIndexLocation,
              uint32 _baseVertexLocation) override;

  /**
   * @brief Clear the render target fiew and fill the screen with a new color.
   * @param _color New screen color.
   */
  void
  clearRenderTargetView(float _color[], SPtr<Texture> _rtv) override;

  /**
   * @brief Clear the depth buffer.
   */
  void
  clearDepthBuffer(float _depth, SPtr<DepthStencilView> _depthSV) override;

  /**
   * @brief Create the Input Layout.
   */
  SPtr<InputLayout>
  createInputLayout(const Vector<InputDesc>& _vDesc, const SPtr<Shader> _pVShader) override;

  /**
   * @brief Set the Vertex Shader constant buffer.
   * @param _pCBuffer Pointer to the constant buffer.
   * @param _startSlot Index into the device's zero-based array.
   * @param _numBuffers Number of buffers to set.
   */
  void
  VSSetConstantBuffer(SPtr<ConstantBuffer> _pCBuffer,
                       uint32 _startSlot,
                       uint32 _numBuffers) override;

  /**
   * @brief Set the Pixel Shader constant buffer.
   * @param _pCBuffer Pointer to the constant buffer.
   * @param _startSlot Index into the device's zero-based array.
   * @param _numBuffers Number of buffers to set.
   */
  void
  PSSetConstantBuffer(SPtr<ConstantBuffer> _pCBuffer,
                       uint32 _startSlot,
                       uint32 _numBuffers) override;

  /**
   * @brief Present the result to the screen.
   * @param _syncInterval If vertical sync is enabled.
   * @param _flags Swap chain presentation options.
   */
  void
  present(uint32 _syncInterval, uint32 _flags) override;
  
 public:

  // swap chain
  SPtr<SwapChain> m_pSwapChain;

  // mesh color
  Vector4 vMeshColor;

 private:
  // api device
  SPtr<Device> m_pDevice;
};
}
