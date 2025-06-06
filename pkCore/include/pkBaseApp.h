/*************************************************************************/
/**
 * @file    pkBaseApp.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    07/12/2024
 * @brief   Base app for the Presk Game Engine
 *
 * @bug    no knon bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkCamera.h"
#include "pkEventQueue.h"
#include "pkLight.h"
#include "pkPrerequisitesCore.h"
#include "pkRendererManager.h"
#include "pkResourceManager.h"
#include "pkTimeManager.h"
#include "pkWindow.h"

namespace pkEngineSDK
{

/**
 * Memory structures used to store already loaded data and call it when needed.
 */
struct ModelMemory
{
  String name;
  SPtr<Model> model;
};

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
  init(const char** _argv, int32 _count);

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
   * @param _argv
   * @param _count Ammount of parameters.
   */
  void
  initAPI(const char** _argv, int32 _count);

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
  onUpdate() {}

  /**
   * @brief Fixed update of the engine
   */
  virtual void
  fixedUpdate() {}

  /**
   * @brief Base app render function.
   */
  void
  render();

  /**
   * @brief Render function for any child class of this base class
   */
  virtual void
  onRender() {}

  /**
   * @brief Create a material from a texture.
   * @return The material pointer.
   */
  SPtr<Material>
  createMaterial();

  /**
   * @brief Create a new model component.
   * @param _modelName Name of the model file to load.
   * @return Pointer to the new model object.
   */
  SPtr<Model>
  newModel(String _modelName = "");

 public:
  // system
  Window m_window;
  EventQueue m_eventQueue;

  // camera movement speed
  float m_cameraSpeed;

  // fixed update
  float m_deltaTime = 0.0f;
  float m_fixedTimer = 0.0f;
  float m_fixedDeltaTime = 0.016f;

 protected:
  /**
   * Used to store an object that is loaded from memory. once loaded, it will be stored
   * in here, if the same object is asked to be loaded, it will first be searched here,if
   * its found, the loaded object will be returned, instead of loading it again.
   */

   // to do: transfer this to a resource manager
  Vector<SPtr<ModelMemory>> m_models;
  bool m_vSync;
};
}