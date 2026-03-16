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
#include "DDSTextureLoader.h"

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

class DX11GraphicsAPI : public GraphicsAPI
{
 public:
  DX11GraphicsAPI() = default;
  virtual ~DX11GraphicsAPI() = default;

  /**
   * @brief Initialize the api.
   */
  void
  init(const Window& _window) override;

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
   * @brief Get the API Swap chain
   * @return Swap chain.
   */
  SPtr<SwapChain>
  getSwapChain() override { return m_pSwapChain; }

  /**
   * @brief resize the swap chain.
   * @param _size Size to set the swap chain to.
   */
  void
  resizeSwapChain(const Vector2 _size) override;

  /**
   * @brief Get the device pointer.
   * @return Return te pointer to the device.
   */
  SPtr<Device>
  getDevice() override { return m_pDevice; }

  /**
   * @brief Wait for the device to be idle.
   */
  void
  waitDevice() override;

  /**
   * @brief Set the Client viewport.
   * @param _size Size of the viewport.
   */
  void
  setViewport(Vector2 _size) override;

  /**
   * @brief Set the Client viewport.
   * @param _width Client width.
   * @param _height Client height.
   */
  void
  setViewport(float _width, float _height) override;

  /**
   * @brief Get the client viewport size.
   * @param _vpPos What viewport to get.
   */
  Vector2
  getViewportSize(uint32 _vpPos) override;

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
  setBlendState(const SPtr<BlendState>& _pBlendState) override;

  /**
   * @brief Create the Rasterizer state.
   * @param _desc Rasterizer description.
   * @return Rasterizer state pointer.
   */
  SPtr<RasterizerState>
  createRasterizerState(const RASTERIZER_DESC& _desc) override;

  /**
   * @brief Set the rasterizer state.
   * @param _pRasterizerState Rasterizer state to set.
   */
  void
  setRasterizerState(const SPtr<RasterizerState>& _pRasterizerState) override;

  /**
   * @brief Create the sampler state.
   * @param _mode Mode of the sampler.
   * @param _filter What filter will be used.
   * @return Pointer to the new sampler state.
   */
  SPtr<SamplerState>
  createSamplerState(const uint32 _mode, const uint32 _filter) override;

  /**
   * Set the sampler state.
   */
  void
  setSampler(const SPtr<SamplerState>& _pSamLinear,
             const uint32& _startSlot = 0,
             const uint32& _numSamplers = 1) override;

  /**
   * @brief Set the render target to the device.
   * @param _rTargets Target to set.
   * @param _DepthSV Depth stencil view to use.
   */
  void
  setRenderTarget(const WPtr<Texture>& _pRTarget,
                  const SPtr<Texture>& _pDepthSV = nullptr,
                  const uint32 _mipLevel = 0) override;

  /**
   * @brief Set the render targets to the device.
   * @param _rTargets List of targets to set.
   * @param _DepthSV Depth stencil view to use.
   */
  void
  setRenderTargets(const Vector<WPtr<Texture>> _rTargets,
                   const SPtr<Texture>& _pDepthSV = nullptr,
                   const uint32 _mipLevel = 0) override;

  /**
   * @brief Unbinds all render targets.
   * @param _count How many targets will be unbound.
   */
  void
  unbindRenderTargets(const SIZE_T _count = 8) override;

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
  createVShader(SPtr<Shader>& _pShader) override;

  /**
   * @brief Create a pixel shader.
   * @return Pixel Shader.
   */
  SPtr<Shader>
  createPShader(SPtr<Shader>& _pShader) override;

  /**
   * @brief Create a compute shader.
   * @return Compute shader.
   */
  SPtr<Shader>
  createCShader(SPtr<Shader>& _pShader) override;

  /**
   * @brief Create a geometry shader.
   * @return Geometry shader.
   */
  SPtr<Shader>
  createGShader(SPtr<Shader>& _pShader) override;

  /**
   * @brief Set the vertex shader to the device context.
   * @param _pShader Shader to set.
   * @return If there was a shader to set.
   */
  bool
  setVShader(const SPtr<Shader>& _pShader) override;

