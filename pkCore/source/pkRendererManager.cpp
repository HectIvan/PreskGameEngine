#include "pkCamera.h"
#include "pkGraphicsAPI.h"
#include "pkLogger.h"
#include "pkRendererManager.h"
#include "pkSamplerState.h"
#include "pkSceneManager.h"
#include "pkTextureManager.h"
#include "pkTextureCodec.h"

namespace pkEngineSDK
{

void
RendererManager::init()
{
  GraphicsAPI& api = g_GraphicAPI();
  TextureManager& tm = g_TextureManager();
  TextureCodec& texCodec = g_TextureCodec();

  uint32 winHeight = api.getSwapChain()->getHeight();
  uint32 winWidth = api.getSwapChain()->getWidth();

  float sizeMulShadow = 10.0f;

  // Texture description
  TextureDesc txDesc;
  txDesc.width = winWidth;
  txDesc.height = winHeight;
  txDesc.format = kPK_FORMAT_R32G32B32A32_FLOAT;
  txDesc.bindFlags = kPK_BIND_SHADER_RESOURCE | kPK_BIND_RENDER_TARGET;
  txDesc.usage = kPK_USAGE_DEFAULT;
  txDesc.mipLevels = 1;
  txDesc.shaderResourceFormat = kPK_FORMAT_R32G32B32A32_FLOAT;
  
  // render target for scene colors.
  SPtr<Texture> albedoRTV = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Albedo, albedoRTV });

  // create the normal render target that will store the normals of the world.
  SPtr<Texture> normalRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Normal, normalRT });

  // render target for the diffuseBRDF result.
  SPtr<Texture> diffuseBRDF = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_DiffuseBRDF, diffuseBRDF });

  // render target for the specularBRDF result.
  SPtr<Texture> specularBRDF = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_SpecularBRDF, specularBRDF });

  // render target for the metallic result.
  SPtr<Texture> ormRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_ORM, ormRT });

  SPtr<Texture> ssaoRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_SSAO, ssaoRT });

  // skybox texture.
  SPtr<Texture> skyboxRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Skybox, skyboxRT });

  // positions texture.
  SPtr<Texture> posRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Positions, posRT });

  // positions texture for the light.
  txDesc.width = winWidth * sizeMulShadow;
  txDesc.height = winHeight * sizeMulShadow;
  SPtr<Texture> posLightRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_PositionsLight, posLightRT });
  txDesc.width = winWidth;
  txDesc.height = winHeight;

  // emissive texture.
  SPtr<Texture> emissiveRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Emissive, emissiveRT });

  SPtr<Texture> emissiveHBlurRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_EmissiveHBlur, emissiveHBlurRT });

  SPtr<Texture> emissiveBlurRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_EmissiveBlur, emissiveBlurRT });

  // IBL texture.
  SPtr<Texture> iblRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_IBL, iblRT });

  // merge texture.
  SPtr<Texture> mergeRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Merge, mergeRT });

  // luminance texture.
  SPtr<Texture> lumRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Luminance, lumRT });

  SPtr<Texture> lumBlurHRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_LumBlurH, lumBlurHRT });

  SPtr<Texture> lumBlurRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_LumBlur, lumBlurRT });

  m_mainSkybox = tm.loadTexture(Path("resources/Skybox_papermill.pkt"));

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
  txDesc.width = winWidth * sizeMulShadow;
  txDesc.height = winHeight * sizeMulShadow;
  SPtr<Texture> shadowDepth = api.createTexture(txDesc);
  m_depthBuffers.insert({ D_BUFFERS::kDB_Light, shadowDepth });
  txDesc.width = winWidth;
  txDesc.height = winHeight;

  // depth buffer quad
  SPtr<Texture> shadowQuadDepth = api.createTexture(txDesc);
  m_depthBuffers.insert({ D_BUFFERS::kDB_QuadShadow, shadowDepth });

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
RendererManager::onShutDown()
{
  m_gBuffers.clear();
  m_depthBuffers.clear();
  m_uavBuffers.clear();

  for (auto& pair : m_passes) {
    pair.second->~Pass();
  }
  m_passes.clear();
  m_mainSkybox = nullptr;
}

