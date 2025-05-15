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
   * @brief Set the Vertex Shader constant buffers
   */
  void
  VSSetConstantBuffers(Vector<SPtr<ConstantBuffer>> _cBuffers);

  /**
   * @brief Set the Pixel Shader constant buffers
   */
  void
  PSSetConstantBuffers(Vector<SPtr<ConstantBuffer>> _cBuffers);

  /**
   * @brief Set the buffers of each actor.
   */
  void
  setActorsBuffers();

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
};

PK_CORE_EXPORT RendererManager&
g_RenderManager();
}