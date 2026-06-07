#include "pkAssetResourceManager.h"
#include "pkCamera.h"
#include "pkGraphicsAPI.h"
#include "pkLogger.h"
#include "pkRendererManager.h"
#include "pkSamplerState.h"
#include "pkSceneManager.h"
#include "pkTextureManager.h"
#include "pkTextureCodec.h"
#include "pkShaderManager.h"

namespace pkEngineSDK
{

void
RendererManager::init()
{
  AssetResourceManager& assetMan = g_AssetResourceManager();
  GraphicsAPI& api = g_GraphicAPI();
  TextureManager& tm = g_TextureManager();

  const uint32 winHeight = api.getSwapChain()->getHeight();
  const uint32 winWidth = api.getSwapChain()->getWidth();

  float sizeMulShadow = 3.0f;

  // Texture description
  TextureDesc txDesc;
  txDesc.width = winWidth;
  txDesc.height = winHeight;  
  txDesc.format = kPK_FORMAT_R32G32B32A32_FLOAT;
  txDesc.bindFlags = kPK_BIND_SHADER_RESOURCE | kPK_BIND_RENDER_TARGET;
  txDesc.usage = kPK_USAGE_DEFAULT;
  txDesc.mipLevels = 1;
  txDesc.shaderResourceFormat = kPK_FORMAT_R32G32B32A32_FLOAT;

  txDesc.name = "Target Render";
  m_targetRT = api.createTexture(txDesc);
  txDesc.name = "Target Actors";
  m_actorsRT = api.createTexture(txDesc);
  txDesc.name = "Target Transparent";
  m_transpActorsRT = api.createTexture(txDesc);
  
  // render target for scene colors.
  txDesc.name = "AlbedoRT";
  SPtr<Texture> albedoRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Albedo, albedoRT });

  txDesc.name = "AlbedoTranspRT";
  SPtr<Texture> albedoTranspRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_TranspAlbedo, albedoTranspRT });

  // create the normal render target that will store the normals of the world.
  txDesc.name = "NormalRT";
  SPtr<Texture> normalRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Normal, normalRT });
  
  txDesc.name = "NormalTranspRT";
  SPtr<Texture> normalTranspRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_TranspNormal, normalTranspRT });

  // render target for the orm result.
  txDesc.name = "ORMRT";
  SPtr<Texture> ormRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_ORM, ormRT });

  txDesc.name = "ORMTranspRT";
  SPtr<Texture> ormTranspRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_TranspORM, ormTranspRT });

  txDesc.name = "SSAORT";
  SPtr<Texture> ssaoRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_SSAO, ssaoRT });

  // BRDF texture.
  txDesc.name = "BRDFRT";
  SPtr<Texture> brdfRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_BRDF, brdfRT });

  txDesc.name = "BRDFTranspRT";
  SPtr<Texture> brdfTranspRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_BRDFTransp, brdfTranspRT });

  // skybox texture.
  txDesc.name = "SkyboxRT";
  SPtr<Texture> skyboxRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Skybox, skyboxRT });

  // positions texture.
  txDesc.name = "PosRT";
  SPtr<Texture> posRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Positions, posRT });

  txDesc.name = "PosTranspRT";
  SPtr<Texture> posTranspRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_TranspPos, posTranspRT });

  // positions texture for the light.
  txDesc.name = "PosLightRT";
  txDesc.width = winWidth * sizeMulShadow;
  txDesc.height = winHeight * sizeMulShadow;
  SPtr<Texture> posLightRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_PositionsLight, posLightRT });
  txDesc.width = winWidth;
  txDesc.height = winHeight;

  // emissive texture.
  txDesc.name = "EmissRT";
  SPtr<Texture> emissiveRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Emissive, emissiveRT });

  txDesc.name = "EmissTranspRT";
  SPtr<Texture> emissiveTranspRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_TranspEmiss, emissiveTranspRT });

  txDesc.name = "EmissHBlurRT";
  SPtr<Texture> emissiveHBlurRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_EmissiveHBlur, emissiveHBlurRT });

  txDesc.name = "EmissBlurRT";
  SPtr<Texture> emissiveBlurRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_EmissiveBlur, emissiveBlurRT });

  // luminance texture.
  txDesc.name = "LumRT";
  SPtr<Texture> lumRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Luminance, lumRT });

  txDesc.name = "LumBlurHRT";
  SPtr<Texture> lumBlurHRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_LumBlurH, lumBlurHRT });

  txDesc.name = "LumBlurRT";
  SPtr<Texture> lumBlurRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_LumBlur, lumBlurRT });

  SPtr<BaseResource> resSky = make_shared<TextureResource>();
  
  g_TextureCodec().createResourceFromFile(Path("textures/Skybox_papermill.hdr"));
  const bool success = resSky->softLoad(Path("resources/Skybox_papermill.pkt"));
  m_mainSkybox = api.createEmptyTexture();
  if (success) {
    assetMan.insertNewResource(resSky);
    SPtr<Texture> skyboxTexture = tm.createTexture(resSky->m_id);
    m_mainSkybox = skyboxTexture;
  }

  // Cube map for the skybox
  txDesc.name = "CubeMapRT";
  txDesc.width = 512;
  txDesc.height = 512;
  txDesc.isCube = true;
  SPtr<Texture> cubeMapRT = api.createTexture(txDesc);
  txDesc.width = 64;
  txDesc.height = 64;
  m_gBuffers.insert({ G_BUFFERS::kGB_CubeMap, cubeMapRT });
  txDesc.name = "irradianceMap";
  SPtr<Texture> irradianceRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_IrradianceMap, irradianceRT });
  txDesc.isCube = false;

  txDesc.name = "LUTRT";
  txDesc.width = 512;
  txDesc.height = 512;
  SPtr<Texture> LUTRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_LUT, LUTRT });
  txDesc.width = winWidth;
  txDesc.height = winHeight;

  // ---------------------------------------------------------- //
  // DEPTH TARGETS
  // ---------------------------------------------------------- //
  // depth buffer description
  txDesc.format = PK_TEXTURE_FORMAT::kPK_FORMAT_R32_TYPELESS;
  txDesc.bindFlags = kPK_BIND_SHADER_RESOURCE | kPK_BIND_DEPTH_STENCIL;
  txDesc.shaderResourceFormat = PK_TEXTURE_FORMAT::kPK_FORMAT_R32_FLOAT;
  // camera depth buffer
  txDesc.name = "Depth";
  SPtr<Texture> depthBuffer = api.createTexture(txDesc);
  m_depthBuffers.insert({ D_BUFFERS::kDB_Base, depthBuffer });

  txDesc.name = "TranspDepth";
  SPtr<Texture> transparencyDepth = api.createTexture(txDesc);
  m_depthBuffers.insert({ D_BUFFERS::kDB_Transparency, transparencyDepth });

  // light depth buffer
  txDesc.name = "LightDepth";
  txDesc.width = winWidth * sizeMulShadow;
  txDesc.height = winHeight * sizeMulShadow;
  SPtr<Texture> shadowDepth = api.createTexture(txDesc);
  m_depthBuffers.insert({ D_BUFFERS::kDB_Light, shadowDepth });
  txDesc.width = winWidth;
  txDesc.height = winHeight;

  // ---------------------------------------------------------- //
  // UNORDERED ACCESS VIEWS
  // ---------------------------------------------------------- //
  // shadows
  txDesc.bindFlags = kPK_BIND_UNORDERED_ACCESS | kPK_BIND_SHADER_RESOURCE;
  txDesc.format = PK_TEXTURE_FORMAT::kPK_FORMAT_R8G8B8A8_UNORM;
  txDesc.shaderResourceFormat = PK_TEXTURE_FORMAT::kPK_FORMAT_R8G8B8A8_UNORM;

  generateCubeMap(m_mainSkybox, cubeMapRT);
  generateIrradianceCubeMap(cubeMapRT, irradianceRT);
  
  // create the passes needed
  createPasses();
}

