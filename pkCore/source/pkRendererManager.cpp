#include "pkAssetResourceManager.h"
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
  const SPtr<Texture> albedoRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Albedo, albedoRT });

  // create the normal render target that will store the normals of the world.
  const SPtr<Texture> normalRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Normal, normalRT });

  // render target for the metallic result.
  const SPtr<Texture> ormRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_ORM, ormRT });

  const SPtr<Texture> ssaoRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_SSAO, ssaoRT });

  // BRDF texture.
  const SPtr<Texture> brdfRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_BRDF, brdfRT });

  // skybox texture.
  const SPtr<Texture> skyboxRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Skybox, skyboxRT });

  // positions texture.
  const SPtr<Texture> posRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Positions, posRT });

  // positions texture for the light.
  txDesc.width = winWidth * sizeMulShadow;
  txDesc.height = winHeight * sizeMulShadow;
  const SPtr<Texture> posLightRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_PositionsLight, posLightRT });
  txDesc.width = winWidth;
  txDesc.height = winHeight;

  // emissive texture.
  const SPtr<Texture> emissiveRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Emissive, emissiveRT });

  const SPtr<Texture> emissiveHBlurRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_EmissiveHBlur, emissiveHBlurRT });

  const SPtr<Texture> emissiveBlurRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_EmissiveBlur, emissiveBlurRT });

  // luminance texture.
  const SPtr<Texture> lumRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_Luminance, lumRT });

  const SPtr<Texture> lumBlurHRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_LumBlurH, lumBlurHRT });

  const SPtr<Texture> lumBlurRT = api.createTexture(txDesc);
  m_gBuffers.insert({ G_BUFFERS::kGB_LumBlur, lumBlurRT });

  const SPtr<BaseResource> resSky = make_shared<TextureResource>();
  const bool success = resSky->softLoad(Path("resources/Skybox_papermill.pkt"));
  m_mainSkybox = api.createEmptyTexture();
  if (success) {
    assetMan.insertNewResource(resSky);
    SPtr<Texture> skyboxResource = tm.loadTexture(resSky->m_id);
    m_mainSkybox->copyFrom(skyboxResource);
  }

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
  const SPtr<Texture> shadowDepth = api.createTexture(txDesc);
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
  const SPtr<Texture> albedoRT = getGBuffer(G_BUFFERS::kGB_Albedo);
  const SPtr<Texture> normalRT = getGBuffer(G_BUFFERS::kGB_Normal);
  const SPtr<Texture> ormRT = getGBuffer(G_BUFFERS::kGB_ORM);
  const SPtr<Texture> ssaoRT = getGBuffer(G_BUFFERS::kGB_SSAO);
  const SPtr<Texture> brdfRT = getGBuffer(G_BUFFERS::kGB_BRDF);
  const SPtr<Texture> posRT = getGBuffer(G_BUFFERS::kGB_Positions);
  const SPtr<Texture> posLightRT = getGBuffer(G_BUFFERS::kGB_PositionsLight);
  const SPtr<Texture> emissRT = getGBuffer(G_BUFFERS::kGB_Emissive);
  const SPtr<Texture> emissHBlurRT = getGBuffer(G_BUFFERS::kGB_EmissiveHBlur);
  const SPtr<Texture> emissBlurRT = getGBuffer(G_BUFFERS::kGB_EmissiveBlur);
  const SPtr<Texture> skyboxRT = getGBuffer(G_BUFFERS::kGB_Skybox);
  const SPtr<Texture> lumBlurRT = getGBuffer(G_BUFFERS::kGB_LumBlur);

  // Depth textures
  const SPtr<Texture> DepthBuffer = getDepthBuffer(D_BUFFERS::kDB_Base);
  const SPtr<Texture> LightDepthBuffer = getDepthBuffer(D_BUFFERS::kDB_Light);
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
  pDesc.inputs = { DepthBuffer, posRT, posLightRT, albedoRT, normalRT, ormRT, m_mainSkybox };
  pDesc.outputs = { brdfRT };
  pDesc.pDepth = {};
  SPtr<Pass> lightQuad = make_shared<Pass>(pDesc);
  // insert to the passes
  m_passes.insert({ PASS_TYPE::kP_Light, lightQuad });

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
                   DepthBuffer };
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
RendererManager::renderActors(const Vector<SPtr<Actor>>& _gameActors)
{
  // for each actor
  const uint32 actorCount = static_cast<uint32>(_gameActors.size());
  for (uint32 i = 0; i < actorCount; ++i) {
    const SPtr<Actor> currActor = _gameActors[i];
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

    // render the model of the actor
    const SPtr<Model> model = currActor->getComponent<Model>();
    if (model && model->isActive()) {
      renderModel(model, transform);
    }
    // if the actor has children, do the same for them (recursive)
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
  const SPtr<Pass> lightPositionsPass = rManager.getPass(PASS_TYPE::kP_LightPositions);

  api.setVertexBuffer(_model->m_vertexB);
  api.setIndexBuffer(_model->m_indexB);
  // offsets
  uint32 currentVertexOrigin = 0;
  uint32 currentIndexOrigin = 0;
  // for each mesh in the model
  for (uint32 i = 0; i < _model->meshes.size(); ++i) {
    // get the material
    const SPtr<Mesh> mesh = _model->meshes[i];
    // get if the mesh is active or not, if it's not, dont render and keep going.
    const SPtr<Material> material = mesh->material;
    if (mesh->getActive() &&  material) {
      // set the material textures to the shader
      const Vector<SPtr<Texture>> textures = { material->m_albedo,
                                               material->m_normal,
                                               material->m_height,
                                               material->m_metallic,
                                               material->m_oclussion,
                                               material->m_roughness,
                                               material->m_emissive };
      // update resources & constant buffers
      const Matrix4 transform = _actorTransform * mesh->m_transform;
      api.pSSetShaderResourceViews(textures);
      api.updateConstantBuffer(basePass->getCBuffer(2), &transform, sizeof(Matrix4));
      api.updateConstantBuffer(lightPositionsPass->getCBuffer(2), &transform, sizeof(Matrix4));
      api.updateConstantBuffer(basePass->getCBuffer(3),
                               &material->m_properties,
                               sizeof(CBMaterialProps));
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
