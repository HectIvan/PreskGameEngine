/************************************************************************/
/**
* @pkRendererManager pkRendererManager.h
* @Hector Ivan Muñoz Ceballos
* @date 04/02/2025
* @Render Manager file for the Presk Game Engine.
*
* This file will contain the Render Manager used for the engine
*
* @bug No current function.
*
* @HectIvan 04/02/20245
* File Creation.
*/
/************************************************************************/
#pragma once
/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkActor.h"
#include "pkConstantBuffer.h"
#include "pkLight.h"
#include "pkModule.h"
#include "pkPrerequisitesCore.h"
#include "pkScene.h"

namespace pkEngineSDK
{

class Model;

class PK_CORE_EXPORT RendererManager : public Module<RendererManager>
{
 public:
  RendererManager() = default;
  virtual ~RendererManager() = default;

  /**
   * @brief Update the camera.
   * @param _pCamera Camera to update.
   */
  void
  updateCamera(Camera* _pCamera);

  /**
   * @brief Converts the actor to a game object pointer.
   * @param _subject Actor to convert.
   * @return Pointer to the game object
   */
  template<typename T>
  SPtr<T>
  actorToClass(SPtr<Actor>& _subject);

  /**
   * @brief Set the Vertex Shader constant buffers
   */
  void
  VSSetConstantBuffers();

  /**
   * @brief Set the Pixel Shader constant buffers
   */
  void
  PSSetConstantBuffers();

  /**
   * @brief Set the buffers of each game object.
   */
  void
  setActorsBuffers(Scene& _scene);

  /**
   * @brief Render a list of actors.
   * @param _gameActors The actors to render.
   */
  void
  renderActors(Vector<SPtr<Actor>> _gameActors);

  /**
   * @brief Render a given model
   * @param _model The model to render
   */
  void
  renderModel(Model& _model);

 public:
  // light source
  Light light;

  // constant buffers
  SPtr<ConstantBuffer> m_buffer;
  SPtr<ConstantBuffer> m_cBView;
  SPtr<ConstantBuffer> m_cBProjection;
  SPtr<ConstantBuffer> m_cBWorld;
  SPtr<ConstantBuffer> m_cbLight;

  //
};

PK_CORE_EXPORT RendererManager&
g_RenderManager();
}