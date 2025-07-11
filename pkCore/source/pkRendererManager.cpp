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

  // render target for the metallic result
  SPtr<Texture> metallicRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Metallic, metallicRT });

  // render target for the luminance result
  SPtr<Texture> luminanceRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Luminance, luminanceRT });

  // horizontal blur of luminance
  SPtr<Texture> hBlurredluminanceRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_HBlurredLuminance, hBlurredluminanceRT });

  // vertical blur of luminance
  SPtr<Texture> vBlurredluminanceRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_VBlurredLuminance, vBlurredluminanceRT });

  // skybox texture
  SPtr<Texture> skyboxTex = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Skybox, skyboxTex });

  // ---------------------------------------------------------- //
  // DEPTH TARGETS
  // ---------------------------------------------------------- //
  // depth buffer description
  txDesc.format = TEXTURE_FORMAT::kPK_FORMAT_R32_TYPELESS;
  txDesc.bindFlags = kPK_BIND_SHADER_RESOURCE | kPK_BIND_DEPTH_STENCIL;
  txDesc.shaderResourceFormat = TEXTURE_FORMAT::kPK_FORMAT_R32_FLOAT;
  // camera depth buffer
  SPtr<Texture> depthBuffer = api.createTexture(txDesc);
  m_depthBuffers.insert({ D_BUFFERS::kDB_Base, depthBuffer });

  // light depth buffer
  SPtr<Texture> shadowDepth = api.createTexture(txDesc);
  m_depthBuffers.insert({ D_BUFFERS::kDB_Shadow, shadowDepth });

  // ---------------------------------------------------------- //
  // UNORDERED ACCESS VIEWS
  // ---------------------------------------------------------- //
  txDesc.bindFlags = kPK_BIND_UNORDERED_ACCESS | kPK_BIND_SHADER_RESOURCE;
  txDesc.format = TEXTURE_FORMAT::kPK_FORMAT_R8G8B8A8_UNORM;
  txDesc.shaderResourceFormat = TEXTURE_FORMAT::kPK_FORMAT_R8G8B8A8_UNORM;
  SPtr<Texture> testUAV = api.createTexture(txDesc);
  m_uavBuffers.insert({ UAV_BUFFERS::kDB_Test, testUAV });
  
  // create the passes needed
  createPasses();
}

