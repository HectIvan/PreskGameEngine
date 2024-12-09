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
#include "pkCamera.h"
#include "pkGameObject.h"
#include "pkModel.h"
#include "pkModule.h"
#include "pkPrerequisitesCore.h"
#include "pkWindow.h"
namespace pkEngineSDK
{

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
  * Render the graphic api result.
  **/
  virtual void
  render() = 0;

  /**
  * Load a model.
  * 
  * @param _path
  * Path of the mesh that the model will have.
  * 
  * @return
  * Pointer to the new model.
  **/
  virtual Model*
  loadModel(String& _path) = 0;

  /**
  * Insert a new game object to the game objects vector.
  * 
  * @param _pGameObject
  * Pointer to the new gameObject.
  **/
  void
  insertGameObject(GameObject* _pGameObject) { gameObjects.push_back(_pGameObject); }

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

  virtual void
  updateCamera(Camera* _pCamera) = 0;

 public:
  Vector<GameObject*> gameObjects;
  Camera m_camera;
};
}