  /**
   * @brief Set the pixel shader to the device context.
   * @param _pShader Shader to set.
   * @return If there was a shader to set.
   */
  bool
  setPShader(const SPtr<Shader>& _pShader) override;

  /**
   * @brief Set the compute shader to the device context.
   * @param _pShader Shader to set.
   * @return If there was a shader to set.
   */
  bool
  setCShader(const SPtr<Shader>& _pShader) override;

  /**
   * @brief Set the geometry shader to the device context.
   * @param _pShader Shader to set.
   * @return If there was a shader to set.
   */
  bool
  setGShader(const SPtr<Shader>& _pShader) override;

  /**
   * @brief Compile a shader from a specific file.
   * @param _szFileName What file we will get.
   * @param _szEntryPoint Main function of the shader.
   * @param _szShaderModel What kind of model is the shader.
   * @return Data blob.
   */
  PKBlob*
  compileShaderFromFile(Path _szFileName,
                        const ANSICHAR* _szEntryPoint,
                        const ANSICHAR* _szShaderModel) override;

  /**
   * @brief Create the input layout based on the shader.
   * @param _pShader Shader to use.
   */
  SPtr<InputLayout>
  createInputLayoutFromVShader(const SPtr<Shader>& _pShader) override;

  /**
   * @brief Create the Input Layout.
   * @param _vDesc Description of the input layout.
   * @param _pVShader Shader to use.
   * @return Pointer to the new input layout
   */
  SPtr<InputLayout>
  createInputLayout(const Vector<InputDesc>& _vDesc, const SPtr<Shader> _pVShader) override;

  /**
   * @brief Set input layout
   */
  void
  setInputLayout(const SPtr<InputLayout>& _pInputLayout) override;

  /**
   * @brief Get bytes per pixel from the format.
   * @param _format Format to get the bytes from.
   */
  uint32
  getBytesFromFormat(const uint32 _format) override;

  /**
   * @brief Get the format of a texture from the bytes per pixel.
   * @param _bpp Bytes per pixel of the texture.
   * @return
   */
  uint32
  getFormatFromBytes(const uint32 _bpp) override;

  /**
   * @brief Create a new empty texture.
   * @return Pointer to the texture.
   */
  virtual SPtr<Texture>
  createEmptyTexture() override;

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
   * @param _shaderResourceFormat Format of the shader resource view.
   * @param _mipLevels The maximum number of mipmap levels in the texture.
   * @param _isCube If the texture is a cube map.
   */
  SPtr<Texture>
  createTexture(const uint32 _width,
                const uint32 _height,
                const int32 _format,
                const int32 _usage,
                int32 _bindFlags,
                const int32 _shaderResourceFormat,
                int32 _mipLevels = 1,
                const bool _isCube = false) override;

  /**
   * @brief Create a texture from a pk resource.
   * @param _pResource pointer to the resource.
   * @param _bindFlags What kind of binding will it have.
   * @param _mipLevels MipMap level count.
   * @return Pointer to the texture.
   */
  SPtr<Texture>
  createTextureFromResource(const SPtr<BaseResource>& _pResource,
                            uint32 _bindFlags) override;

  /**
   * @brief Create a texture from a DDS file.
   * @param _directory Directory of the file.
   * @return The texture created.
   */
  SPtr<Texture>
  createDDSTextureFromFile(const Path& _directory) override;

  /**
   * @brief Generate mips for a texture.
   * @param _pTexture Texture to use.
   */
  void
  generateMips(const SPtr<Texture>& _pTexture) override;

  /**
   * @brief Create the vertex buffer.
   * @param _vertex Vector with all the vertex data needed.
   * @param _usage What use will it be given to the buffer.
   */
  SPtr<VertexBuffer>
  createVertexBuffer(const Vector<SimpleVertex>& _vertex,
                     const uint32 _usage = 0) override;

