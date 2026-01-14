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
  SPtr<Texture> albedoRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Albedo, albedoRT });

  SPtr<Texture> albedoTranspRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_TranspAlbedo, albedoTranspRT });

  // create the normal render target that will store the normals of the world.
  SPtr<Texture> normalRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Normal, normalRT });

  SPtr<Texture> normalTranspRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_TranspNormal, normalTranspRT });

  // render target for the metallic result.
  SPtr<Texture> ormRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_ORM, ormRT });

  SPtr<Texture> ormTranspRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_TranspORM, ormTranspRT });

  SPtr<Texture> ssaoRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_SSAO, ssaoRT });

  // BRDF texture.
  SPtr<Texture> brdfRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_BRDF, brdfRT });

  SPtr<Texture> brdfTranspRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_BRDFTransp, brdfTranspRT });

  // skybox texture.
  SPtr<Texture> skyboxRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Skybox, skyboxRT });

  // positions texture.
  SPtr<Texture> posRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Positions, posRT });

  SPtr<Texture> posTranspRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_TranspPos, posTranspRT });

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

  SPtr<Texture> emissiveTranspRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_TranspEmiss, emissiveTranspRT });

  SPtr<Texture> emissiveHBlurRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_EmissiveHBlur, emissiveHBlurRT });

  SPtr<Texture> emissiveBlurRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_EmissiveBlur, emissiveBlurRT });

  // luminance texture.
  SPtr<Texture> lumRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Luminance, lumRT });

  SPtr<Texture> lumBlurHRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_LumBlurH, lumBlurHRT });

  SPtr<Texture> lumBlurRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_LumBlur, lumBlurRT });

  SPtr<BaseResource> resSky = make_shared<TextureResource>();
  
  g_TextureCodec().createResourceFromFile(Path("textures/Skybox_papermill.hdr"));
  const bool success = resSky->softLoad(Path("resources/Skybox_papermill.pkt"));
  m_mainSkybox = api.createEmptyTexture();
  if (success) {
    assetMan.insertNewResource(resSky);
    SPtr<Texture> skyboxResource = tm.loadTexture(resSky->m_id);
    m_mainSkybox->copyFrom(skyboxResource);
  }

  // Cube map for the skybox
  txDesc.width = 2048;
  txDesc.height = 2048;
  txDesc.isCube = true;
  SPtr<Texture> cubeMapRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_CubeMap, cubeMapRT });
  txDesc.isCube = false;

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
  SPtr<Texture> depthBuffer = api.createTexture(txDesc);
  m_depthBuffers.insert({ D_BUFFERS::kDB_Base, depthBuffer });

  SPtr<Texture> transparencyDepth = api.createTexture(txDesc);
  m_depthBuffers.insert({ D_BUFFERS::kDB_Transparency, transparencyDepth });

  // light depth buffer
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

  // generateCubeMap(m_mainSkybox, cubeMapRT);
  
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
  SPtr<Texture> albedoRT = getGBuffer(G_BUFFERS::kGB_Albedo);
  SPtr<Texture> normalRT = getGBuffer(G_BUFFERS::kGB_Normal);
  SPtr<Texture> ormRT = getGBuffer(G_BUFFERS::kGB_ORM);
  SPtr<Texture> ssaoRT = getGBuffer(G_BUFFERS::kGB_SSAO);
  SPtr<Texture> brdfRT = getGBuffer(G_BUFFERS::kGB_BRDF);
  SPtr<Texture> posRT = getGBuffer(G_BUFFERS::kGB_Positions);
  SPtr<Texture> posLightRT = getGBuffer(G_BUFFERS::kGB_PositionsLight);
  SPtr<Texture> emissRT = getGBuffer(G_BUFFERS::kGB_Emissive);
  SPtr<Texture> emissHBlurRT = getGBuffer(G_BUFFERS::kGB_EmissiveHBlur);
  SPtr<Texture> emissBlurRT = getGBuffer(G_BUFFERS::kGB_EmissiveBlur);
  SPtr<Texture> skyboxRT = getGBuffer(G_BUFFERS::kGB_Skybox);
  SPtr<Texture> lumBlurRT = getGBuffer(G_BUFFERS::kGB_LumBlur);
  SPtr<Texture> cubeMapRT = getGBuffer(G_BUFFERS::kGB_CubeMap);

  SPtr<Texture> transpAlbedo = getGBuffer(G_BUFFERS::kGB_TranspAlbedo);
  SPtr<Texture> transpNormal = getGBuffer(G_BUFFERS::kGB_TranspNormal);
  SPtr<Texture> transpORM = getGBuffer(G_BUFFERS::kGB_TranspORM);
  SPtr<Texture> transpEmiss = getGBuffer(G_BUFFERS::kGB_TranspEmiss);
  SPtr<Texture> transpPos = getGBuffer(G_BUFFERS::kGB_TranspPos);
  SPtr<Texture> brdfTranspRT = getGBuffer(G_BUFFERS::kGB_BRDFTransp);

  // Depth textures
  SPtr<Texture> DepthBuffer = getDepthBuffer(D_BUFFERS::kDB_Base);
  SPtr<Texture> LightDepthBuffer = getDepthBuffer(D_BUFFERS::kDB_Light);
  SPtr<Texture> transpDepth = getDepthBuffer(D_BUFFERS::kDB_Transparency);
  /****************************************************************************
   * Create the base pass.
   ***************************************************************************/
  pDesc.vSDirectory = "resources/pkVShader.pks";
  pDesc.pSDirectory = "resources/pkPShader.pks";
  pDesc.samAdress = PK_SAM_STATE_ADRESS::kWrap;
  pDesc.samFilters = PK_SAM_STATE_FILTERS::kFilterMigMagMipLinear;
  pDesc.cBSizes = { sizeof(CBMatrix),
                    sizeof(CBMatrix),
                    sizeof(CBMatrix),
                    sizeof(CBMaterialProps) };
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
  // insert to the pass map.
  m_passes.insert({ PASS_TYPE::kP_Base, basePass });

  /****************************************************************************
   * Create the transparency pass.
   ***************************************************************************/
  pDesc.pSDirectory = "resources/pkPSTransparency.pks";
  pDesc.outputs = { transpAlbedo, transpNormal, transpORM, transpEmiss, transpPos };
  pDesc.pDepth = transpDepth;
  // make the pass
  SPtr<Pass> transparencyPass = make_shared<Pass>(pDesc);
  // insert to the pass map.
  m_passes.insert({ PASS_TYPE::kP_Transparency, transparencyPass });

  /****************************************************************************
   * Shadow Pass
   ***************************************************************************/
  pDesc.pSDirectory = "resources/pkPShaderDepth.pks";
  pDesc.outputs = { posLightRT };
  pDesc.pDepth = LightDepthBuffer;
  SPtr<Pass> ligtPosPass = make_shared<Pass>(pDesc);
  m_passes.insert({ PASS_TYPE::kP_LightPositions, ligtPosPass });

  /****************************************************************************
   * Shadow Specular Quad Pass
   ***************************************************************************/
  pDesc.vSDirectory = "resources/pkQuadShader.pks";
  pDesc.pSDirectory = "resources/pkLightShader.pks";
  pDesc.cBSizes = { sizeof(CBLight), sizeof(CBCamera), sizeof(Matrix4), sizeof(Vector4), sizeof(Vector4)};
  pDesc.inputs = { DepthBuffer, posRT, posLightRT, albedoRT, normalRT, ormRT, m_mainSkybox, cubeMapRT };
  pDesc.outputs = { brdfRT };
  pDesc.pDepth = {};
  SPtr<Pass> lightQuad = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_Light, lightQuad });

  /****************************************************************************
   * Shadow Specular Transparency Quad Pass
   ***************************************************************************/
  pDesc.inputs = { transpDepth, transpPos, posLightRT, transpAlbedo, transpNormal, transpORM, m_mainSkybox, cubeMapRT };
  pDesc.outputs = { brdfTranspRT };
  SPtr<Pass> lightTranspQuad = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_LightTransparency, lightTranspQuad });

  /****************************************************************************
   * Skybox Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = "resources/pkSkyboxShader.pks";
  pDesc.cBSizes = { sizeof(Matrix4), sizeof(Matrix4) };
  pDesc.inputs = { m_mainSkybox };
  pDesc.outputs = { skyboxRT };
  SPtr<Pass> skyboxPass = make_shared<Pass>(pDesc);
  m_passes.insert({ PASS_TYPE::kP_SkyBox, skyboxPass });

  /****************************************************************************
   * Screen Space Ambient Occlusion Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = "resources/pkPSAOshader.pks";
  pDesc.cBSizes = { sizeof(CBSSAO), sizeof(CBVector2x2) };
  pDesc.inputs = { posRT,
                   normalRT };
  pDesc.outputs = { ssaoRT };
  pDesc.samAdress = PK_SAM_STATE_ADRESS::kWrap;
  SPtr<Pass> ssaoPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_SSAO, ssaoPass });

  /****************************************************************************
   * Emissive Horizontal Blur Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = "resources/pkBlur.pks";
  pDesc.cBSizes = { sizeof(CBBlur) };
  pDesc.inputs = { emissRT };
  pDesc.outputs = { emissHBlurRT };
  pDesc.samAdress = PK_SAM_STATE_ADRESS::kClamp;
  SPtr<Pass> emissiveHPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_EmissiveHBlur, emissiveHPass });

  /****************************************************************************
   * Emissive Blur Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = "resources/pkBlur.pks";
  pDesc.cBSizes = { sizeof(CBBlur) };
  pDesc.inputs = { emissHBlurRT };
  pDesc.outputs = { emissBlurRT };
  SPtr<Pass> emissivePass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_EmissiveBlur, emissivePass });

  /****************************************************************************
   * Luminance
   ***************************************************************************/
  pDesc.pSDirectory = "resources/pkLuminanceQuad.pks";
  pDesc.cBSizes = { sizeof(CBVector2x2) };
  pDesc.inputs = { brdfRT };
  pDesc.outputs = { getGBuffer(G_BUFFERS::kGB_Luminance) };
  SPtr<Pass> lumPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_Luminance, lumPass });

  /****************************************************************************
   * Luminance Horizontal Blur
   ***************************************************************************/
  pDesc.pSDirectory = "resources/pkBlur.pks";
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
  pDesc.pSDirectory = "resources/pkBlur.pks";
  pDesc.cBSizes = { sizeof(CBBlur) };
  pDesc.inputs = { getGBuffer(G_BUFFERS::kGB_LumBlurH) };
  pDesc.outputs = { lumBlurRT };
  SPtr<Pass> lumBlurPass = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_LumBlur, lumBlurPass });

  /****************************************************************************
   * Tone mapping Quad pass
   ***************************************************************************/
  pDesc.pSDirectory = "resources/pkToneMap.pks";
  pDesc.cBSizes = { sizeof(CBFloat) };
  pDesc.inputs = { brdfRT,
                   lumBlurRT,
                   emissRT,
                   emissBlurRT,
                   skyboxRT,
                   DepthBuffer,
                   transpDepth,
                   brdfTranspRT };
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