void
RendererManager::createPasses()
{
  TextureManager& tm = g_TextureManager().instance();
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
  pDesc.samAdress = SAM_STATE_ADRESS::kWrap;
  pDesc.samFilters = SAM_STATE_FILTERS::kFilterMigMagMipLinear;
  pDesc.cBSizes = { sizeof(CBView), sizeof(CBProjection), sizeof(CBTransform), sizeof(CBLight),
                    sizeof(CBCamera) };
  pDesc.inputs = {};
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_Albedo), getGBuffer(G_BUFFERS::kGB_Normal),
                    getGBuffer(G_BUFFERS::kGB_Metallic) };
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
  // pDesc.pSDirectory = L"shaders/pkPShaderDepth.hlsl";
  // pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_Albedo) };
  pDesc.pDepth = getDepthBuffer(D_BUFFERS::kDB_Shadow);
  SPtr<Pass> shadowPass = make_shared<Pass>(pDesc);
  m_passes.insert({ PASS_TYPE::kP_Shadow, shadowPass });

  // cancel rasterizer state
  pDesc.rSExists = false;

  /****************************************************************************
   * Ambient Occlussion Pass
   ***************************************************************************/
  // pDesc.pSDirectory = L"shaders/pkPSAOshader.hlsl";
  // pDesc.cBSizes = { sizeof(CBAOData) };
  // pDesc.samAdress = SAM_STATE_ADRESS::kClamp;
  // pDesc.type = PK_PASS_TYPE::kDeferred;
  // SPtr<Pass> AOPass = make_shared<Pass>(pDesc);
  // // insert to the passes
  // m_passes.insert({ PASS_TYPE::kP_AO, AOPass });

  /****************************************************************************
   * Shadow Quad pass
   ***************************************************************************/
  pDesc.vSDirectory = Path("shaders/pkQuadShader.hlsl");
  pDesc.pSDirectory = Path("shaders/pkShadowMapping.hlsl");
  pDesc.cBSizes = { sizeof(CBLight), sizeof(CBCamera), sizeof(CBCamera), sizeof(Matrix4), 
                    sizeof(Matrix4), sizeof(CBShadowParam) };
  pDesc.inputs = { getDepthBuffer(D_BUFFERS::kDB_Shadow),
                   getDepthBuffer(D_BUFFERS::kDB_Base),
                   getGBuffer(G_BUFFERS::kGB_Normal),
                   getGBuffer(G_BUFFERS::kGB_Metallic) };
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_Shadow) };
  pDesc.pDepth = nullptr;
  pDesc.samAdress = SAM_STATE_ADRESS::kClamp;
  SPtr<Pass> shadowDef = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_ShadowDef, shadowDef });

  /****************************************************************************
   * Luminance Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkLuminanceQuad.hlsl");
  pDesc.cBSizes = { sizeof(CBLuminance) };
  pDesc.inputs = { getGBuffer(G_BUFFERS::kGB_Albedo) };
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_Luminance) };
  SPtr<Pass> luminancePass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_Luminance, luminancePass });

  /****************************************************************************
   * Horizontal Blur Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkHBlur.hlsl");
  pDesc.cBSizes = { sizeof(CBBlur) };
  pDesc.inputs = { getGBuffer(G_BUFFERS::kGB_Luminance) };
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_HBlurredLuminance) };
  SPtr<Pass> hBlurPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_HBlur, hBlurPass });

  /****************************************************************************
   * Vertical Blur Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkVBlur.hlsl");
  pDesc.cBSizes = { sizeof(CBBlur) };
  pDesc.inputs = { getGBuffer(G_BUFFERS::kGB_HBlurredLuminance) };
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_VBlurredLuminance) };
  SPtr<Pass> vBlurPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_VBlur, vBlurPass });

  /****************************************************************************
   * Tone mapping Quad pass
   ***************************************************************************/
  SPtr<Texture> skyboxTex = tm.loadTexture(Path("textures/skybox.hdr"));
  pDesc.pSDirectory = Path("shaders/pkToneMapQuadShader.hlsl");
  //         pDesc.cSDirectory = Path("shaders/pkCShaderTest.hlsl");
  //         pDesc.cSEntry = "CSMain";
  //         pDesc.cSModel = "c_5_0";
  pDesc.cBSizes = {};
  pDesc.inputs = { getGBuffer(G_BUFFERS::kGB_Albedo),
                   getGBuffer(G_BUFFERS::kGB_VBlurredLuminance),
                   getUAVBuffer(UAV_BUFFERS::kDB_Test),
                   skyboxTex };
  pDesc.outputs = { g_GraphicAPI().getSwapChain()->getBuffer(0) };
  SPtr<Pass> tonePass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_Tone, tonePass });

  //        pDesc.pSDirectory = Path("shaders/pkSkyboxShader.hlsl");
  //        pDesc.vSDirectory = pDesc.pSDirectory;
  //        pDesc.pSEntry = "PS";
  //        pDesc.vSEntry = "VS";
  //        pDesc.cBSizes = { sizeof(CBCamera) };
  //        pDesc.inputs = { skyboxTex };
  //        pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_Skybox) };
  //        SPtr<Pass> skyboxPass = make_shared<Pass>(pDesc);
  //        // insert to the passes
  //        m_passes.insert({ PASS_TYPE::kP_SkyBox, skyboxPass });

  /****************************************************************************
   * Shadow Compute
   ***************************************************************************/
  pDesc.vSDirectory = Path("");
  pDesc.pSDirectory = Path("");
  pDesc.vSEntry = "";
  pDesc.pSEntry = "";
  pDesc.vSModel = "";
  pDesc.pSModel = "";
  pDesc.cSDirectory = Path("shaders/pkCShadowMapping.hlsl");
  pDesc.cSEntry = "CSMain";
  pDesc.cSModel = "cs_5_0";
  pDesc.inputs = { getGBuffer(G_BUFFERS::kGB_Normal),
                   getGBuffer(G_BUFFERS::kGB_Metallic),
                   getDepthBuffer(D_BUFFERS::kDB_Shadow),
                   getDepthBuffer(D_BUFFERS::kDB_Base) };
  pDesc.cBSizes = { sizeof(CBLight), sizeof(CBCamera), sizeof(CBCamera), sizeof(Matrix4),
                    sizeof(Matrix4), sizeof(CBShadowParam) };
  pDesc.outputs = {};
  pDesc.uavs = { getUAVBuffer(UAV_BUFFERS::kDB_Test) };
  SPtr<Pass> testCompute = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_CShadows, testCompute });

}