  /**
   * @brief Set data to the vertex buffer.
   * @param _pVertexB Vertex buffer.
   * @param _start Start slot of the buffer.
   * @param _bufferCount How many buffers will be set.
   * @param _offset buffer offsets.
   */
  void
  setVertexBuffer(const SPtr<VertexBuffer>& _pVertexB,
                  const uint32 _start = 0,
                  const uint32 _bufferCount = 1,
                  const uint32 _offset = 0) override;

  /**
   * @brief Create an IndexBuffer.
   * @param _index Vector with all the index data needed.
   * @param _usage What use will it be given to the buffer.
   */
  SPtr<IndexBuffer>
  createIndexBuffer(const Vector<uint32>& _index,
                    const uint32 _usage = 0) override;

  /**
   * @brief Set the index buffer.
   * @param _format What kind of format will the buffer use.
   * @param _offset Distance between blobs of data.
   */
  void
  setIndexBuffer(const SPtr<IndexBuffer>& _pIndexB,
                 const uint32 _format = 42,
                 const uint32 _offset = 0) override;

  /**
   * @brief Create the constant buffer.
   * @param _pDevice What device will create the buffer.
   * @param _size Size of the constant buffer.
   * @param _pData What data will the constant buffer store.
   * @param _usage What usage will be given to the buffer.
   */
  SPtr<ConstantBuffer>
  createConstantBuffer(const uint32 _size,
                       const void* _pData = nullptr,
                       const uint32 _usage = 0) override;

  /**
   * @brief Update the constant buffer.
   * @param _pCBuffer Pointer to the constant buffer.
   * @param _pNewData New data that the buffer will store.
   * @param _size Size of the data to store.
   */
  void
  updateConstantBuffer(const SPtr<ConstantBuffer>& _pCBuffer,
                       const void* _pNewData,
                       const SIZE_T _size) override;

  /**
   * @brief Set resources to a vertex shader.
   * @param _pTextures Textures to set.
   * @param _start In what slot of the vertex shader will the resources be allocated.
   */
  void
  vSSetShaderResourceViews(const Vector<WPtr<Texture>>& _pTextures,
                           const uint32 _start = 0) override;

  /**
   * @brief Unbind resources from a vertex shader.
   * @param _count How many resources will be unbound.
   */
  void
  vSUnbindShaderResourceViews(const SIZE_T _count = 8) override;

  /**
   * @brief Set resources to a pixel shader.
   * @param _pTextures Textures to set.
   * @param _start In what slot of the pixel shader will the resources be allocated.
   */
  void
  pSSetShaderResourceViews(const Vector<WPtr<Texture>>& _pTextures,
                           const uint32 _start = 0) override;

  /**
   * @brief Unbind resources from a pixel shader.
   * @param _count How many resources will be unbound.
   */
  void
  pSUnbindShaderResourceViews(const SIZE_T _count = 8) override;

  /**
   * @brief Set resources to a compute shader.
   * @param _pTextures Textures to set.
   * @param _start In what slot of the compute shader will the resources be allocated.
   */
  void
  cSSetShaderResourceViews(const Vector<WPtr<Texture>>& _pTextures,
                           const uint32 _start = 0) override;

  /**
   * @brief Unbind resources of a compute shader.
   * @param _count How many resources will be unbound.
   */
  void
  cSUnbindShaderResourceViews(const SIZE_T _count = 8) override;

  /**
   * @brief Set unordered views to a compute shader.
   * @param _pTextures Textures to set.
   * @param _start In what slot of the compute shader will the resources be allocated.
   * @param _initialCounts Array of initial values for append or consume UAVs.
   */
  void
  cSSetUnorderedAccessViews(const Vector<SPtr<Texture>>& _pTextures,
                            const uint32 _start = 0,
                            const uint32* _initialCounts = nullptr,
                            const uint32 _mipLevels = 0) override;

  /**
   * @brief Unbind unordered views of a compute shader.
   * @param _count How many resources will be unbound.
   */
  void
  cSUnbindUnorderedAccessViews(const SIZE_T _count = 8) override;

