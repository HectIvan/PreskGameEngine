/************************************************************************/
/**
* @pkBaseApp pkBaseApp.h
* @Hector Ivan Muñoz Ceballos
* @date 07/12/2024
* @Base app for the Presk Game Engine.
*
* This file contains the Base App of the engine
*
* @bug. Children will not follow their parent transform correctly.
* 
* @HectIvan 27/01/2025
* @brief Changed the children transform inheritance.
* @brief Added a new function that allows to convert any actor into any class,
* the value returned will depend on wether the cast was successful or not.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkCamera.h"
#include "pkEventQueue.h"
#include "pkGameObject.h"
#include "pkLight.h"
#include "pkPrerequisitesCore.h"
#include "pkRendererManager.h"
#include "pkResourceManager.h"
#include "pkTimeManager.h"
#include "pkWindow.h"

namespace pkEngineSDK
{

class Camera;

class PK_CORE_EXPORT BaseApp
{
 public:
  BaseApp() = default;
  virtual ~BaseApp() = default;

  /**
   * @brief Initialize the app.
   */
  void
  init(const char** _argv);

  /**
   * @brief On initializing the app.
   */
  virtual void
  onInit() {}

  /**
   * @brief Initialize the app window.
   */
  void
  initWindow();

  /**
   * @brief Initialize the api.
   */
  void
  initAPI(const char** _argv);

  /**
   * @brief Event message loop.
   * @param _api API to use.
   */
  void
  messageLoop();

  /**
   * @brief Update function for any child class of this base app
   */
  virtual void
  onUpdate(float _deltaTime) {}

  /**
   * @brief Fixed update of the engine
   */
  virtual void
  fixedUpdate() {}

  /**
   * @brief Render function for any child class of this base class
   */
  virtual void
  onRender() {}

  /**
   * @brief Create a material from a texture.
   * @param _path Path of the texture.
   */
  SPtr<Material>
  createMaterial(String _path);

  /**
   * @brief Create a new model component.
   * @param _modelName Name of the model file to load.
   * @return Pointer to the new model object.
   */
  SPtr<Model>
  newModel(String _modelName = "");

  /**
   * @brief Find an actor by name.
   * @param _actorName Name of the actor.
   * @return Pointer to the actor.
   */
  SPtr<Actor>
  actorFind(String _actorName);

  /**
   * @brief Get a game object with a specific component.
   * @return A pointer to the game object.
   */
  template<typename T>
  SPtr<Actor>
  getActorWithComponent();

  /**
   * @brief Get a vector with all game objects with a specific component.
   * @return A vector of game objects.
   */
  template<typename T>
  Vector<SPtr<Actor>>
  getAllActorsWithComponent();

 private:

  /**
   * @brief Create the constant buffers needed.
   */
  void
  createBuffers();

 public:
  // system
  Window m_window;
  Camera m_camera;
  EventQueue m_eventQueue;

  // vector of game objects in the scene
  Scene m_scene;

  // camera movement speed
  float m_cameraSpeed;

  // fixed update
  float m_deltaTime = 0.0f;
  float m_fixedTimer = 0.0f;
  float m_fixedDeltaTime = 0.016f;
};
}