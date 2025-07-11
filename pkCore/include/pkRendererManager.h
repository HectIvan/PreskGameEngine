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
#include "pkWindow.h"

namespace pkEngineSDK
{

namespace G_BUFFERS
{
  enum E {
    kGB_Albedo = 0,
    kGB_Normal,
    kGB_Shadow,
    kGB_Luminance,
    kGB_Metallic,
    kGB_HBlurredLuminance,
    kGB_VBlurredLuminance,
    kGB_Skybox,
  };
};

namespace D_BUFFERS
{
  enum E {
    kDB_Base = 0,
    kDB_Shadow,
  };
}

namespace UAV_BUFFERS
{
  enum E {
    kDB_Test = 0,
  };
}

namespace PASS_TYPE
{
  enum E {
    kP_Base = 0,
    kP_Shadow,
    kP_AO,
    kP_ShadowDef,
    kP_Luminance,
    kP_HBlur,
    kP_VBlur,
    kP_Tone,
    kP_CShadows, // compute shadows
    kP_SkyBox,
  };
}

class Model;

class PK_CORE_EXPORT RendererManager : public Module<RendererManager>
{
 public:
  RendererManager() = default;
  virtual ~RendererManager() = default;

  /**
   * @brief Initialize the render manager.
   */
  void
  init();

  /**
   * @brief Create the passes needed for the renderer.
   */
  void
  createPasses();

  /**
   * @brief Get a pass from the map.
   * @param _type Pass type to search for.
   * @return Pointer to the pass.
   */
  SPtr<Pass>
  getPass(PASS_TYPE::E _type);

  /**
   * @brief Get a specific G-buffer.
   * @param _type Type of buffer.
   * @return Pointer to the buffer.
   */
  SPtr<Texture>&
  getGBuffer(G_BUFFERS::E _type);

  /**
   * @brief Get all the buffers in the manager.
   * @return Vector of Textures.
   */
  Vector<SPtr<Texture>>
  getGBuffers();

  /**
   * @brief Get all buffers of the types inserted.
   * @param _types Types to search for.
   * @return Vector of G-Buffers.
   */
  Vector<SPtr<Texture>>
  getGBuffers(const G_BUFFERS::E _types);

  /**
   * @brief Get a specific Depth buffer.
   * @param _type Type of buffer.
   * @return Pointer to the buffer.
   */
  SPtr<Texture>&
  getDepthBuffer(const D_BUFFERS::E _type);

  /**
   * @brief Get a specific unordered view buffer.
   * @param _type Type of buffer.
   * @return Pointer to the buffer.
   */
  SPtr<Texture>&
  getUAVBuffer(const UAV_BUFFERS::E _type);

  /**
   * @brief Compile the shaders of all passes.
   */
  void
  compileShaders();

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
  UMap<G_BUFFERS::E, SPtr<Texture>> m_gBuffers;
  UMap<D_BUFFERS::E, SPtr<Texture>> m_depthBuffers;
  UMap<UAV_BUFFERS::E, SPtr<Texture>> m_uavBuffers;

  // passes
  UMap<PASS_TYPE::E, SPtr<Pass>> m_passes;

  // shadows
  SPtr<Texture> m_pShadowDepth;
  SPtr<DepthStencilView> m_pShadowDepthSV;
};

PK_CORE_EXPORT RendererManager&
g_RenderManager();
}