SPtr<Pass>
RendererManager::getPass(PASS_TYPE::E _type)
{
  return m_passes.find(_type)->second;
}

SPtr<Texture>&
RendererManager::getGBuffer(G_BUFFERS::E _type)
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

SPtr<Texture>&
RendererManager::getDepthBuffer(const D_BUFFERS::E _type)
{
  return m_depthBuffers.find(_type)->second;
}

SPtr<Texture>&
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
RendererManager::updateBuffer(T& _data, SPtr<ConstantBuffer> _pCBuffer)
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
  for (uint32 i = 0; i < sm.getActiveScene()->getAllActors().size(); ++i) {
    // Cast to a gameObject, if it fails, do none of the following process
    SPtr<Actor> actor = sm.getActiveScene()->getActor(i);
    // if the actor has a model component
    if (actor->getComponent<Model>()) {
      api.setVertexBuffer(actor->getComponent<Model>()->m_vertexB);
      api.setIndexBuffer(actor->getComponent<Model>()->m_indexB);
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
    // set the current actor transform as the world in which the shader will work in
    api.updateConstantBuffer(rManager.getPass(PASS_TYPE::kP_Base)->getCBuffer(2),
                             &transform,
                             static_cast<uint32>(sizeof(CBTransform)));
    api.updateConstantBuffer(rManager.getPass(PASS_TYPE::kP_Shadow)->getCBuffer(2),
                             &transform,
                             static_cast<uint32>(sizeof(CBTransform)));

    // render the model of the actor
    if (_gameActors[i]->getComponent<Model>()) {
      renderModel(*_gameActors[i]->getComponent<Model>());
    }
    // if the actor has children, do the same for them (recursive)
    if (!_gameActors[i]->m_children.empty()) {
      renderActors(_gameActors[i]->m_children);
    }
  }
}


void
RendererManager::renderModel(Model& _model)
{
  GraphicsAPI& api = g_GraphicAPI().instance();
  // get a reference from the api
  api.setVertexBuffer(_model.m_vertexB);
  api.setIndexBuffer(_model.m_indexB);
  // offsets
  uint32 currentVertexOrigin = 0;
  uint32 currentIndexOrigin = 0;
  // for each mesh in the model
  for (uint32 i = 0; i < _model.meshes.size(); ++i) {
    // get the material
    SPtr<Material> material = _model.meshes[i]->material;
    // set the material textures to the shader
    Vector<SPtr<Texture>> textures = { material->diffuse, material->normal, material->height,
                                       material->metallic, material->occlusion };
    api.pSSetShaderResourceViews(textures);
    // draw the mesh
    api.drawIndexed(static_cast<uint32>(_model.meshes[i]->numIndex),
                               currentIndexOrigin,
                               currentVertexOrigin);
    // update the offsets
    currentIndexOrigin += static_cast<uint32>(_model.meshes[i]->numIndex);
    currentVertexOrigin += static_cast<uint32>(_model.meshes[i]->vertexCount);
  }
}
PK_CORE_EXPORT RendererManager&
g_RenderManager()
{
  return RendererManager::instance();
}
}
