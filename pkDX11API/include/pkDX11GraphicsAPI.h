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
#include "pkDX11Shader.h"


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
   */
  void
  initApi(const Window& _window) override;

  /**
   * @brief Set the render targets to the device.
   * @param _rTargets List of targets to set.
   * @param _DepthSV Depth stencil view to use.
   */
  void
  setRenderTargets(Vector<SPtr<Texture>> _rTargets, SPtr<Texture> _pDepthSV = nullptr) override;

  /**
   * @brief Set the render target to the device.
   * @param _rTargets Target to set.
   * @param _DepthSV Depth stencil view to use.
   */
  void
  setRenderTarget(SPtr<Texture> _pRTarget, SPtr<Texture> _pDepthSV = nullptr) override;

  /**
   * @brief Create the blend state.
   * @return Blend state pointer
   */
  SPtr<BlendState>
  createBlendState() override;

  /**
   * @brief Create the Rasterizer state.
   * @param _desc Rasterizer description.
   * @return Rasterizer state pointer.
   */
  SPtr<RasterizerState>
  createRasterizerState(RASTERIZER_DESC& _desc) override;

  /**
   * @brief Set the blend state.
   * @param _pBlendState Blend state to set.
   */
  void
  setBlendState(SPtr<BlendState> _pBlendState) override;

  /**
   * @brief Set the rasterizer state.
   * @param _pRasterizerState Rasterizer state to set.
   */
  void
  setRasterizerState(SPtr<RasterizerState> _pRasterizerState) override;

  /**
   * @brief Creates a shader of the specific graphic API.
   * @return API Specific shader.
   */
  SPtr<Shader>
  internalCreateShader() override { return make_shared<DX11Shader>(); }

  /**
   * @brief Create a vertex shader.
   * @return Vertex Shader.
   */
  SPtr<Shader>
  createVShader(SPtr<Shader> _pShader) override;

  /**
   * @brief Create a pixel shader.
   * @return Pixel Shader.
   */
  SPtr<Shader>
  createPShader(SPtr<Shader> _pShader) override;

  /**
   * @brief Create a compute shader.
   * @return Compute shader.
   */
  SPtr<Shader>
  createCShader(SPtr<Shader> _pShader) override;

  /**
   * @brief Set the vertex shader to the device context.
   */
  void
  setVShader(SPtr<Shader> _pShader) override;

  /**
   * @brief Set the pixel shader to the device context.
   */
  void
  setPShader(SPtr<Shader> _pShader) override;

  /**
   * @brief Set a compute shader.
   * @return Compute shader.
   */
  void
  setCShader(SPtr<Shader> _pShader) override;

  /**
   * @brief Compile a shader from a specific file.
   * @param _szFileName What file we will get.
   * @param _szEntryPoint Main function of the shader.
   * @param _szShaderModel What kind of model is the shader.
   * @return Data blob.
   */
  void **
  compileShaderFromFile(Path _szFileName,
                        const char* _szEntryPoint,
                        const char* _szShaderModel) override;

  /**
   * @brief Create the input layout based on the shader.
   * @param _pShader Shader to use.
   */
  SPtr<InputLayout>
  createInputLayoutFromVShader(SPtr<Shader> _pShader) override;

  /**
   * @brief Create a texture.
   * @param _desc Texture descrition.
   * @return Texture.
   */
  SPtr<Texture>
  createTexture(const TextureDesc& _desc) override;

  /**
   * @brief Create a texture.
   * @param _width How wide is the texture.
   * @param _height How tall is the texture.
   * @param _format Format of the texture.
   * @param _usage What usage will the api give the texture.
   * @param _bindFlags flag for binding to the pipeline stages.
   * @param _mipLevels The maximum number of mipmap levels in the texture.
   * @param _data Data of the image loaded.
   */
  SPtr<Texture>
  createTexture(uint32 _bpp,
                uint32 _width,
                uint32 _height,
                int32 _format,
                int32 _usage,
                int32 _bindFlags,
                bool _mipLevels,
                int32 _shaderResourceFormat,
                unsigned char* _data = nullptr) override;

  /**
   * @brief Create the sampler state.
   */
  SPtr<SamplerState>
  createSamplerState(const uint32 _mode, const uint32 _filter) override;

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
                       const void* _pData = nullptr,
                       uint32 _usage = 0) override;

  /**
   * @brief Update the constant buffer.
   * @param _pCBuffer Pointer to the constant buffer.
   * @param _pNewData New data that the buffer will store.
   * @param _size Size of the data to store.
   */
  void
  updateConstantBuffer(SPtr<ConstantBuffer> _pCBuffer,
                       const void* _pNewData,
                       SIZE_T _size) override;

  /**
   * @brief Set a resource to the vertex shader.
   * @param _pTextures Textures to set.
   * @param _start In what slot of the vertex shader will the resources be allocated.
   */
  void
  vSSetShaderResourceViews(Vector<SPtr<Texture>> _pTextures, uint32 _start = 0) override;

  /**
   * @brief Set resources to a pixel shader.
   * @param _pTextures Textures to set.
   * @param _start In what slot of the pixel shader will the resources be allocated.
   */
  void
  pSSetShaderResourceViews(Vector<SPtr<Texture>> _pTextures, uint32 _start = 0) override;

  /**
   * @brief Set resources to a compute shader.
   * @param _pTextures Textures to set.
   * @param _start In what slot of the compute shader will the resources be allocated.
   */
  void
  cSSetShaderResourceViews(Vector<SPtr<Texture>> _pTextures, uint32 _start = 0) override;

  /**
   * @brief Set unordered views to a compute shader.
   * @param _pTextures Textures to set.
   * @param _start In what slot of the compute shader will the resources be allocated.
   * @param _initialCounts Array of initial values for append or consume UAVs.
   */
  void
  cSSetUnorderedAccessViews(Vector<SPtr<Texture>> _pTextures,
                            uint32 _start = 0,
                            uint32* _initialCounts = nullptr) override;

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
   * @brief Get the client viewport size.
   * @pa
   */
  Vector2
  getViewportSize(uint32 _vpPos);

  /**
   * Set the sampler state.
   */
  void
  setSampler(SPtr<SamplerState> _pSamLinear,
             uint32 _startSlot = 0,
             uint32 _numSamplers = 1) override;

  /**
   * @brief Create a texture from file.
   * @param _directory Directory of the texture.
   * @param _bindFlags What kind of binding will it have.
   * @param _mipLevels If the texture has mip levels.
   * @param _format What format will the texture be.
   */
  SPtr<Texture>
  createTextureFromFile(const Path& _directory,
                        uint32 _bindFlags,
                        bool _mipLevels,
                        uint32 _format) override;

  /**
   * @brief Create a texture from file as float.
   * @param _directory Directory of the texture.
   * @param _bindFlags What kind of binding will it have.
   * @param _mipLevels If the texture has mip levels.
   * @return Pointer to the texture.
   */
  SPtr<Texture>
  createTextureFromFileF(const Path& _directory,
                         uint32 _bindFlags,
                         bool _mipLevels) override;

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
   * @brief Draw the indexed data.
   * @param _indexCount The ammount of index to draw.
   * @param _startIndexLocation Which index will be the starting point.
   */
  void
  draw(uint32 _indexCount,
      uint32 _startIndexLocation) override;

  /**
   * @brief Compute shader draw call.
   * @param _countX Thread group size in the X axis.
   * @param _countY Thread group size in the Y axis.
   * @param _countZ Thread group size in the Z axis.
   */
  void
  dispatch(uint32 _countX, uint32 _countY, uint32 _countZ) override;

  /**
   * @brief Clear all render target views of a vector.
   * @param _color New render target color.
   */
  void
  clearRenderTargetViews(const Color& _color, Vector<SPtr<Texture>> _rtvs) override;

  /**
   * @brief Clear the render target fiew and fill the screen with a new color.
   * @param _color New screen color.
   */
  void
  clearRenderTargetView(const Color& _color, SPtr<Texture> _rtv) override;

  /**
   * @brief Clear all unordered access views of a vector.
   * @param _color New view color.
   */
  void
  clearUnorderedAccessViews(Vector<SPtr<Texture>> _uavs,
                            const Color& _color = Color(1, 1, 1, 0)) override;

  /**
   * @brief Clear access view.
   * @param _color New view color.
   */
  void
  clearUnorderedAccessView(SPtr<Texture> _uav,
                           const Color& _color = Color(1, 1, 1, 0)) override;

  /**
   * @brief Clear the depth buffer.
   * @param _pDepthSV Depth stencil to clear.s
   */
  void
  clearDepthBuffer(float _depth, SPtr<Texture> _pDepthSV) override;

  /**
   * @brief Create the Input Layout.
   */
  SPtr<InputLayout>
  createInputLayout(const Vector<InputDesc>& _vDesc, const SPtr<Shader> _pVShader) override;

  /**
   * @brief Set the Vertex Shader constant buffer.
   * @param _pCBuffers Pointer to the constant buffer.
   * @param _startSlot Start position of the buffers.
   */
  void
  vSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>>& _pCBuffers,
                       const uint32 _startSlot = 0) override;

  /**
   * @brief Set the Pixel Shader constant buffer.
   * @param _pCBuffers Vertex of Pointers to a constant buffer.
   * @param _startSlot Start position of the buffers.
   */
  void
  pSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>>& _pCBuffers,
                       const uint32 _startSlot = 0) override;

  /**
   * @brief Set the Compute Shader Constant Buffer
   * @param _pCBuffers Vertex of Pointers to a constant buffer.
   * @param _startSlot Start position of the buffers.
   */
  void
  cSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>>& _pCBuffers,
                       const uint32 _startSlot = 0) override;

  /**
   * @brief Get the API Swap chain
   * @return Swap chain.
   */
  SPtr<SwapChain>
  getSwapChain() override { return m_pSwapChain; }

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