  /**
   * @brief Clear all render target views of a vector.
   * @param _color New render target color.
   * @param _mipSlice Mip slice to clear.
   */
  void
  clearRenderTargetViews(const Color& _color,
                         const Vector<WPtr<Texture>>& _rtvs,
                         const uint32 _mipslice = -1) override;

  /**
   * @brief Clear the render target fiew and fill the screen with a new color.
   * @param _color New screen color.
   * @param _mipSlice Mip slice to clear.
   */
  void
  clearRenderTargetView(const Color& _color,
                        const WPtr<Texture>& _rtv,
                        const uint32 _mipSlice = -1) override;

  /**
   * @brief Clear access view.
   * @param _color New view color.
   */
  void
  clearUnorderedAccessView(const SPtr<Texture>& _uav,
                           const Color& _color = Color(1, 1, 1, 0)) override;

  /**
   * @brief Clear all unordered access views of a vector.
   * @param _color New view color.
   */
  void
  clearUnorderedAccessViews(const Vector<SPtr<Texture>>& _uavs,
                            const Color& _color = Color(1, 1, 1, 0)) override;

  /**
   * @brief Clear the depth buffer.
   * @param _pDepthSV Depth stencil to clear.s
   */
  void
  clearDepthBuffer(const float _depth, const SPtr<Texture>& _pDepthSV) override;

  /**
   * @brief Set the Vertex Shader constant buffers.
   * @param _pCBuffers Vector of pointers to the constant buffers.
   * @param _startSlot Start position of the buffers.
   */
  void
  vSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>>& _pCBuffers,
                       const uint32 _startSlot = 0) override;

  /**
   * @brief Unbind the Vertex Shader constant buffers.
   */
  void
  vSUnbindConstantBuffers(const uint32 _count = 8) override;

  /**
   * @brief Set the Pixel Shader constant buffer.
   * @param _pCBuffers Vertex of Pointers to a constant buffer.
   * @param _startSlot Start position of the buffers.
   */
  void
  pSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>>& _pCBuffers,
                       const uint32 _startSlot = 0) override;

  /**
   * @brief Unbind the Pixel Shader constant buffers.
   * @param _count How many buffers will be unbound.
   */
  void
  pSUnbindConstantBuffers(const uint32 _count = 8) override;

  /**
   * @brief Set the Compute Shader Constant Buffer
   * @param _pCBuffers Vertex of Pointers to a constant buffer.
   * @param _startSlot Start position of the buffers.
   */
  void
  cSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>>& _pCBuffers,
                       const uint32 _startSlot = 0) override;

  /**
   * @brief Unbind the compute Shader constant buffers.
   * @param _count How many buffers will be unbound.
   */
  void
  cSUnbindConstantBuffers(const uint32 _count = 8) override;

  /**
   * @brief Draw the indexed data.
   * @param _indexCount The ammount of index to draw.
   * @param _startIndexLocation Which index will be the starting point.
   */
  void
  draw(uint32 _indexCount,
       uint32 _startIndexLocation) override;

  /**
   * @brief Draw the indexed data.
   * @param _indexCount The ammount of index to draw.
   * @param _startIndexLocation Which index will be the starting point.
   * @param _baseVertexLocation Which vertex will be the starting point.
   */
  void
  drawIndexed(const uint32 _indexCount,
              const uint32 _startIndexLocation,
              const uint32 _baseVertexLocation) override;

  /**
   * @brief Compute shader draw call.
   * @param _countX Thread group size in the X axis.
   * @param _countY Thread group size in the Y axis.
   * @param _countZ Thread group size in the Z axis.
   */
  void
  dispatch(const uint32 _countX, const uint32 _countY, const uint32 _countZ) override;

  /**
   * @brief Present the result to the screen.
   * @param _syncInterval If vertical sync is enabled.
   * @param _flags Swap chain presentation options.
   */
  void
  present(const uint32 _syncInterval, const uint32 _flags) override;
  
 public:
  // swap chain
  SPtr<SwapChain> m_pSwapChain;

 private:
  // api device
  SPtr<DX11Device> m_pDevice;
};
}
