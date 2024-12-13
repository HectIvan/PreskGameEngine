/************************************************************************/
/**
* @pkGraphicsAPI pkGraphicsAPI.h
* @Hector Ivan Muñoz Ceballos
* @date 19/11/2024
* @Graphics api file for the Presk Game Engine.
*
* Graphics API class for the engine
*
* @bug No bug known.
*
* @HectIvan 09/12/2024
* changed the render function.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkModel.h"
#include "pkModule.h"
#include "pkPrerequisitesCore.h"
#include "pkWindow.h"
namespace pkEngineSDK
{

class Camera;
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
  * Create a VertexBuffer
  **/
  virtual SPtr<VertexBuffer>
  createVertexBuffer(const Vector<SimpleVertex>& _vertex,
                     uint32 _usage = 0) = 0;

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
  * Update the camera buffers.
  * 
  * @param _pCamera
  * Camera to update.
  **/
  virtual void
  updateCamera(Camera* _pCamera) = 0;

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
  * Clear the render target fiew and fill the
  * screen with a new color.
  * 
  * @param _color
  * New screen color.
  **/
  virtual void
  clearRenderTargetView(float _color[]) = 0;

  /**
  * clear the depth buffer.
  **/
  virtual void
  clearDepthBuffer(float _depth) = 0;

  /**
  * Update the light and world constant buffers.
  **/
  virtual void
  updateWorldAndLightCB() = 0;
  
  /**
  * Set the shaders of the api.
  **/
  virtual void
  setShaders() = 0;

  /**
  * Set the Vertex Shader constant buffers.
  **/
  virtual void
  VSSetConstantBuffers() = 0;

  /**
  * Set the Pixel Shader constant buffers.
  **/
  virtual void
  PSSetConstantBuffers() = 0;

  /**
  * Set the sampler state.
  **/
  virtual void
  setSampler() = 0;

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
};
}