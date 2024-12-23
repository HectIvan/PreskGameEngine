/************************************************************************/
/**
* @pkBaseApp pkBaseApp.h
* @Hector Ivan Muñoz Ceballos
* @date 07/12/2024
* @Base app for the Presk Game Engine.
*
* This file contains the Base App of the engine
*
* @bug.
* Its currently impossible to add multiple game objects without them showing issues.
* 
* @HectIvan 21/12/2024
* Added a way of adding both materials and models as components.
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
  newGameObject(String _modelName,
                String _textureName,
                Transform _transform,
                SPtr<GameObject> _pParent = nullptr);

  /**
  * Initialize the app.
  **/
  void
  init(const char** _argv);

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
  * Create the constant buffers needed.
  **/
  void
  createBuffers();

  /**
  * Event message loop.
  * 
  * @param _api
  * API to use.
  **/
  void
  messageLoop();

  /**
  * Loads a model from a file.
  * 
  * @param _path
  * File path.
  **/
  SPtr<Model>
  loadModel(String& _fileName);

  /**
  * Update the camera.
  * 
  * @param _pCamera
  * Camera to update.
  **/
  void
  updateCamera(Camera* _pCamera);

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
  * Gets the time elapsed between frames.
  * 
  * @return
  * The deltaTime.
  **/
  float
  getDeltaTime(high_resolution_clock::time_point& _delta);

  /**
  * Render the scene.
  **/
  void
  render();

  /**
  * Render the game objects in scene.
  **/
  void
  renderGameObjects(Vector<SPtr<GameObject>> _gameObjects);

  /**
  * Render the desired model.
  * 
  * @param _model
  * The model to render
  **/
  void
  renderModel(Model& _model);

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

  // rotation angles
  float rotX = 0;
  float rotY = 0;
  float rotZ = 0;
};
}