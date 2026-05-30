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
#include "pkCBuffers.h"
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
    kGB_PositionsLight = 0, // positions from the light perspective
    kGB_Positions, // positions from the camera perspective
    kGB_Albedo, // object colors
    kGB_Normal, // object normals
    kGB_ORM, // oclussion roughness metallic
    kGB_SSAO, // screen space ambient occlusion
    kGB_BRDF, // light buffer
    kGB_BRDFTransp, // light transparency buffer
    kGB_Emissive, // base emissive buffer
    kGB_EmissiveHBlur, // emissive with horizontal blur
    kGB_EmissiveBlur, // emissive with full blur
    kGB_Skybox, // skybox buffer
    kGB_Luminance, // luminance buffer
    kGB_LumBlurH, // luminance horizontal blur
    kGB_LumBlur, // luminance full blur
    kGB_CubeMap, // luminance full blur
    kGB_IrradianceMap, // irradianceMap
    kGB_LUT, // Look-Up Table
    kGB_TranspAlbedo, // transparency albedo
    kGB_TranspNormal, // transparency normals
    kGB_TranspORM, // transparency orm
    kGB_TranspEmiss, // transparency orm
    kGB_TranspPos, // transparency positions
  };
};

namespace D_BUFFERS
{
  enum E {
    kDB_Base = 0,
    kDB_Light,
    kDB_Transparency,
  };
}

namespace UAV_BUFFERS
{
  enum E {
    kCB_Default = 0,
  };
}

namespace PASS_TYPE
{
  enum E {
    kP_Base = 0,
    kP_Transparency,
    kP_Material,
    kP_Merge,
    kP_EmissiveBlur,
    kP_EmissiveHBlur,
    kP_Exposure,
    kP_Luminance,
    kP_LumBlur,
    kP_LumBlurH,
    kP_LightPositions,
    kP_Light,
    kP_LightTransparency,
    kP_SkyBox,
    kP_SSAO,
    kP_Tone,
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
   * @brief on shut down of the module.
   */
  virtual void
  onShutDown() override;

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
  getPass(const PASS_TYPE::E _type);

  /**
   * @brief Get a specific G-buffer.
   * @param _type Type of buffer.
   * @return Pointer to the buffer.
   */
  SPtr<Texture>
  getGBuffer(const G_BUFFERS::E _type);

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
  SPtr<Texture>
  getDepthBuffer(const D_BUFFERS::E _type);

  /**
   * @brief Get a specific unordered view buffer.
   * @param _type Type of buffer.
   * @return Pointer to the buffer.
   */
  SPtr<Texture>
  getUAVBuffer(const UAV_BUFFERS::E _type);

  /**
   * @brief Generate a cubemap from a texture.
   * @param _pInput Texture to convert.
   * @param _pOutput Output cubemap texture.
   */
  void
  generateCubeMap(const SPtr<Texture>& _pInput, const SPtr<Texture>& _pOutput);

  /**
   * @brief Generate an irradiance cubemap from a cubemap texture.
   * @param _pInput Cubemap to convert.
   * @param _pOutput Output irradiance cubemap texture.
   */
  void
  generateIrradianceCubeMap(const SPtr<Texture>& _pInput, const SPtr<Texture>& _pOutput);

  /**
   * @brief Generate a LUT.
   * @param _pOutput Texture output.
   */
  void
  generateLUT(const SPtr<Texture>& _pOutput);

  /**
   * @brief Render a list of actors.
   * @param _gameActors The actors to render.
   */
  void
  renderActors(const Vector<SPtr<Actor>>& _gameActors);

  /**
   * @brief Render a given model
   * @param _model Pointer to the model to render
   * @param _actorTransform Transform matrix of the actor itself.
   */
  void
  renderModel(const SPtr<Model>& _model, const Matrix4& _actorTransform);

  /**
   * @brief Update a constant buffer.
   * @param _data Data to update the cBuffer with.
   * @param _pCBuffer Constant buffer to update.
   */
  template<class T> void
  updateBuffer(const T& _data, const SPtr<ConstantBuffer>& _pCBuffer);

 public:

  // render targets
  UMap<G_BUFFERS::E, SPtr<Texture>> m_gBuffers;
  UMap<D_BUFFERS::E, SPtr<Texture>> m_depthBuffers;
  UMap<UAV_BUFFERS::E, SPtr<Texture>> m_uavBuffers;

  // skyboxes
  SPtr<Texture> m_mainSkybox;

  SPtr<Texture> m_targetRT;
  SPtr<Texture> m_actorsRT;
  SPtr<Texture> m_transpActorsRT;

  // passes
  UMap<PASS_TYPE::E, SPtr<Pass>> m_passes;
  Vector<SPtr<Light>> m_lights;
  Vector<SPtr<Camera>> m_cameras;
};

PK_CORE_EXPORT RendererManager&
g_RenderManager();
}
