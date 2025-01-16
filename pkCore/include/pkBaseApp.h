/************************************************************************/
/**
* @pkBaseApp pkBaseApp.h
* @Hector Ivan Muñoz Ceballos
* @date 07/12/2024
* @Base app for the Presk Game Engine.
*
* This file contains the Base App of the engine
*
* @bug. Game object will have only one material, even when multiple exist.
* 
* @HectIvan 03/01/2025
* correct mouse position update.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include <chrono>

#include "pkCamera.h"
#include "pkConstantBuffer.h"
#include "pkEventQueue.h"
#include "pkGameObject.h"
#include "pkLight.h"
#include "pkPrerequisitesCore.h"
#include "pkWindow.h"

namespace pkEngineSDK
{

class GraphicsAPI;

using std::chrono::high_resolution_clock;
using std::chrono::duration;

class PK_CORE_EXPORT BaseApp
{
 public:
  BaseApp() = default;
  virtual ~BaseApp() = default;

  /**
  * Initialize the app.
  **/
  void
  init(const char** _argv);

  /**
  * On initializing the app.
  **/
  virtual void
  onInit() {}

  /**
  * Initialize the app window.
  **/
  void
  initWindow();

  /**
  * Initialize the api.
  **/
  void
  initAPI(const char** _argv);

  /**
  * Event message loop.
  * 
  * @param _api
  * API to use.
  **/
  void
  messageLoop();

  /**
  * update function for any child class of this base app
  **/
  virtual void
  onUpdate(float _deltaTime) {}

  /**
  * Render the scene.
  **/
  void
  render();

  /**
  * render function for any child class of this base class
  **/
  virtual void
  onRender() {}

  /**
  * Insert a new game object into the scene or as
  * a child of another gameObject.
  * 
  * @param _modelName
  * Name of the model file to search for.
  * 
  * @param _textureName
  * Name of the texture file to search for.
  * 
  * @param _tranform
  * Transform of the game object.
  * 
  * @param _pParent
  * If the gameobject will be the child of a game object.
  **/
  void
  newGameObject(Matrix4 _transform = Matrix4::IDENTITY,
                SPtr<GameObject> _pParent = nullptr);

  /**
  * Create a new model component.
  * 
  * @param _modelName
  * Name of the model file to load.
  * 
  * @return
  * Pointer to the new model object.
  **/
  SPtr<Model>
  newModel(String _modelName = "");

  /**
  * Create a new Material component.
  * 
  * @param _textureName
  * Nameof the texture file to load.
  * 
  * @return
  * Pointer to the new material object.
  **/
  SPtr<Material>
  newMaterial(String _textureName = "");

 private:
  /**
  * Update the camera.
  * 
  * @param _pCamera
  * Camera to update.
  **/
  void
  updateCamera(Camera* _pCamera);

  /**
  * Loads a model from a file.
  * 
  * @param _path
  * File path.
  **/
  SPtr<Model>
  loadModel(String& _fileName);

  /**
  * Create the constant buffers needed.
  **/
  void
  createBuffers();

  /**
  * Set the buffers of each game object.
  **/
  void
  setGameObjectsBuffers();

  /**
  * Set the Vertex Shader constant buffers
  **/
  void
  VSSetConstantBuffers();

  /**
  * Set the Pixel Shader constant buffers
  **/
  void
  PSSetConstantBuffers();
  /**
  * Render the game objects in scene.
  **/
  void
  renderGameObjects(Vector<SPtr<GameObject>> _gameObjects);

 public:
  // system
  Window window;
  Camera camera;
  EventQueue eventQueue;

  // vector of game objects in the scene
  Vector<SPtr<GameObject>> gameObjects;

  // light source
  Light light;

  // constant buffers
  SPtr<ConstantBuffer> buffer;
  SPtr<ConstantBuffer> cBView;
  SPtr<ConstantBuffer> cBProjection;
  SPtr<ConstantBuffer> cBWorld;
  SPtr<ConstantBuffer> cbLight;

  // camera movement speed
  float cameraSpeed;
};
}