/*****************************************************************************/
/**
 * @file    pkRendererManager.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    04/02/2025
 * @brief   This file will contain the Render Manager used for the engine
 *
 * @bug    No known bugs.
 * 
 * @changes Changed the management of render targets to use maps.
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
   * @brief Get a pass from the map.
   * @param _index Pass to search for.
   * @return Pointer to the pass.
   */
  SPtr<Pass>
  getPass(uint32 _index);

  /**
   * @brief Get a specific G-buffer.
   * @param _name Name of the buffer.
   * @return Pointer to the buffer.
   */
  SPtr<Texture>
  getGBuffer(String _name);

  /**
   * @brief Get a specific Depth buffer.
   * @param _name Name of the buffer.
   * @return Pointer to the buffer.
   */
  SPtr<Texture>
  getDepthBuffer(String _name);

  /**
   * @brief Compile the shaders of all passes.
   */
  void
  compileShaders();

  /**
   * @brief Set the Vertex Shader constant buffers
   */
  void
  VSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>> _cBuffers);

  /**
   * @brief Set the Pixel Shader constant buffers
   */
  void
  PSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>> _cBuffers);

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
  renderActors(const Vector<SPtr<Actor>> _gameActors);

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
  Map<String, SPtr<Texture>> m_gBuffers;
  Map<String, SPtr<Texture>> m_depthBuffers;
  // SPtr<Texture> m_pRTargetView;
  // SPtr<Texture> m_pNormalRT;
  // SPtr<Texture> m_pDepthRT;

  // depth stencil
  // SPtr<DepthStencilView> m_pDepthSView;

  // passes
  Map<uint32, SPtr<Pass>> m_passes;

  // shadows
  SPtr<Texture> m_pShadowDepth;
  SPtr<DepthStencilView> m_pShadowDepthSV;
};

PK_CORE_EXPORT RendererManager&
g_RenderManager();
}