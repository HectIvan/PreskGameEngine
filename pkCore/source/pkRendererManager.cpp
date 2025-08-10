#include "pkCamera.h"
#include "pkGraphicsAPI.h"
#include "pkLogger.h"
#include "pkRendererManager.h"
#include "pkSamplerState.h"
#include "pkSceneManager.h"
#include "pkTextureManager.h"

namespace pkEngineSDK
{

void RendererManager::init()
{
  GraphicsAPI& api = g_GraphicAPI().instance();
  TextureManager& tm = g_TextureManager().instance();

  uint32 winHeight = api.getSwapChain()->getHeight();
  uint32 winWidth = api.getSwapChain()->getWidth();

  // Texture description
  TextureDesc txDesc;
  txDesc.bpp = 4;
  txDesc.width = winWidth;
  txDesc.height = winHeight;
  txDesc.format = kPK_FORMAT_R32G32B32A32_FLOAT;
  txDesc.bindFlags = kPK_BIND_SHADER_RESOURCE | kPK_BIND_RENDER_TARGET;
  txDesc.usage = kPK_USAGE_DEFAULT;
  txDesc.mipLevels = false;
  txDesc.miscFlags = 0;
  txDesc.shaderResourceFormat = kPK_FORMAT_R32G32B32A32_FLOAT;
  
  // render target for scene colors
  SPtr<Texture> albedoRTV = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Albedo, albedoRTV });

  // create the normal render target that will store the normals of the world
  SPtr<Texture> normalRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Normal, normalRT });

  // render target for the shadow result
  SPtr<Texture> shadowRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Shadow, shadowRT });

  // render target for the shadow result
  SPtr<Texture> specularRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Specular, specularRT });

  // render target for the metallic result
  SPtr<Texture> metallicRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Metallic, metallicRT });

  // render target for the roughness result
  SPtr<Texture> roughnessRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Roughness, roughnessRT });

  // skybox texture
  SPtr<Texture> skyboxRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Skybox, skyboxRT });

  // positions texture
  SPtr<Texture> posRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Positions, posRT });

  // positions texture for the light
  SPtr<Texture> posLightRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_PositionsLight, posLightRT });

  // emissive texture
  SPtr<Texture> emissiveRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Emissive, emissiveRT });

  SPtr<Texture> emissiveHBlurRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_EmissiveHBlur, emissiveHBlurRT });

  SPtr<Texture> emissiveBlurRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_EmissiveBlur, emissiveBlurRT });

  // IBR texture
  SPtr<Texture> ibrRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_IBR, ibrRT });

  // merge texture
  SPtr<Texture> mergeRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Merge, mergeRT });

  // luminance texture
  SPtr<Texture> lumRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Luminance, lumRT });

  SPtr<Texture> lumBlurHRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_LumBlurH, lumBlurHRT });

  SPtr<Texture> lumBlurRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_LumBlur, lumBlurRT });

  m_mainSkybox = tm.loadTexture(Path("textures/skybox/Skybox_papermill.hdr"));

  // ---------------------------------------------------------- //
  // DEPTH TARGETS
  // ---------------------------------------------------------- //
  // depth buffer description
  txDesc.format = PK_TEXTURE_FORMAT::kPK_FORMAT_R32_TYPELESS;
  txDesc.bindFlags = kPK_BIND_SHADER_RESOURCE | kPK_BIND_DEPTH_STENCIL;
  txDesc.shaderResourceFormat = PK_TEXTURE_FORMAT::kPK_FORMAT_R32_FLOAT;
  // camera depth buffer
  SPtr<Texture> depthBuffer = api.createTexture(txDesc);
  m_depthBuffers.insert({ D_BUFFERS::kDB_Base, depthBuffer });

  // light depth buffer
  SPtr<Texture> shadowDepth = api.createTexture(txDesc);
  m_depthBuffers.insert({ D_BUFFERS::kDB_Light, shadowDepth });

  // ---------------------------------------------------------- //
  // UNORDERED ACCESS VIEWS
  // ---------------------------------------------------------- //
  // shadows
  txDesc.bindFlags = kPK_BIND_UNORDERED_ACCESS | kPK_BIND_SHADER_RESOURCE;
  txDesc.format = PK_TEXTURE_FORMAT::kPK_FORMAT_R8G8B8A8_UNORM;
  txDesc.shaderResourceFormat = PK_TEXTURE_FORMAT::kPK_FORMAT_R8G8B8A8_UNORM;
  
  // create the passes needed
  createPasses();
}

