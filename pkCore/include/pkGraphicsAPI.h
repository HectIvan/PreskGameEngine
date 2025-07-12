/*****************************************************************************/
/**
 * @file    pkGraphicsAPI.h
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    19/11/2024
 * @brief   Graphics API class for the engine
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
#include "pkBlendState.h"
#include "pkDepthStencilView.h"
#include "pkInputLayout.h"
#include "pkInputLayoutDesc.h"
#include "pkModel.h"
#include "pkModule.h"
#include "pkPath.h"
#include "pkPrerequisitesCore.h"
#include "pkRasterizerState.h"
#include "pkRenderTargetView.h"
#include "pkTexture.h"
#include "pkSwapChain.h"
#include "pkSamplerState.h"
#include "pkShader.h"
#include "pkWindow.h"

namespace pkEngineSDK
{

class ConstantBuffer;
class Device;
class IndexBuffer;
class VertexBuffer;

class PK_CORE_EXPORT GraphicsAPI : public Module<GraphicsAPI>
{
 public:
  GraphicsAPI() = default;
  virtual ~GraphicsAPI() = default;

  /**
  * @brief Initialize the graphic api.
  * @param _wHnd Handler to the window to be used.
  **/
  virtual void
  initApi(const Window& _window) = 0;

  /**
   * @brief Set the render targets to the device.
   * @param _rTargets List of targets to set.
   * @param _DepthSV Depth stencil view to use.
   */
  virtual void
  setRenderTargets(Vector<SPtr<Texture>> _rTargets, SPtr<Texture> _pDepthSV = nullptr) = 0;

  /**
   * @brief Set the render target to the device.
   * @param _rTargets Target to set.
   * @param _DepthSV Depth stencil view to use.
   */
  virtual void
  setRenderTarget(SPtr<Texture> _pRTarget, SPtr<Texture> _pDepthSV = nullptr) = 0;

  /**
   * @brief Create the blend state.
   * @return Blend state pointer
   */
  virtual SPtr<BlendState>
  createBlendState() = 0;

  /**
   * @brief Create the Rasterizer state.
   * @param _desc Rasterizer description.
   * @return Rasterizer state pointer.
   */
  virtual SPtr<RasterizerState>
  createRasterizerState(RASTERIZER_DESC& _desc) = 0;

  /**
   * @brief Set the rasterizer state.
   * @param _pRasterizerState Rasterizer state to set.
   */
  virtual void
  setRasterizerState(SPtr<RasterizerState> _pRasterizerState) = 0;

  /**
   * @brief Set the blend state.
   * @param _pBlendState Blend state to set.
   */
  virtual void
  setBlendState(SPtr<BlendState> _pBlendState) = 0;

  /**
   * @brief Creates a shader of the specific graphic API.
   * @return API Specific shader.
   */
  virtual SPtr<Shader>
  internalCreateShader() = 0;

  /**
   * @brief Create a vertex shader.
   * @returtn Vertex Shader.
   */
  virtual SPtr<Shader>
  createVShader(SPtr<Shader> _pShader) = 0;

  /**
   * @brief Create a pixel shader.
   * @return Pixel Shader.
   */
  virtual SPtr<Shader>
  createPShader(SPtr<Shader> _pShader) = 0;

  /**
   * @brief Create a compute shader.
   * @return Compute shader.
   */
  virtual SPtr<Shader>
  createCShader(SPtr<Shader> _pShader) = 0;

  /**
   * @brief Set the vertex shader to the device context.
   * @param _pShader Shader to set.
   */
  virtual void
  setVShader(SPtr<Shader> _pShader) = 0;

  /**
   * @brief Set the pixel shader to the device context.
   * @param _pShader Shader to set.
   */
  virtual void
  setPShader(SPtr<Shader> _pShader) = 0;

  /**
   * @brief Set a compute shader.
   * @param _pShader Compute shader.
   */
  virtual void
  setCShader(SPtr<Shader> _pShader) = 0;

  /**
   * @brief Get the API Swap chain
   * @return Swap chain.
   */
  virtual SPtr<SwapChain>
  getSwapChain() = 0;

  /**
   * @brief Compile a shader from a specific file.
   * @param _szFileName What file we will get.
   * @param _szEntryPoint Main function of the shader.
   * @param _szShaderModel What kind of model is the shader.
   * @return Data blob.
   */
  virtual void**
  compileShaderFromFile(Path _szFileName,
                        const char* _szEntryPoint,
                        const char* _szShaderModel) = 0;

  /**
   * @brief Create the input layout based on the shader.
   * @param _pShader Shader to use.
   * @return Input layout pointer
   */
  virtual SPtr<InputLayout>
  createInputLayoutFromVShader(SPtr<Shader> _pShader) = 0;

  /**
   * @brief Create a texture.
   * @param _desc Texture descrition.
   * @return Texture.
   */
  virtual SPtr<Texture>
  createTexture(const TextureDesc& _desc) = 0;

  /**
  * @brief Create a texture.
  * @param _data Data of the image loaded.
  * @param _width How wide is the texture.
  * @param _height How tall is the texture.
  * @param _format Format of the texture.
  * @param _usage What usage will the api give the texture.
  * @param _bindFlags flag for binding to the pipeline stages.
  * @param _mipLevels The maximum number of mipmap levels in the texture.
  **/
  virtual SPtr<Texture>
  createTexture(uint32 _bpp,
                uint32 _width,
                uint32 _height,
                int32 _format,
                int32 _usage,
                int32 _bindFlags,
                bool _mipLevels,
                int32 _shaderResourceFormat,
                int32 _miscflags = 0,
                unsigned char* _data = nullptr) = 0;

  /**
   * @brief Create the sampler state.
   * @param _mode Mode of the sampler.
   * @param _filter What filter will be used.
   * @return Pointer to the new sampler state.
   */
  virtual SPtr<SamplerState>
  createSamplerState(const uint32 _mode, const uint32 _filter) = 0;

  /**
   * @brief Set input layout.
   * @param _pInputLayout What input layout to use.
   */
  virtual void
  setInputLayout(SPtr<InputLayout> _pInputLayout) = 0;

  /**
   * @brief Create a VertexBuffer.
   * @param _vertex Vertex for the buffer.
   * @param _usage What use will be given to the buffer.
   * @return Pointer to the Vertex Buffer.
   */
  virtual SPtr<VertexBuffer>
  createVertexBuffer(const Vector<SimpleVertex>& _vertex,
                     uint32 _usage = 0) = 0;

  /**
   * @brief Set data to the vertex buffer.
   * @param _pVertexB Vertex buffer to set.
   * @param _start Where the buffer starts.
   * @param _bufferCount How many buffers are there.
   * @param _offset Difference in size between buffers.
   */
  virtual void
  setVertexBuffer(SPtr<VertexBuffer>& _pVertexB,
                  uint32 _start = 0,
                  uint32 _bufferCount = 1,
                  uint32 _offset = 0) = 0;

  /**
   * @brief Create an IndexBuffer.
   * @param _index Index for the buffer.
   * @param _usage What use will be given to the buffer.
   * @return Pointer to the Vertex Buffer.
   */
  virtual SPtr<IndexBuffer>
  createIndexBuffer(const Vector<uint32>& _index,
                    uint32 _usage = 0) = 0;

  /**
   * @brief Set the index buffer.
   * @param _format What kind of format will the buffer use.
   * @param _offset Distance between blobs of data.
   */
  virtual void
  setIndexBuffer(SPtr<IndexBuffer>& _pIndexB,
                 uint32 _format = 42,
                 uint32 _offset = 0) = 0;

  /**
   * @brief Create the constant buffer.
   * @param _size Size of the constant buffer.
   * @param _pData What data will the constant buffer store.
   * @param _usage What usage will be given to the buffer.
   * @return Pointer to the constant buffer
   */
  virtual SPtr<ConstantBuffer>
  createConstantBuffer(uint32 _size,
                       const void* _pData = nullptr,
                       uint32 _usage = 0) = 0;

  /**
   * @brief Update the constant buffer.
   * @param _pCBuffer Pointer to the constant buffer.
   * @param _pNewData New data that the buffer will store.
   * @param _size Size of the data to store.
   */
  virtual void
  updateConstantBuffer(SPtr<ConstantBuffer> _pCBuffer,
                       const void* _pNewData,
                       SIZE_T _size) = 0;

  /**
   * @brief Set resources to a vertex shader.
   * @param _pTextures Textures to set.
   * @param _start In what slot of the vertex shader will the resources be allocated.
   */
  virtual void
  vSSetShaderResourceViews(Vector<SPtr<Texture>> _pTextures, uint32 _start = 0) = 0;

  /**
   * @brief Set resources to a pixel shader.
   * @param _pTextures Textures to set.
   * @param _start In what slot of the pixel shader will the resources be allocated.
   */
  virtual void
  pSSetShaderResourceViews(Vector<SPtr<Texture>> _pTextures, uint32 _start = 0) = 0;

  /**
   * @brief Set resources to a pixel shader.
   * @param _pTextures Textures to set.
   * @param _start In what slot of the pixel shader will the resources be allocated
   */
  virtual void
  cSSetShaderResourceViews(Vector<SPtr<Texture>> _pTextures, uint32 _start = 0) = 0;

  /**
   * @brief Set unordered views to a compute shader.
   * @param _pTextures Textures to set.
   * @param _start In what slot of the compute shader will the resources be allocated.
   * @param _initialCounts Array of initial values for append or consume UAVs.
   */
  virtual void
  cSSetUnorderedAccessViews(Vector<SPtr<Texture>> _pTextures,
                            uint32 _start = 0,
                            uint32* _initialCounts = nullptr) = 0;

  /**
   * @brief Clear all render target views of a vector.
   * @param _color New render target color.
   */
  virtual void
  clearRenderTargetViews(const Color& _color, Vector<SPtr<Texture>> _rtvs) = 0;

  /**
   * @brief Clear the render target fiew and fill the screen with a new color.
   * @param _color New screen color.
   */
  virtual void
  clearRenderTargetView(const Color& _color, SPtr<Texture> _rtv) = 0;

  /**
   * @brief Clear all unordered access views of a vector.
   * @param _uavs Vector of UAVs.
   * @param _color New view color.
   */
  virtual void
  clearUnorderedAccessViews(Vector<SPtr<Texture>> _uavs,
                            const Color& _color = Color(1,1,1,0)) = 0;

  /**
   * @brief Clear access view.
   * @param _color New view color.
   */
  virtual void
  clearUnorderedAccessView(SPtr<Texture> _uav, const Color& _color = Color(1, 1, 1, 0)) = 0;

  /**
   * @brief clear the depth buffer.
   * @param _depth Default depth of the stencil.
   * @param _pDepthSV Depth stencil view to clear.
   */
  virtual void
  clearDepthBuffer(float _depth, SPtr<Texture> _pDepthSV) = 0;

  /**
   * @brief Create the Input Layout.
   * @param _vDesc Description of the input layout.
   * @param _pVShader Shader to use.
   * @return Pointer to the new input layout
   */
  virtual SPtr<InputLayout>
  createInputLayout(const Vector<InputDesc>& _vDesc, const SPtr<Shader> _pVShader) = 0;

  /**
   * @brief Set the Vertex Shader constant buffer.
   * @param _pCBuffers Pointer to the constant buffer.
   * @param _startSlot Start position of the buffers.
   */
  virtual void
  vSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>>& _pCBuffers,
                       const uint32 _startSlot = 0) = 0;

  /**
   * @brief Set the Pixel Shader constant buffer.
   * @param _pCBuffers Vertex of Pointers to a constant buffer.
   * @brief _startSlot Start position of the buffers.
   */
  virtual void
  pSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>>& _pCBuffers,
                       const uint32 _startSlot = 0) = 0;

  /**
   * @brief Set the Compute Shader Constant Buffer.
   * @param _pCBuffers Pointer to the constant buffer.
   * @param _startSlot Start position of the buffers.
   */
  virtual void
  cSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>>& _pCBuffers,
                       const uint32 _startSlot = 0) = 0;

  /**
  * Set the sampler state.
  **/
  virtual void
  setSampler(SPtr<SamplerState> _pSamLinear,
             uint32 _startSlot = 0,
             uint32 _numSamplers = 1) = 0;

  /**
   * @brief Create a texture from file.
   * @param _directory Directory of the texture.
   * @param _bindFlags What kind of binding will it have.
   * @param _bindFlags Bind flags of the texture.
   * @param _format Format of the texture.
   * @return Pointer to the texture.
   */
  virtual SPtr<Texture>
  createTextureFromFile(const Path& _directory,
                        uint32 _bindFlags,
                        bool _mipLevels,
                        uint32 _format,
                        int32 _miscFlags = 0) = 0;

  /**
   * @brief Create a texture from file as float.
   * @param _directory Directory of the texture.
   * @param _bindFlags What kind of binding will it have.
   * @param _mipLevels If the texture has mip levels.
   * @return Pointer to the texture.
   */
  virtual SPtr<Texture>
  createTextureFromFileF(const Path& _directory,
                         uint32 _bindFlags,
                         bool _mipLevels,
                         int32 _miscFlags = 0) = 0;

  virtual SPtr<Texture>
  createDDSTextureFromFile(const Path& _directory) = 0;

  /**
   * @brief Get the api device.
   * @return The Pointer to the device
   */
  virtual SPtr<Device>
  getDevice() = 0;

  /**
   * @brief Draw the indexed data.
   * @param _indexCount The ammount of index to draw.
   * @param _startIndexLocation Which index will be the starting point.
   * @param _baseVertexLocation Which vertex will be the starting point.
   */
  virtual void
  drawIndexed(uint32 _indexCount,
              uint32 _startIndexLocation,
              uint32 _baseVertexLocation) = 0;

  /**
   * @brief Draw the indexed data.
   * @param _indexCount The ammount of index to draw.
   * @param _startIndexLocation Which index will be the starting point.
   */
  virtual void
  draw(uint32 _indexCount,
       uint32 _startIndexLocation) = 0;

  /**
   * @brief Compute shader draw call.
   * @param _countX Thread group size in the X axis.
   * @param _countY Thread group size in the Y axis.
   * @param _countZ Thread group size in the Z axis.
   */
  virtual void
  dispatch(uint32 _countX, uint32 _countY, uint32 _countZ) = 0;

  /**
   * @brief Present the result to the screen.
   * @param _syncInterval If vertical sync is enabled.
   * @param _flags Swap chain presentation options.
   */
  virtual void
  present(uint32 _syncInterval, uint32 _flags) = 0;

};

PK_CORE_EXPORT GraphicsAPI&
g_GraphicAPI();
}
