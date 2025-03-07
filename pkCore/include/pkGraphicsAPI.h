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
#include "pkPrerequisitesCore.h"
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
  * Initialize the graphic api.
  *
  * @param _wHnd
  * Handler to the window to be used.
  **/
  virtual void
  initApi(const Window& _window) = 0;

  /**
   * @brief Create the render target view.
   */
  virtual SPtr<Texture>
  createRenderTargetView() = 0;

  /**
   * @brief Set the render targets to the device.
   */
  virtual void
  setRenderTargets(Vector<SPtr<Texture>> _rTargets, SPtr<DepthStencilView> _DepthSV) = 0;

  /**
   * @brief Create the blend state.
   * @return Blend state pointer
   */
  virtual SPtr<BlendState>
  createBlendState() = 0;

  /**
   * @brief Set the blend state.
   * @param _pBlendState Blend state to set.
   */
  virtual void
  setBlendState(SPtr<BlendState> _pBlendState) = 0;

  /**
   * @brief Create a vertex shader.
   * @param _pShader Shader to create
   */
  virtual void
  createVShader(SPtr<Shader> _pShader) = 0;

  /**
   * @brief Create a pixel shader.
   * @param _pShader Shader to create
   */
  virtual void
  createPShader(SPtr<Shader> _pShader) = 0;

  /**
   * @brief Set the vertex shader to the device context.
   */
  virtual void
  setVSShader(SPtr<Shader> _pShader) = 0;

  /**
   * @brief Set the pixel shader to the device context.
   */
  virtual void
  setPSShader(SPtr<Shader> _pShader) = 0;

  /**
   * @brief Compile a shader from a specific file.
   * @param _szFileName What file we will get.
   * @param _szEntryPoint Main function of the shader.
   * @param _szShaderModel What kind of model is the shader.
   * @param _pTargetShader Shader to store the data in.
   */
  virtual void
  compileShaderFromFile(WString _szFileName,
                        const char* _szEntryPoint,
                        const char* _szShaderModel,
                        SPtr<Shader> _pTargetShader) = 0;

  /**
   * @brief Create the input layout based on the shader.
   * @param _pShader Shader to use.
   * @return Input layout pointer
   */
  virtual SPtr<InputLayout>
  createInputLayoutFromVShader(SPtr<Shader> _pShader) = 0;

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
  createTexture(unsigned char* _data,
                uint32 _bpp,
                uint32 _width,
                uint32 _height,
                uint32 _format,
                uint32 _usage,
                uint32 _bindFlags,
                bool _mipLevels) = 0;

  /**
  * Create the sampler state.
  **/
  virtual SPtr<SamplerState>
  createSamplerState(const uint32 _mode, const uint32 _filter) = 0;

  /**
   * @brief Create the depth stencil texture and view.
   * @param _width Client width.
   * @param _height Client height.
   */
  virtual SPtr<DepthStencilView>
  createDepthStencilView(uint32 _width, uint32 _height, SPtr<Texture> _depthRT) = 0;

  /**
  * Set input layout
  **/
  virtual void
  setInputLayout(SPtr<InputLayout> _pInputLayout) = 0;

  /**
  * Create a VertexBuffer
  **/
  virtual SPtr<VertexBuffer>
  createVertexBuffer(const Vector<SimpleVertex>& _vertex,
                     uint32 _usage = 0) = 0;

  /**
  * Set data to the vertex buffer
  **/
  virtual void
  setVertexBuffer(SPtr<VertexBuffer>& _pVertexB,
                  uint32 _start = 0,
                  uint32 _bufferCount = 1,
                  uint32 _offset = 0) = 0;

  /**
  * Create an IndexBuffer
  **/
  virtual SPtr<IndexBuffer>
  createIndexBuffer(const Vector<uint32>& _index,
                    uint32 _usage = 0) = 0;

  /**
  * Set the index buffer.
  *
  * @param _format
  * What kind of format will the buffer use.
  *
  * @param _offset
  * Distance between blobs of data.
  **/
  virtual void
  setIndexBuffer(SPtr<IndexBuffer>& _pIndexB,
                 uint32 _format = 42,
                 uint32 _offset = 0) = 0;

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
  virtual SPtr<ConstantBuffer>
  createConstantBuffer(uint32 _size,
                       const void* _pData,
                       uint32 _usage) = 0;

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
  virtual void
  updateConstantBuffer(SPtr<ConstantBuffer> _pCBuffer,
                       const void* _pNewData,
                       uint32 _size) = 0;

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
  virtual void
  setShaderResourceView(SPtr<Texture> _pTexture,
                        uint32 _start = 0,
                        uint32 _numViews = 1) = 0;

  /**
   * @brief Set the render targets to the device.
   */
  // virtual void
  // setRenderTargets(Vector<SPtr<Texture>> _rTargets) = 0;

  /**
  * Clear the render target fiew and fill the
  * screen with a new color.
  * 
  * @param _color
  * New screen color.
  **/
  virtual void
  clearRenderTargetView(float _color[], SPtr<Texture> _rtv) = 0;

  /**
  * clear the depth buffer.
  **/
  virtual void
  clearDepthBuffer(float _depth, SPtr<DepthStencilView> _depthSV) = 0;

  /**
  * Create the Input Layout.
  **/
  virtual SPtr<InputLayout>
  createInputLayout(const Vector<InputDesc>& _vDesc, const SPtr<Shader> _pVShader) = 0;

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
  virtual void
  VSSetConstantBuffer(SPtr<ConstantBuffer> _pCBuffer,
                       uint32 _startSlot,
                       uint32 _numBuffers) = 0;

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
  virtual void
  PSSetConstantBuffer(SPtr<ConstantBuffer> _pCBuffer,
                       uint32 _startSlot,
                       uint32 _numBuffers) = 0;

  /**
  * Set the sampler state.
  **/
  virtual void
  setSampler(SPtr<SamplerState> _pSamLinear,
             uint32 _startSlot = 0,
             uint32 _numSamplers = 1) = 0;

  /**
  * create a texture from file.
  * 
  * @param _fileName
  * Name of the texture.
  * 
  * @param _bindFlags
  * What kind of binding will it have
  **/
  virtual SPtr<Texture>
  createTextureFromFile(String& _fileName,
                        uint32 _bindFlags,
                        bool _mipLevels,
                        uint32 _format) = 0;

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
  virtual void
  drawIndexed(uint32 _indexCount,
              uint32 _startIndexLocation,
              uint32 _baseVertexLocation) = 0;

  /**
  * Present the result to the screen.
  * 
  * @param _syncInterval
  * If vertical sync is enabled.
  * 
  * @param _flags
  * Swap chain presentation options.
  **/
  virtual void
  present(uint32 _syncInterval, uint32 _flags) = 0;

 public:
  // world matrix
  // Matrix4 world;
};

PK_CORE_EXPORT GraphicsAPI&
g_GraphicAPI();
}