void
RendererManager::createPasses()
{
  /****************************************************************************
   * Create the base pass.
   ***************************************************************************/
   // pass description
  PassDesc pDesc = PassDesc();
  pDesc.vSDirectory = Path("shaders/pkVShader.hlsl");
  pDesc.pSDirectory = Path("shaders/pkPShader.hlsl");
  pDesc.vSEntry = "VS";
  pDesc.pSEntry = "PS";
  pDesc.vSModel = "vs_5_0";
  pDesc.pSModel = "ps_5_0";
  pDesc.samAdress = PK_SAM_STATE_ADRESS::kWrap;
  pDesc.samFilters = PK_SAM_STATE_FILTERS::kFilterMigMagMipLinear;
  pDesc.cBSizes = { sizeof(CBView), sizeof(CBProjection), sizeof(CBTransform), sizeof(CBLight),
                    sizeof(CBCamera), sizeof(CBTransform) };
  pDesc.inputs = {};
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_Albedo),
                    getGBuffer(G_BUFFERS::kGB_Normal),
                    getGBuffer(G_BUFFERS::kGB_Metallic),
                    getGBuffer(G_BUFFERS::kGB_Roughness),
                    getGBuffer(G_BUFFERS::kGB_Emissive),
                    getGBuffer(G_BUFFERS::kGB_Positions) };
  pDesc.pDepth = getDepthBuffer(D_BUFFERS::kDB_Base);
  // rasterizer state
  pDesc.rSExists = true;
  pDesc.rSCullMode = RS_CULL_MODE::kPK_CULL_NONE;
  pDesc.rSFillMode = RS_FILL_MODE::kPK_FILL_SOLID;
  pDesc.rSFrontCounterClockwise = false;
  pDesc.rSDepthClipEnable = true;
  // make the pass
  SPtr<Pass> basePass = make_shared<Pass>(pDesc);
  // insert to the pass map.
  m_passes.insert({ PASS_TYPE::kP_Base, basePass });

  /****************************************************************************
   * Shadow Pass
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkPShaderDepth.hlsl");
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_PositionsLight) };
  pDesc.pDepth = getDepthBuffer(D_BUFFERS::kDB_Light);
  SPtr<Pass> shadowPass = make_shared<Pass>(pDesc);
  m_passes.insert({ PASS_TYPE::kP_Shadow, shadowPass });

  /****************************************************************************
   * Shadow Specular Quad Pass
   ***************************************************************************/
  pDesc.vSDirectory = Path("shaders/pkQuadShader.hlsl");
  pDesc.pSDirectory = Path("shaders/pkShadowMapping.hlsl");
  pDesc.cBSizes = { sizeof(CBLight), sizeof(CBCamera), sizeof(CBCamera), sizeof(Matrix4), 
                    sizeof(Vector4) };
  pDesc.inputs = { getDepthBuffer(D_BUFFERS::kDB_Light),
                   getDepthBuffer(D_BUFFERS::kDB_Base),
                   getGBuffer(G_BUFFERS::kGB_Normal),
                   getGBuffer(G_BUFFERS::kGB_Albedo),
                   getGBuffer(G_BUFFERS::kGB_Positions),
                   getGBuffer(G_BUFFERS::kGB_Metallic),
                   getGBuffer(G_BUFFERS::kGB_Roughness),
                   getGBuffer(G_BUFFERS::kGB_PositionsLight) };
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_Shadow), getGBuffer(G_BUFFERS::kGB_Specular) };
  SPtr<Pass> shadowQuadPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_ShadowQuad, shadowQuadPass });

  /****************************************************************************
   * Skybox Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkSkyboxShader.hlsl");
  pDesc.cBSizes = { sizeof(Matrix4), sizeof(Matrix4) };
  pDesc.inputs = { m_mainSkybox };
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_Skybox) };
  SPtr<Pass> skyboxPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_SkyBox, skyboxPass });

  /****************************************************************************
   * IBR Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkIBRShader.hlsl");
  pDesc.cBSizes = { sizeof(Vector4), sizeof(Vector4) };
  pDesc.inputs = { m_mainSkybox, getGBuffer(G_BUFFERS::kGB_Normal),
                   getGBuffer(G_BUFFERS::kGB_Positions),
                   getGBuffer(G_BUFFERS::kGB_Metallic),
                   getGBuffer(G_BUFFERS::kGB_Roughness),
                   getGBuffer(G_BUFFERS::kGB_Albedo) };
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_IBR) };
  SPtr<Pass> ibrPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_IBR, ibrPass });

  /****************************************************************************
   * Emissive Horizontal Blur Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkHBlur.hlsl");
  pDesc.cBSizes = { sizeof(CBBlur) };
  pDesc.inputs = { getGBuffer(G_BUFFERS::kGB_Emissive) };
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_EmissiveHBlur) };
  SPtr<Pass> emissiveHPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_EmissiveHBlur, emissiveHPass });

  /****************************************************************************
   * Emissive Blur Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkHBlur.hlsl");
  pDesc.cBSizes = { sizeof(CBBlur) };
  pDesc.inputs = { getGBuffer(G_BUFFERS::kGB_EmissiveHBlur) };
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_EmissiveBlur) };
  SPtr<Pass> emissivePass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_EmissiveBlur, emissivePass });

  /****************************************************************************
   * Merge Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkMergeShader.hlsl");
  pDesc.cBSizes = {};
  pDesc.inputs = { getGBuffer(G_BUFFERS::kGB_Albedo),
                   getGBuffer(G_BUFFERS::kGB_Specular),
                   getGBuffer(G_BUFFERS::kGB_Shadow),
                   getGBuffer(G_BUFFERS::kGB_Skybox),
                   getGBuffer(G_BUFFERS::kGB_IBR),
                   getGBuffer(G_BUFFERS::kGB_Emissive),
                   getGBuffer(G_BUFFERS::kGB_EmissiveBlur),
                   getGBuffer(G_BUFFERS::kGB_Metallic) };
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_Merge) };
  SPtr<Pass> mergePass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_Merge, mergePass });

  /****************************************************************************
   * POST_PROCESSING
   ***************************************************************************/

  /****************************************************************************
   * Luminance
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkLuminanceQuad.hlsl");
  pDesc.cBSizes = { sizeof(CBVector2x2) };
  pDesc.inputs = { getGBuffer(G_BUFFERS::kGB_Merge) };
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_Luminance) };
  SPtr<Pass> lumPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_Luminance, lumPass });

  /****************************************************************************
   * Luminance Horizontal Blur
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkHBlur.hlsl");
  pDesc.cBSizes = { sizeof(CBBlur) };
  pDesc.inputs = { getGBuffer(G_BUFFERS::kGB_Luminance) };
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_LumBlurH) };
  SPtr<Pass> lumBlurHPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_LumBlurH, lumBlurHPass });

  /****************************************************************************
   * Luminance Vertical Blur
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkHBlur.hlsl");
  pDesc.cBSizes = { sizeof(CBBlur) };
  pDesc.inputs = { getGBuffer(G_BUFFERS::kGB_LumBlurH) };
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_LumBlur) };
  SPtr<Pass> lumBlurPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_LumBlur, lumBlurPass });

  /****************************************************************************
   * Tone mapping Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkToneMap.hlsl");
  pDesc.cBSizes = {  };
  pDesc.inputs = { getGBuffer(G_BUFFERS::kGB_Merge),
                   getGBuffer(G_BUFFERS::kGB_LumBlur) };
  pDesc.outputs = { g_GraphicAPI().getSwapChain()->getBuffer(0) };
  SPtr<Pass> TonePass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_Tone, TonePass });
}

SPtr<Pass>
RendererManager::getPass(const PASS_TYPE::E _type)
{
  return m_passes.find(_type)->second;
}

SPtr<Texture>
RendererManager::getGBuffer(const G_BUFFERS::E _type)
{
  return m_gBuffers.find(_type)->second;
}

Vector<SPtr<Texture>>
RendererManager::getGBuffers()
{
  Vector<SPtr<Texture>> textures(m_gBuffers.size());

  int32 i = 0;
  for (auto [textureName, sptrTexture] : m_gBuffers) {
    textures[i++] = sptrTexture;
  }
  return textures;
}

Vector<SPtr<Texture>>
RendererManager::getGBuffers(const G_BUFFERS::E _types)
{
  Vector<SPtr<Texture>> textures;

  for (uint32 i = 0; i < m_gBuffers.size(); ++i) {
    if ((_types & G_BUFFERS::kGB_Albedo) == G_BUFFERS::kGB_Albedo) {
      textures.push_back(getGBuffer(G_BUFFERS::kGB_Albedo));
    }
    if ((_types & G_BUFFERS::kGB_Normal) == G_BUFFERS::kGB_Normal) {
      textures.push_back(getGBuffer(G_BUFFERS::kGB_Normal));
    }
  }
  return textures;
}

SPtr<Texture>
RendererManager::getDepthBuffer(const D_BUFFERS::E _type)
{
  return m_depthBuffers.find(_type)->second;
}

SPtr<Texture>
RendererManager::getUAVBuffer(const UAV_BUFFERS::E _type)
{
  return m_uavBuffers.find(_type)->second;
}

void
RendererManager::compileShaders()
{
  for (auto it = m_passes.begin(); it != m_passes.end(); ++it) {
    // Compile shaders
    it->second->compileShaders();
    g_Logger().print("recompiled shaders.");
  }
}

template<class T> void
RendererManager::updateBuffer(const T& _data, const SPtr<ConstantBuffer>& _pCBuffer)
{
  g_GraphicAPI().updateConstantBuffer(_pCBuffer, &_data, 0);
}

void
RendererManager::setActorsBuffers()
{
  // get managers
  GraphicsAPI& api = g_GraphicAPI().instance();
  SceneManager& sm = g_SceneManager().instance();
  // for each actor in the world
  SPtr<Scene> activeScene = sm.getActiveScene();
  SIZE_T actorCount = activeScene->getAllActors().size();
  for (uint32 i = 0; i < actorCount; ++i) {
    // Cast to a gameObject, if it fails, do none of the following process
    SPtr<Actor> actor = activeScene->getActor(i);
    // if the actor has a model 
    SPtr<Model> model = actor->getComponent<Model>();
    if (model) {
      api.setVertexBuffer(model->m_vertexB);
      api.setIndexBuffer(model->m_indexB);
    }
  }
}

void
RendererManager::renderActors(const Vector<SPtr<Actor>> _gameActors)
{
  RendererManager& rManager = g_RenderManager().instance();
  GraphicsAPI& api = g_GraphicAPI().instance();
  // for each actor
  for (uint32 i = 0; i < _gameActors.size(); ++i) {
    // if actor is not active
    if (!_gameActors[i]->isActive()) {
      continue;
    }
    // Get the final matrix by taking into account the parent actors
    SPtr<Actor> parent = _gameActors[i]->m_parent;
    Matrix4 transform = _gameActors[i]->m_transform;
    // while there's a parent
    while (parent) {
      // add the parent transform to the current transform matrix
      transform *= parent->m_transform;
      // the next parent will be the parent of this parent
      parent = parent->m_parent;
    }
    // set the current actor transform as the world in which the shader will work 
    SPtr<Pass> basePass = rManager.getPass(PASS_TYPE::kP_Base);
    SPtr<Pass> shadowPass = rManager.getPass(PASS_TYPE::kP_Shadow);
    api.updateConstantBuffer(basePass->getCBuffer(2), &transform, sizeof(Matrix4));
    api.updateConstantBuffer(shadowPass->getCBuffer(2), &transform, sizeof(Matrix4));
    Matrix4 transformInvTransp = transform.inverse().getTransposed();
    api.updateConstantBuffer(basePass->getCBuffer(5), &transformInvTransp, sizeof(Matrix4));

    // render the model of the actor
    SPtr<Model> model = _gameActors[i]->getComponent<Model>();
    if (model && model->isActive()) {
      renderModel(model);
    }
    // if the actor has children, do the same for them (recursive)
    if (!_gameActors[i]->m_children.empty()) {
      renderActors(_gameActors[i]->m_children);
    }
  }
}


void
RendererManager::renderModel(const SPtr<Model>& _model)
{
  GraphicsAPI& api = g_GraphicAPI().instance();
  // get a reference from the api
  api.setVertexBuffer(_model->m_vertexB);
  api.setIndexBuffer(_model->m_indexB);
  // offsets
  uint32 currentVertexOrigin = 0;
  uint32 currentIndexOrigin = 0;
  // for each mesh in the model
  for (uint32 i = 0; i < _model->meshes.size(); ++i) {
    // get the material
    SPtr<Mesh> mesh = _model->meshes[i];
    // get if the mesh is active or not, if it's not, dont render and keep going.
    if (mesh->getActive()) {
      SPtr<Material> material = mesh->material;
      // set the material textures to the shader
      Vector<SPtr<Texture>> textures = { material->m_diffuse,
                                         material->m_normal,
                                         material->m_height,
                                         material->m_metallic,
                                         material->m_occlusion,
                                         material->m_roughness,
                                         material->m_emissive };
      api.pSSetShaderResourceViews(textures);
      // draw the mesh
      api.drawIndexed(mesh->numIndex, currentIndexOrigin, currentVertexOrigin);
    }
    // update the offsets
    currentIndexOrigin += mesh->numIndex;
    currentVertexOrigin += mesh->vertexCount;
  }
}
PK_CORE_EXPORT RendererManager&
g_RenderManager()
{
  return RendererManager::instance();
}
}