void
RendererManager::onShutDown()
{
  m_passes.clear();
  m_gBuffers.clear();
  m_depthBuffers.clear();
  m_uavBuffers.clear();
}

void
RendererManager::createPasses()
{
  // object data
  const SPtr<Texture> posLightRT = getGBuffer(G_BUFFERS::kGB_PositionsLight);

  const SPtr<Texture> albedoRT = getGBuffer(G_BUFFERS::kGB_Albedo);
  const SPtr<Texture> normalRT = getGBuffer(G_BUFFERS::kGB_Normal);
  const SPtr<Texture> ormRT = getGBuffer(G_BUFFERS::kGB_ORM);
  const SPtr<Texture> emissRT = getGBuffer(G_BUFFERS::kGB_Emissive);
  const SPtr<Texture> posRT = getGBuffer(G_BUFFERS::kGB_Positions);
  const SPtr<Texture> brdfRT = getGBuffer(G_BUFFERS::kGB_BRDF);
 
  // transparent data.
  const SPtr<Texture> transpAlbedo = getGBuffer(G_BUFFERS::kGB_TranspAlbedo);
  const SPtr<Texture> transpNormal = getGBuffer(G_BUFFERS::kGB_TranspNormal);
  const SPtr<Texture> transpORM = getGBuffer(G_BUFFERS::kGB_TranspORM);
  const SPtr<Texture> transpEmiss = getGBuffer(G_BUFFERS::kGB_TranspEmiss);
  const SPtr<Texture> transpPos = getGBuffer(G_BUFFERS::kGB_TranspPos);
  const SPtr<Texture> brdfTranspRT = getGBuffer(G_BUFFERS::kGB_BRDFTransp);

  // Skybox pass
  const SPtr<Texture> skyboxRT = getGBuffer(G_BUFFERS::kGB_Skybox);
  const SPtr<Texture> cubeMapRT = getGBuffer(G_BUFFERS::kGB_CubeMap);
  const SPtr<Texture> irradianceRT = getGBuffer(G_BUFFERS::kGB_IrradianceMap);

  // Post Process passes
  const SPtr<Texture> ssaoRT = getGBuffer(G_BUFFERS::kGB_SSAO);
  const SPtr<Texture> emissHBlurRT = getGBuffer(G_BUFFERS::kGB_EmissiveHBlur);
  const SPtr<Texture> emissBlurRT = getGBuffer(G_BUFFERS::kGB_EmissiveBlur);
  const SPtr<Texture> lumBlurRT = getGBuffer(G_BUFFERS::kGB_LumBlur);

  // Depth textures
  const SPtr<Texture> DepthBuffer = getDepthBuffer(D_BUFFERS::kDB_Base);
  const SPtr<Texture> LightDepthBuffer = getDepthBuffer(D_BUFFERS::kDB_Light);
  const SPtr<Texture> transpDepth = getDepthBuffer(D_BUFFERS::kDB_Transparency);

  PixelDesc pDesc;
  ComputeDesc cDesc;

  /****************************************************************************
   * Create the base pass.
   ***************************************************************************/
  LOG_REGISTER("---------Creating base pass.---------", __FILE__, __LINE__);
  pDesc.vSKey = ShaderKey("resources/pkVShader.pks", "VS", "vs_5_0");
  pDesc.pSKey = ShaderKey("resources/pkPShader.pks", "PS", "ps_5_0");
  pDesc.samAdress = PK_SAM_STATE_ADRESS::kWrap;
  pDesc.samFilters = PK_SAM_STATE_FILTERS::kFilterMigMagMipLinear;
  pDesc.cBSizes = { sizeof(CBWVP), sizeof(CBMaterialProps)};
  pDesc.inputs = {};
  pDesc.outputs = { albedoRT, normalRT, ormRT, emissRT, posRT };
  pDesc.pDepth = DepthBuffer;
  // rasterizer state
  pDesc.rSExists = true;
  pDesc.rSCullMode = RS_CULL_MODE::kPK_CULL_NONE;
  pDesc.rSFillMode = RS_FILL_MODE::kPK_FILL_SOLID;
  pDesc.rSFrontCounterClockwise = false;
  pDesc.rSDepthClipEnable = true;
  // make the pass
  SPtr<Pass> basePass = make_shared<Pass>(pDesc);
  basePass->m_name = "Base Pass";
  // insert to the pass map.
  m_passes.insert({ PASS_TYPE::kP_Base, basePass });

  /****************************************************************************
   * Create the transparency pass.
   ***************************************************************************/
  LOG_REGISTER("---------Creating transparency pass.---------", __FILE__, __LINE__);
  pDesc.pSKey = ShaderKey("resources/pkPSTransparency.pks", "PS", "ps_5_0");
  pDesc.outputs = { transpAlbedo, transpNormal, transpORM, transpEmiss, transpPos };
  pDesc.pDepth = transpDepth;
  // make the pass
  SPtr<Pass> transparencyPass = make_shared<Pass>(pDesc);
  transparencyPass->m_name = "Transparency Pass";
  // insert to the pass map.
  m_passes.insert({ PASS_TYPE::kP_Transparency, transparencyPass });

  /****************************************************************************
   * Light positions Pass
   ***************************************************************************/
  LOG_REGISTER("---------Creating light positions pass.---------", __FILE__, __LINE__);
  pDesc.pSKey = ShaderKey("resources/pkPShaderDepth.pks", "PS", "ps_5_0");
  pDesc.outputs = { posLightRT };
  pDesc.pDepth = LightDepthBuffer;
  SPtr<Pass> ligtPosPass = make_shared<Pass>(pDesc);
  ligtPosPass->m_name = "Light Positions Pass";
  m_passes.insert({ PASS_TYPE::kP_LightPositions, ligtPosPass });
  
  /****************************************************************************
   * BRDF Quad Pass
   ***************************************************************************/
  LOG_REGISTER("---------Creating BRDF Quad pass.---------", __FILE__, __LINE__);
  pDesc.vSKey = ShaderKey("resources/pkQuadShader.pks", "VS", "vs_5_0");
  pDesc.pSKey = ShaderKey("resources/pkLightShader.pks", "PS", "ps_5_0");
  pDesc.cBSizes = { sizeof(CBLight),
                    sizeof(CBCamera) };
  pDesc.inputs = { DepthBuffer,
                   posRT,
                   posLightRT,
                   albedoRT,
                   normalRT,
                   ormRT,
                   m_mainSkybox,
                   cubeMapRT,
                   irradianceRT };
  pDesc.outputs = { brdfRT };
  pDesc.pDepth = {};
  SPtr<Pass> lightQuad = make_shared<Pass>(pDesc);
  lightQuad->m_name = "Light Quad Pass";
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_Light, lightQuad });

  /****************************************************************************
   * Light Transparency Quad Pass
   ***************************************************************************/
  LOG_REGISTER("---------Creating Light transparency pass.---------", __FILE__, __LINE__);
  pDesc.inputs = { transpDepth,
                   transpPos,
                   posLightRT,
                   transpAlbedo,
                   transpNormal,
                   transpORM,
                   m_mainSkybox,
                   cubeMapRT,
                   irradianceRT };
  pDesc.outputs = { brdfTranspRT };
  SPtr<Pass> lightTranspQuad = make_shared<Pass>(pDesc);
  lightTranspQuad->m_name = "Light Transparency Quad Pass";
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_LightTransparency, lightTranspQuad });

  /****************************************************************************
   * Skybox Quad pass
   ***************************************************************************/
  LOG_REGISTER("---------Creating SkyBox pass.---------", __FILE__, __LINE__);
  pDesc.pSKey = ShaderKey("resources/pkSkyboxShader.pks", "PS", "ps_5_0");
  pDesc.cBSizes = { sizeof(Matrix4), sizeof(Matrix4) };
  pDesc.inputs = { m_mainSkybox };
  pDesc.outputs = { skyboxRT };
  SPtr<Pass> skyboxPass = make_shared<Pass>(pDesc);
  skyboxPass->m_name = "Skybox Pass";
  m_passes.insert({ PASS_TYPE::kP_SkyBox, skyboxPass });

  /****************************************************************************
   * Screen Space Ambient Occlusion Quad pass
   ***************************************************************************/
  LOG_REGISTER("---------Creating SSAO pass.---------", __FILE__, __LINE__);
  pDesc.pSKey = ShaderKey("resources/pkPSAOshader.pks", "PS", "ps_5_0");
  pDesc.cBSizes = { sizeof(CBSSAO), sizeof(CBVector2x2) };
  pDesc.inputs = { posRT,
                   normalRT };
  pDesc.outputs = { ssaoRT };
  pDesc.samAdress = PK_SAM_STATE_ADRESS::kWrap;
  SPtr<Pass> ssaoPass = make_shared<Pass>(pDesc);
  ssaoPass->m_name = "SSAO Pass";
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_SSAO, ssaoPass });

  /****************************************************************************
   * Emissive Horizontal Blur Quad pass
   ***************************************************************************/
  LOG_REGISTER("---------Creating Emissive Horizontal Blur pass.---------", __FILE__, __LINE__);
  pDesc.pSKey = ShaderKey("resources/pkBlur.pks", "PS", "ps_5_0");
  pDesc.cBSizes = { sizeof(CBBlur) };
  pDesc.inputs = { emissRT };
  pDesc.outputs = { emissHBlurRT };
  pDesc.samAdress = PK_SAM_STATE_ADRESS::kClamp;
  SPtr<Pass> emissiveHPass = make_shared<Pass>(pDesc);
  emissiveHPass->m_name = "Emissive Horizontal Blur Pass";
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_EmissiveHBlur, emissiveHPass });

  /****************************************************************************
   * Emissive Blur Quad pass
   ***************************************************************************/
  LOG_REGISTER("---------Creating Emissive Vertical Blur pass.---------", __FILE__, __LINE__);
  pDesc.pSKey = ShaderKey("resources/pkBlur.pks", "PS", "ps_5_0");
  pDesc.cBSizes = { sizeof(CBBlur) };
  pDesc.inputs = { emissHBlurRT };
  pDesc.outputs = { emissBlurRT };
  SPtr<Pass> emissivePass = make_shared<Pass>(pDesc);
  emissivePass->m_name = "Emissive Vertical Blur Pass";
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_EmissiveBlur, emissivePass });

  /****************************************************************************
   * Luminance
   ***************************************************************************/
  LOG_REGISTER("---------Creating Luminance pass.---------", __FILE__, __LINE__);
  pDesc.pSKey = ShaderKey("resources/pkLuminanceQuad.pks", "PS", "ps_5_0");
  pDesc.cBSizes = { sizeof(CBVector2x2) };
  pDesc.inputs = { brdfRT };
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_Luminance) };
  SPtr<Pass> lumPass = make_shared<Pass>(pDesc);
  lumPass->m_name = "Luminance Pass";
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_Luminance, lumPass });

  /****************************************************************************
   * Luminance Horizontal Blur
   ***************************************************************************/
  LOG_REGISTER("---------Creating Luminance Horizontal Blur pass.---------", __FILE__, __LINE__);
  pDesc.pSKey = ShaderKey("resources/pkBlur.pks", "PS", "ps_5_0");
  pDesc.cBSizes = { sizeof(CBBlur) };
  pDesc.inputs = { getGBuffer(G_BUFFERS::kGB_Luminance) };
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_LumBlurH) };
  pDesc.samAdress = PK_SAM_STATE_ADRESS::kClamp;
  SPtr<Pass> lumBlurHPass = make_shared<Pass>(pDesc);
  lumBlurHPass->m_name = "Luminance Horizontal Blur Pass";
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_LumBlurH, lumBlurHPass });

  /****************************************************************************
   * Luminance Vertical Blur
   ***************************************************************************/
  LOG_REGISTER("---------Creating Luminance Vertical Blur pass.---------", __FILE__, __LINE__);
  pDesc.pSKey = ShaderKey("resources/pkBlur.pks", "PS", "ps_5_0");
  pDesc.cBSizes = { sizeof(CBBlur) };
  pDesc.inputs = { getGBuffer(G_BUFFERS::kGB_LumBlurH) };
  pDesc.outputs = { lumBlurRT };
  SPtr<Pass> lumBlurPass = make_shared<Pass>(pDesc);
  lumBlurPass->m_name = "Luminance Vertical Blur Pass";
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_LumBlur, lumBlurPass });

  /****************************************************************************
   * Merge pass
   ***************************************************************************/
  LOG_REGISTER("---------Creating Merge pass.---------", __FILE__, __LINE__);
  pDesc.pSKey = ShaderKey("resources/pkMergeShader.pks", "PS", "ps_5_0");
  pDesc.cBSizes = {};
  pDesc.inputs = { brdfRT, brdfTranspRT, skyboxRT, emissBlurRT, lumBlurRT };
  pDesc.outputs = { m_targetRT };
  pDesc.samAdress = PK_SAM_STATE_ADRESS::kWrap;
  SPtr<Pass> mergePass = make_shared<Pass>(pDesc);
  mergePass->m_name = "Merge Pass";
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_Merge, mergePass });
  LOG_REGISTER("-----------------------------------------", __FILE__, __LINE__);

  /****************************************************************************
   * Tone mapping Quad pass
   ***************************************************************************/
  LOG_REGISTER("---------Creating Tone Map pass.---------", __FILE__, __LINE__);
  pDesc.pSKey = ShaderKey("resources/pkToneMap.pks", "PS", "ps_5_0");
  pDesc.cBSizes = { sizeof(CBFloat) };
  pDesc.inputs = { m_targetRT};
  pDesc.outputs = { g_GraphicAPI().getSwapChain()->getBuffer(0) };
  pDesc.samAdress = PK_SAM_STATE_ADRESS::kWrap;
  SPtr<Pass> TonePass = make_shared<Pass>(pDesc);
  TonePass->m_name = "Tone Mapping Pass";
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_Tone, TonePass });
  LOG_REGISTER("-----------------------------------------", __FILE__, __LINE__);


  /****************************************************************************
   * Material Pass
   ***************************************************************************/
  LOG_REGISTER("---------Creating Material Specific pass.---------", __FILE__, __LINE__);
  pDesc.vSKey = ShaderKey("resources/pkQuadShader.pks", "VS", "vs_5_0");
  pDesc.pSKey = ShaderKey("resources/pkLightShader.pks", "PS", "ps_5_0");
  pDesc.cBSizes = { sizeof(CBLight),
                    sizeof(CBCamera) };
  pDesc.inputs = { DepthBuffer,
                   posRT,
                   posLightRT,
                   albedoRT,
                   normalRT,
                   ormRT,
                   m_mainSkybox,
                   cubeMapRT };
  pDesc.outputs = { m_actorsRT };
  pDesc.pDepth = {};
  pDesc.samAdress = PK_SAM_STATE_ADRESS::kClamp;
  SPtr<Pass> matPass = make_shared<Pass>(pDesc);
  matPass->m_name = "Material Pass";
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_Material, matPass });
  LOG_REGISTER("-----------------------------------------", __FILE__, __LINE__);
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
RendererManager::generateCubeMap(const SPtr<Texture>& _pInput, const SPtr<Texture>& _pOutput)
{
  PK_ASSERT(_pInput);
  PK_ASSERT(_pOutput);

  GraphicsAPI& api = g_GraphicAPI();
  ShaderManager& shaderMan = g_ShaderManager();

  const ShaderKey vShaderKey("resources/pkQuadShader.pks", "VS", "vs_5_0");
  const ShaderKey pShaderKey("resources/pkCubeMapShader.pks", "PS", "ps_5_0");
  SPtr<Shader> pShader = shaderMan.getShader(pShaderKey);
  SPtr<Shader> vShader = shaderMan.getShader(vShaderKey);
  const SPtr<InputLayout> iLayout = api.createInputLayoutFromVShader(vShader);

  // create the sampler state.
  SPtr<SamplerState> samplerState = api.createSamplerState(PK_SAM_STATE_ADRESS::kWrap,
                                                           PK_SAM_STATE_FILTERS::kFilterMigMagMipLinear);

  const Vector2 viewportSize = _pOutput->getSize();

  // create constant buffer.
  const uint32 CBFloatSize = sizeof(CBFloat);
  SPtr<ConstantBuffer> cBuffer = api.createConstantBuffer(CBFloatSize);

  // set the necessary resources.
  api.setViewport(viewportSize);
  api.setInputLayout(iLayout);
  api.setVShader(vShader);
  api.setPShader(pShader);
  api.setSampler(samplerState);
  api.pSSetShaderResourceViews({ _pInput });
  api.pSSetConstantBuffers({ cBuffer }, 0);

  // iterate through each face of the cubemap.
  api.clearRenderTargetView(FColor::BLACK, _pOutput);
  for (uint32 i = 0; i < 6; ++i) {
    const CBFloat face(static_cast<float>(i));
    api.setRenderTarget(_pOutput, nullptr, i);
    api.updateConstantBuffer(cBuffer, &face, CBFloatSize);
    api.draw(3, 0);
  }

  // unbind resources.
  api.waitDevice();
  api.pSUnbindShaderResourceViews(1);
  api.unbindRenderTargets(1);

  LOG_REGISTER("Generated Cubemap from texture.", __FILE__, __LINE__);
}