void
RendererManager::createPasses()
{
  // pass description
  PassDesc pDesc = PassDesc();

  // Textures
  SPtr<Texture> albedoTex = getGBuffer(G_BUFFERS::kGB_Albedo);
  SPtr<Texture> normalTex = getGBuffer(G_BUFFERS::kGB_Normal);
  SPtr<Texture> ormTex = getGBuffer(G_BUFFERS::kGB_ORM);
  SPtr<Texture> posTex = getGBuffer(G_BUFFERS::kGB_Positions);
  SPtr<Texture> lightPosTex = getGBuffer(G_BUFFERS::kGB_PositionsLight);
  SPtr<Texture> ssaoTex = getGBuffer(G_BUFFERS::kGB_SSAO);
  SPtr<Texture> emissTex = getGBuffer(G_BUFFERS::kGB_Emissive);
  SPtr<Texture> emissHBlurTex = getGBuffer(G_BUFFERS::kGB_EmissiveHBlur);
  SPtr<Texture> emissBlurTex = getGBuffer(G_BUFFERS::kGB_EmissiveBlur);
  SPtr<Texture> diffBRDFTex = getGBuffer(G_BUFFERS::kGB_DiffuseBRDF);
  SPtr<Texture> specBRDFTex = getGBuffer(G_BUFFERS::kGB_SpecularBRDF);
  SPtr<Texture> skyboxTex = getGBuffer(G_BUFFERS::kGB_Skybox);
  SPtr<Texture> iblTex = getGBuffer(G_BUFFERS::kGB_IBL);
  SPtr<Texture> mergeTex = getGBuffer(G_BUFFERS::kGB_Merge);

  // Depth textures
  SPtr<Texture> DepthBuffer = getDepthBuffer(D_BUFFERS::kDB_Base);
  SPtr<Texture> LightDepthBuffer = getDepthBuffer(D_BUFFERS::kDB_Light);
  /****************************************************************************
   * Create the base pass.
   ***************************************************************************/
  pDesc.vSDirectory = Path("shaders/pkVShader.hlsl");
  pDesc.pSDirectory = Path("shaders/pkPShader.hlsl");
  pDesc.vSEntry = "VS";
  pDesc.pSEntry = "PS";
  pDesc.vSModel = "vs_5_0";
  pDesc.pSModel = "ps_5_0";
  pDesc.samAdress = PK_SAM_STATE_ADRESS::kWrap;
  pDesc.samFilters = PK_SAM_STATE_FILTERS::kFilterMigMagMipLinear;
  pDesc.cBSizes = { sizeof(CBMatrix),
                    sizeof(CBMatrix),
                    sizeof(CBMatrix),
                    sizeof(CBMaterialProps) };
  pDesc.inputs = {};
  pDesc.outputs = { albedoTex,
                    normalTex,
                    ormTex,
                    emissTex,
                    posTex };
  pDesc.pDepth = DepthBuffer;
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
  pDesc.outputs = { lightPosTex };
  pDesc.pDepth = LightDepthBuffer;
  SPtr<Pass> shadowPass = make_shared<Pass>(pDesc);
  m_passes.insert({ PASS_TYPE::kP_Shadow, shadowPass });

  /****************************************************************************
   * Shadow Specular Quad Pass
   ***************************************************************************/
  pDesc.vSDirectory = Path("shaders/pkQuadShader.hlsl");
  pDesc.pSDirectory = Path("shaders/pkShadowMapping.hlsl");
  pDesc.cBSizes = { sizeof(CBLight), sizeof(CBCamera), sizeof(Matrix4), sizeof(Vector4) };
  pDesc.inputs = { LightDepthBuffer,
                   DepthBuffer,
                   normalTex,
                   albedoTex,
                   posTex,
                   ormTex,
                   lightPosTex };
  pDesc.outputs = { diffBRDFTex, specBRDFTex };
  pDesc.pDepth = {};// getDepthBuffer(D_BUFFERS::kDB_Base);
  SPtr<Pass> shadowQuadPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_ShadowQuad, shadowQuadPass });

  /****************************************************************************
   * Skybox Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkSkyboxShader.hlsl");
  pDesc.cBSizes = { sizeof(Matrix4), sizeof(Matrix4) };
  pDesc.inputs = { m_mainSkybox };
  pDesc.outputs = { skyboxTex };
  SPtr<Pass> skyboxPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_SkyBox, skyboxPass });

  /****************************************************************************
   * Screen Space Ambient Occlusion Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkPSAOshader.hlsl");
  pDesc.cBSizes = { sizeof(CBSSAO), sizeof(CBVector2x2) };
  pDesc.inputs = { posTex,
                   normalTex};
  pDesc.outputs = { ssaoTex };
  pDesc.samAdress = PK_SAM_STATE_ADRESS::kWrap;
  SPtr<Pass> ssaoPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_SSAO, ssaoPass });

  /****************************************************************************
   * IBR Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkIBLShader.hlsl");
  pDesc.cBSizes = { sizeof(Vector4), sizeof(Vector4) };
  pDesc.inputs = { m_mainSkybox,
                   normalTex,
                   posTex,
                   ormTex,
                   albedoTex };
  pDesc.outputs = { iblTex };
  pDesc.samAdress = PK_SAM_STATE_ADRESS::kClamp;
  SPtr<Pass> iblPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_IBL, iblPass });

  /****************************************************************************
   * Emissive Horizontal Blur Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkBlur.hlsl");
  pDesc.cBSizes = { sizeof(CBBlur) };
  pDesc.inputs = { emissTex };
  pDesc.outputs = { emissHBlurTex };
  pDesc.samAdress = PK_SAM_STATE_ADRESS::kClamp;
  SPtr<Pass> emissiveHPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_EmissiveHBlur, emissiveHPass });

  /****************************************************************************
   * Emissive Blur Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkBlur.hlsl");
  pDesc.cBSizes = { sizeof(CBBlur) };
  pDesc.inputs = { emissHBlurTex };
  pDesc.outputs = { emissBlurTex };
  SPtr<Pass> emissivePass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_EmissiveBlur, emissivePass });

  /****************************************************************************
   * Merge Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkMergeShader.hlsl");
  pDesc.cBSizes = {};
  pDesc.inputs = { albedoTex,
                   diffBRDFTex,
                   specBRDFTex,
                   skyboxTex,
                   iblTex,
                   emissTex,
                   emissBlurTex,
                   ssaoTex }; // to do: do a correct ambient occlusion implementation.
  pDesc.outputs = { mergeTex };
  pDesc.samAdress = PK_SAM_STATE_ADRESS::kWrap;
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
  pDesc.inputs = { mergeTex };
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_Luminance) };
  SPtr<Pass> lumPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_Luminance, lumPass });

  /****************************************************************************
   * Luminance Horizontal Blur
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkBlur.hlsl");
  pDesc.cBSizes = { sizeof(CBBlur) };
  pDesc.inputs = { getGBuffer(G_BUFFERS::kGB_Luminance) };
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_LumBlurH) };
  pDesc.samAdress = PK_SAM_STATE_ADRESS::kClamp;
  SPtr<Pass> lumBlurHPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_LumBlurH, lumBlurHPass });

  /****************************************************************************
   * Luminance Vertical Blur
   ***************************************************************************/
  pDesc.pSDirectory = Path("shaders/pkBlur.hlsl");
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
  pDesc.cBSizes = { sizeof(CBFloat) };
  pDesc.inputs = { mergeTex,
                   getGBuffer(G_BUFFERS::kGB_LumBlur) };
  pDesc.outputs = { g_GraphicAPI().getSwapChain()->getBuffer(0) };
  pDesc.samAdress = PK_SAM_STATE_ADRESS::kWrap;
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

