/*****************************************************************************/
/**
 * @file    pkRendererManager.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    04/02/2025
 * @brief   This file will contain the Render Manager used for the engine
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
#include "pkActor.h"
#include "pkCamera.h"
#include "pkConstantBuffer.h"
#include "pkDepthStencilView.h"
#include "pkLight.h"
#include "pkModule.h"
#include "pkPass.h"
#include "pkPrerequisitesCore.h"
#include "pkScene.h"
#include "pkTexture.h"
#include "pkWindow.h"

namespace pkEngineSDK
{

class Model;

class PK_CORE_EXPORT RendererManager : public Module<RendererManager>
{
 public:
  RendererManager() = default;
  virtual ~RendererManager() = default;

  /**
   * @brief Initialize the render manager
   */
  void
  init(Window& _window);

  /**
   * @brief Create the passes needed for the renderer.
   */
  void
  createPasses();

  /**
   * @brief Compile the shaders of all passes.
   */
  void
  compileShaders();

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
  setActorsBuffers();

  /**
   * @brief Render a scene
   */
  void
  render();

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

  /**
   * @brief Update a constant buffer.
   * @param _data Data to update the cBuffer with.
   * @param _pCBuffer Constant buffer to update.
   */
  template<class T> void
  updateBuffer(T& _data, SPtr<ConstantBuffer> _pCBuffer);

 public:

  // constant buffers
  SPtr<ConstantBuffer> m_cBView;
  SPtr<ConstantBuffer> m_cBProjection;
  SPtr<ConstantBuffer> m_cBTransform;
  SPtr<ConstantBuffer> m_cbLight;
  SPtr<ConstantBuffer> m_cbCamera;

  // render targets
  SPtr<Texture> m_pRTargetView;
  SPtr<Texture> m_pNormalRT;
  SPtr<Texture> m_pDepthRT;

  // depth stencil
  SPtr<DepthStencilView> m_pDepthSView;

  // passes
  Map<uint32, SPtr<Pass>> m_passes;

  // shadows
  SPtr<Texture> m_pShadowDepth;
  SPtr<DepthStencilView> m_pShadowDepthSV;

  // main camera storage
  Matrix4 m_CView;
  Matrix4 m_CProj;
};

PK_CORE_EXPORT RendererManager&
g_RenderManager();
}