void
RendererManager::generateIrradianceCubeMap(const SPtr<Texture>& _pInput,
                                           const SPtr<Texture>& _pOutput)
{
  PK_ASSERT(_pInput);
  PK_ASSERT(_pOutput);

  GraphicsAPI& api = g_GraphicAPI();
  ShaderManager& shaderMan = g_ShaderManager();

  const ShaderKey vShaderKey("resources/pkQuadShader.pks", "VS", "vs_5_0");
  const ShaderKey pShaderKey("resources/pkIrradianceShader.pks", "PS", "ps_5_0");
  SPtr<Shader> pShader = shaderMan.getShader(pShaderKey);
  SPtr<Shader> vShader = shaderMan.getShader(vShaderKey);
  const SPtr<InputLayout> iLayout = api.createInputLayoutFromVShader(vShader);

  // create the sampler state.
  SPtr<SamplerState> samplerState = api.createSamplerState(PK_SAM_STATE_ADRESS::kWrap,
    PK_SAM_STATE_FILTERS::kFilterMigMagMipLinear);

  const Vector2 viewportSize = _pOutput->getSize();

  // create constant buffer.
  const uint32 CBFloatSize = sizeof(CBFloat);
  SPtr<ConstantBuffer> cBuffer = api.createConstantBuffer(CBFloatSize);

  // set the necessary resources.
  api.setViewport(viewportSize);
  api.setInputLayout(iLayout);
  api.setVShader(vShader);
  api.setPShader(pShader);
  api.setSampler(samplerState);
  api.pSSetShaderResourceViews({ _pInput });
  api.pSSetConstantBuffers({ cBuffer }, 0);

  // iterate through each face of the cubemap.
  api.clearRenderTargetView(FColor::BLACK, _pOutput);
  for (uint32 i = 0; i < 6; ++i) {
    const CBFloat face(static_cast<float>(i));
    api.setRenderTarget(_pOutput, nullptr, i);
    api.updateConstantBuffer(cBuffer, &face, CBFloatSize);
    api.draw(3, 0);
  }

  // unbind resources.
  api.waitDevice();
  api.pSUnbindShaderResourceViews(1);
  api.unbindRenderTargets(1);

  LOG_REGISTER("Generated Irradiance map from cube texture.", __FILE__, __LINE__);
}