template<class T> void
RendererManager::updateBuffer(const T& _data, const SPtr<ConstantBuffer>& _pCBuffer)
{
  g_GraphicAPI().updateConstantBuffer(_pCBuffer, &_data, 0);
}

void
RendererManager::renderActors(const Vector<SPtr<Actor>> _gameActors)
{
  RendererManager& rManager = g_RenderManager();
  GraphicsAPI& api = g_GraphicAPI();
  SPtr<Pass> basePass = rManager.getPass(PASS_TYPE::kP_Base);
  SPtr<Pass> shadowPass = rManager.getPass(PASS_TYPE::kP_Shadow);
  // for each actor
  for (uint32 i = 0; i < _gameActors.size(); ++i) {
    SPtr<Actor> currActor = _gameActors[i];
    // if actor is not active
    if (!currActor->isActive()) {
      continue;
    }
    // Get the final matrix by taking into account the parent actors
    SPtr<Actor> parent = currActor->m_parent;
    Matrix4 transform = currActor->m_transform;
    // while there's a parent
    while (parent) {
      // add the parent transform to the current transform matrix
      transform *= parent->m_transform;
      // the next parent will be the parent of this parent
      parent = parent->m_parent;
    }
    // set the current actor transform as the world in which the shader will work 
    api.updateConstantBuffer(basePass->getCBuffer(2), &transform, sizeof(Matrix4));
    api.updateConstantBuffer(shadowPass->getCBuffer(2), &transform, sizeof(Matrix4));

    // render the model of the actor
    SPtr<Model> model = currActor->getComponent<Model>();
    if (model && model->isActive()) {
      renderModel(model);
    }
    // if the actor has children, do the same for them (recursive)
    if (!currActor->m_children.empty()) {
      renderActors(currActor->m_children);
    }
  }
}


void
RendererManager::renderModel(const SPtr<Model>& _model)
{
  // get a reference from managers and passes.
  RendererManager& rManager = g_RenderManager();
  GraphicsAPI& api = g_GraphicAPI();
  SPtr<Pass> basePass = rManager.getPass(PASS_TYPE::kP_Base);
  SPtr<Pass> shadowPass = rManager.getPass(PASS_TYPE::kP_Shadow);
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
      api.updateConstantBuffer(basePass->getCBuffer(3),
                               &material->m_properties, sizeof(CBMaterialProps));
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