void
RendererManager::generateCubeMap(const SPtr<Texture>& _pInput, const SPtr<Texture>& _pOutput)
{
  PK_ASSERT(_pInput);
  PK_ASSERT(_pOutput);

  GraphicsAPI& api = g_GraphicAPI();
  Logger& log = g_Logger();
  AssetResourceManager& assetMan = g_AssetResourceManager();
  ShaderManager& shaderMan = g_ShaderManager();

  // get vertex shader resource.
  SPtr<BaseResource> vShadRes = assetMan.getResourceBydirectory("resources/pkQuadShader.pks");
  SPtr<BaseResource> pShadRes = assetMan.getResourceBydirectory("resources/pkCubeMapShader.pks");

  PK_ASSERT(vShadRes);
  PK_ASSERT(pShadRes);

  vShadRes->load();
  pShadRes->load();

  // create the vertex shader.
  SPtr<Shader> vShader = api.internalCreateShader();
  vShader->compileFromResource(vShadRes);
  if (!vShader) {
    const String msg = "Could not create vertex shader to generate cubeMap.";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kFatal);
    return;
  }
  api.createVShader(vShader);
  const SPtr<InputLayout> iLayout = api.createInputLayoutFromVShader(vShader);

  // save shader
  const ShaderKey vShaderKey("resources/pkQuadShader.pks", "vs_5_0", "VS");
  shaderMan.insertShader(vShaderKey, vShader);

  // create the pixel shader.
  SPtr<Shader> pShader = api.internalCreateShader();
  pShader->compileFromResource(pShadRes);
  if (!pShader) {
    const String msg = "Could not create pixel shader to generate cubeMap.";
    LOG_FATAL(msg, __FILE__, __LINE__);
    THROW_ERROR(msg);
    return;
  }
  api.createPShader(pShader);

  // save shader
  const ShaderKey pShaderKey("resources/pkCubeMapShader.pks", "ps_5_0", "PS");
  shaderMan.insertShader(pShaderKey, pShader);

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
  api.clearRenderTargetView(Color::BLACK, _pOutput);
  for (uint32 i = 0; i < 6; ++i) {
    api.setRenderTarget(_pOutput, nullptr, i);
    const CBFloat data(static_cast<float>(i));
    api.updateConstantBuffer(cBuffer, &data, CBFloatSize);
    api.draw(3, 0);
  }

  // unbind resources.
  api.waitDevice();
  api.pSSetShaderResourceViews({ nullptr });
  api.unbindRenderTargets(1);

  LOG_REGISTER("Generated Cubemap from texture.", __FILE__, __LINE__);
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
    SPtr<Actor> parent = currActor->m_parent;
    Matrix4 transform = currActor->m_transform;
    while (parent) {
      transform *= parent->m_transform;
      parent = parent->m_parent;
    }

    // render the model of the actor.
    const SPtr<Model> model = currActor->getComponent<Model>();
    if (model && model->isActive()) {
      renderModel(model, transform);
    }
    // if the actor has children, do the same for them.
    if (!currActor->m_children.empty()) {
      renderActors(currActor->m_children);
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

  const SIZE_T mat4x4Size = sizeof(Matrix4);

  api.setVertexBuffer(_model->m_vertexB);
  api.setIndexBuffer(_model->m_indexB);
  // offsets
  uint32 currentVertexOrigin = 0;
  uint32 currentIndexOrigin = 0;
  // for each mesh in the model
  const uint32 meshCount = static_cast<uint32>(_model->meshes.size());
  const uint32 sizeProps = sizeof(CBMaterialProps);
  for (uint32 i = 0; i < meshCount; ++i) {
    // get the material
    const SPtr<Mesh> mesh = _model->meshes[i];
    // get if the mesh is active or not, if it's not, dont render and keep going.
    const SPtr<Material> material = mesh->material;
    if (mesh->getActive() &&  material) {
      // set resources to the pixel shader.
      api.pSSetShaderResourceViews(material->getTextures());
      // update the constant buffers.
      const Matrix4 transform = mesh->m_transform * _actorTransform;
      basePass->updateCBuffer(2, &transform, mat4x4Size);
      basePass->updateCBuffer(3, &material->m_properties, sizeProps);
      transparencyPass->updateCBuffer(2, &transform, mat4x4Size);
      transparencyPass->updateCBuffer(3, &material->m_properties, sizeProps);
      lightPositionsPass->updateCBuffer(2, &transform, mat4x4Size);
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