void
RendererManager::generateLUT(const SPtr<Texture>& _pOutput)
{
  PK_ASSERT(_pOutput);

  LOG_REGISTER("Generated a LUT.", __FILE__, __LINE__);
}

template<class T> void
RendererManager::updateBuffer(const T& _data, const SPtr<ConstantBuffer>& _pCBuffer)
{
  g_GraphicAPI().updateConstantBuffer(_pCBuffer, &_data, 0);
}

void
RendererManager::renderActors(const Vector<SPtr<Actor>>& _gameActors)
{
  // iterate through each actor.
  const uint32 actorCount = static_cast<uint32>(_gameActors.size());
  for (uint32 i = 0; i < actorCount; ++i) {
    const SPtr<Actor> currActor = _gameActors[i];
    // if actor is not active
    if (!currActor->isActive()) {
      continue;
    }
    // use the parent transform to modify the current transform.
    SPtr<Actor> parent = currActor->getParent();
    Matrix4 transform = currActor->m_transform;
    while (parent) {
      transform *= parent->m_transform;
      parent = parent->getParent();
    }

    // render model components in the actor.
    const Vector<SPtr<Model>> models = currActor->getComponents<Model>();
    const uint32 modelCount = static_cast<uint32>(models.size());
    for (uint32 i = 0; i < modelCount; ++i) {
      const SPtr<Model> model = models[i];
      if (model && model->isActive()) {
        renderModel(model, transform);
      }
    }
    // if the actor has children, do the same for them.
    if (currActor->hasChildren()) {
      renderActors(currActor->getChildren());
    }
  }
}


