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
* @HectIvan 2/12/2024
* Added a temporary render function.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkGameObject.h"
#include "pkModel.h"
#include "pkModule.h"
#include "pkPrerequisitesCore.h"
#include "pkWindow.h"
namespace pkEngineSDK
{

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
  * Insert a new game object to the game objects vector.
  * 
  * @param _gameObject
  * Pointer to the new gameObject.
  **/
  void
  insertGameObject(GameObject* _gameObject) { gameObjects.push_back(_gameObject); }

 public:
  Vector<GameObject*> gameObjects;
};
}