void
RendererManager::renderModel(const SPtr<Model>& _model, const Matrix4& _actorTransform)
{
  // get a reference from managers and passes.
  RendererManager& rManager = g_RenderManager();
  GraphicsAPI& api = g_GraphicAPI();

  const SPtr<Pass> basePass = rManager.getPass(PASS_TYPE::kP_Base);
  const SPtr<Pass> transparencyPass = rManager.getPass(PASS_TYPE::kP_Transparency);
  const SPtr<Pass> lightPositionsPass = rManager.getPass(PASS_TYPE::kP_LightPositions);

  // for each mesh in the model
  const uint32 meshCount = static_cast<uint32>(_model->meshes.size());
  const uint32 sizeProps = sizeof(CBMaterialProps);

  // get the main camera.
  CBCamera cBCamera;
  const uint32 cameracount = static_cast<uint32>(m_cameras.size());
  for (uint32 i = 0; i < cameracount; ++i) {
    SPtr<Camera> cam = m_cameras[i];
    if (cam->m_isMain) {
      cBCamera = CBCamera(cam);
      break;
    }
  }

  const uint32 sizeWVP = sizeof(CBWVP);
  api.setVertexBuffer(_model->m_vertexB);
  api.setIndexBuffer(_model->m_indexB);

  // offsets
  uint32 currentVertexOrigin = 0;
  uint32 currentIndexOrigin = 0;

  for (uint32 i = 0; i < meshCount; ++i) {
    const SPtr<Mesh> mesh = _model->meshes[i];
    const SPtr<Material> material = mesh->material;

    // if the mesh can be rendered.
    if (mesh->getActive() && material) {
      const Matrix4 transform = mesh->m_transform * _actorTransform;

      // update the constant buffers.
      CBWVP wvp(transform, cBCamera.view, cBCamera.projection);
    
      // set resources to the pixel shader.
      api.pSSetShaderResourceViews(material->getTextures());
      // update the constant buffers.
      basePass->updateCBuffer(0, &wvp, sizeWVP);
      basePass->updateCBuffer(1, &material->m_properties, sizeProps);
      transparencyPass->updateCBuffer(0, &wvp, sizeWVP);
      transparencyPass->updateCBuffer(1, &material->m_properties, sizeProps);
      lightPositionsPass->updateCBuffer(0, &wvp, sizeWVP);
      // render the mesh.
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
