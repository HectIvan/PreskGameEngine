#include "pkCamera.h"
#include "pkGraphicsAPI.h"
#include "pkLogger.h"
#include "pkRendererManager.h"
#include "pkSamplerState.h"
#include "pkSceneManager.h"
#include "pkTextureManager.h"

using TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_FLOAT;
using PK_USAGE::kPK_USAGE_DEFAULT;
using PK_BIND_FLAG::kPK_BIND_DEPTH_STENCIL;
using PK_BIND_FLAG::kPK_BIND_RENDER_TARGET;
using PK_BIND_FLAG::kPK_BIND_SHADER_RESOURCE;
using PK_BIND_FLAG::kPK_BIND_UNORDERED_ACCESS;

namespace pkEngineSDK
{

  // to do: change albedoRTV to a correct semantic
void RendererManager::init(Window& _window)
{
  GraphicsAPI& api = g_GraphicAPI().instance();

  uint32 winHeight = static_cast<uint32>(_window.getClientWidthHeight().y);
  uint32 winWidth = static_cast<uint32>(_window.getClientWidthHeight().x);

  // uint32 winHeight = _window.getHeight();
  // uint32 winWidth = _window.getWidth();

  SPtr<Texture> albedoRTV = api.createTexture(nullptr,
                                              4,
                                              winWidth,
                                              winHeight,
                                              kPK_FORMAT_R32G32B32A32_FLOAT,
                                              kPK_USAGE_DEFAULT,
                                              kPK_BIND_SHADER_RESOURCE | kPK_BIND_RENDER_TARGET,
                                              false,
                                              kPK_FORMAT_R32G32B32A32_FLOAT);
  m_gBuffers.insert({ G_BUFFERS::kGB_Albedo, albedoRTV });

  // create the normal render target that will store the normals of the world
  SPtr<Texture> normalRT = api.createTexture(nullptr,
                                             4,
                                             winWidth,
                                             winHeight,
                                             kPK_FORMAT_R32G32B32A32_FLOAT,
                                             kPK_USAGE_DEFAULT,
                                             kPK_BIND_SHADER_RESOURCE | kPK_BIND_RENDER_TARGET,
                                             false,
                                             kPK_FORMAT_R32G32B32A32_FLOAT);
  m_gBuffers.insert({ G_BUFFERS::kGB_Normal, normalRT });

  // m_pDepthSView = api.createDepthStencilView(m_pDepthRT);
  // create the main camera depth buffer to store pixel distance from world to main camera.
  SPtr<Texture> depthBuffer = api.createTexture(nullptr,
                                                4,
                                                winWidth,
                                                winHeight,
                                                TEXTURE_FORMAT::kPK_FORMAT_R32_TYPELESS,
                                                kPK_USAGE_DEFAULT,
                                                kPK_BIND_SHADER_RESOURCE |
                                                kPK_BIND_DEPTH_STENCIL,
                                                false,
                                                TEXTURE_FORMAT::kPK_FORMAT_R32_FLOAT);
  m_depthBuffers.insert({ D_BUFFERS::kDB_Base, depthBuffer });

  // create the depth buffer for the light camera rendering in shadow mapping
  SPtr<Texture> shadowDepth = api.createTexture(nullptr,
                                                4,
                                                winWidth,
                                                winHeight,
                                                TEXTURE_FORMAT::kPK_FORMAT_R32_TYPELESS,
                                                kPK_USAGE_DEFAULT,
                                                kPK_BIND_SHADER_RESOURCE |
                                                kPK_BIND_DEPTH_STENCIL,
                                                false,
                                                TEXTURE_FORMAT::kPK_FORMAT_R32_FLOAT);
  m_depthBuffers.insert({ D_BUFFERS::kDB_Shadow, shadowDepth });
  

  // create the passes needed
  createPasses();
}

void
RendererManager::createPasses()
{
  /****************************************************************************
   * Create the base pass.
   ***************************************************************************/
  SPtr<Pass> basePass = make_shared<Pass>();
  // create all pointers
  basePass->create();
  // set the data for the shaders to be compiled, and compile.
  basePass->setVSData(L"shaders/pkVShader.hlsl", "VS", "vs_5_0");
  basePass->setPSData(L"shaders/pkPShader.hlsl", "PS", "ps_5_0");
  basePass->compileShaders();
  basePass->createShaders();
  // create the vertex shader input layout && sampler state.
  basePass->createInputLayout();
  basePass->createSamplerState(SAM_STATE_ADRESS::kWrap,
                               SAM_STATE_FILTERS::kFilterMigMagMipLinear);

  // constant buffers
  basePass->createCBuffer(static_cast<uint32>(sizeof(CBView)), nullptr, 0);
  basePass->createCBuffer(static_cast<uint32>(sizeof(CBProjection)), nullptr, 0);
  basePass->createCBuffer(static_cast<uint32>(sizeof(CBTransform)), nullptr, 0);
  basePass->createCBuffer(static_cast<uint32>(sizeof(CBLight)), nullptr, 0);
  basePass->createCBuffer(static_cast<uint32>(sizeof(CBCamera)), nullptr, 0);

  g_GraphicAPI().setInputLayout(basePass->getInputLayout());
  // insert to the pass map.
  m_passes.insert({ PASS_TYPE::kP_Base, basePass });

  /****************************************************************************
   * Shadow Pass
   ***************************************************************************/
  SPtr<Pass> shadowPass = make_shared<Pass>();
  // create all pointers
  shadowPass->create();
  shadowPass->setVSData(L"shaders/pkVShader.hlsl", "VS", "vs_5_0");
  shadowPass->setPSData(L"shaders/pkPShader.hlsl", "PS", "ps_5_0");
  shadowPass->compileShaders();
  shadowPass->createShaders();

  // create constant buffers
  shadowPass->createCBuffer(static_cast<uint32>(sizeof(CBView)), nullptr, 0);
  shadowPass->createCBuffer(static_cast<uint32>(sizeof(CBProjection)), nullptr, 0);
  shadowPass->createCBuffer(static_cast<uint32>(sizeof(CBTransform)), nullptr, 0);
  shadowPass->createCBuffer(static_cast<uint32>(sizeof(CBLight)), nullptr, 0);
  shadowPass->createCBuffer(static_cast<uint32>(sizeof(CBCamera)), nullptr, 0);

  shadowPass->createInputLayout();
  shadowPass->createSamplerState(SAM_STATE_ADRESS::kWrap,
                                 SAM_STATE_FILTERS::kFilterMigMagMipLinear);
  m_passes.insert({ PASS_TYPE::kP_Shadow, shadowPass });

  /****************************************************************************
   * Ambient Occlussion Pass
   ***************************************************************************/
  SPtr<Pass> AOPass = make_shared<Pass>();
  // create all pointers
  AOPass->create();
  // set and compile shaders
  AOPass->setVSData(L"shaders/pkDeferredShader.hlsl", "VS", "vs_5_0");
  AOPass->setPSData(L"shaders/pkPSAOshader.hlsl", "PS", "ps_5_0");
  AOPass->compileShaders();
  AOPass->createShaders();

  // create the buffers needed (reminder to remember the order in which they are created)
  AOPass->createCBuffer(static_cast<uint32>(sizeof(CBAOData)), nullptr, 0);

  AOPass->createInputLayout();
  AOPass->createSamplerState(SAM_STATE_ADRESS::kClamp,
                             SAM_STATE_FILTERS::kFilterMigMagMipLinear);
  m_passes.insert({ PASS_TYPE::kP_AO, AOPass });

  /****************************************************************************
   * Shadow Deferred pass
   ***************************************************************************/
  SPtr<Pass> shadowDef = make_shared<Pass>();
  // create all pointers
  shadowDef->create();
  // set and compile shaders
  shadowDef->setVSData(L"shaders/pkDeferredShader.hlsl", "VS", "vs_5_0");
  shadowDef->setPSData(L"shaders/pkShadowMapping.hlsl", "PS", "ps_5_0");
  shadowDef->compileShaders();
  shadowDef->createShaders();

  shadowDef->createCBuffer(static_cast<uint32>(sizeof(CBCamera)), nullptr, 0); // light camera
  shadowDef->createCBuffer(static_cast<uint32>(sizeof(CBCamera)), nullptr, 0); // main camera
  shadowDef->createCBuffer(static_cast<uint32>(sizeof(Matrix4)), nullptr, 0); // light transform
  shadowDef->createCBuffer(static_cast<uint32>(sizeof(Matrix4)), nullptr, 0); // camera transform

  shadowDef->createInputLayout();
  shadowDef->createSamplerState(SAM_STATE_ADRESS::kClamp,
                                SAM_STATE_FILTERS::kFilterMigMagMipLinear);
  m_passes.insert({ PASS_TYPE::kP_ShadowDef, shadowDef });

  /****************************************************************************
   * Test pass
   ***************************************************************************/
  SPtr<Pass> testPass = make_shared<Pass>();
  // create all pointers
  testPass->create();
  // set and compile shaders
  testPass->setVSData(L"shaders/pkDeferredShader.hlsl", "VS", "vs_5_0");
  testPass->setPSData(L"shaders/pkTestDefShader.hlsl", "PS", "ps_5_0");
  testPass->compileShaders();
  testPass->createShaders();

  testPass->createInputLayout();
  testPass->createSamplerState(SAM_STATE_ADRESS::kClamp,
                               SAM_STATE_FILTERS::kFilterMigMagMipLinear);
  m_passes.insert({ PASS_TYPE::kP_Test, testPass });
}

SPtr<Pass>
RendererManager::getPass(PASS_TYPE::E _type)
{
  return m_passes.find(_type)->second;
}

SPtr<Texture>
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

SPtr<Texture>
RendererManager::getDepthBuffer(D_BUFFERS::E _type)
{
  return m_depthBuffers.find(_type)->second;
}

void
RendererManager::compileShaders()
{
  Map<PASS_TYPE::E, SPtr<Pass>>::iterator it;
  for (it = m_passes.begin(); it != m_passes.end(); ++it) {
    // Compile shaders
    it->second->compileShaders();
    it->second->createShaders();
    g_Logger().print("recompiled shaders.");
  }
}

template<class T> void
RendererManager::updateBuffer(T& _data, SPtr<ConstantBuffer> _pCBuffer)
{
  g_GraphicAPI().updateConstantBuffer(_pCBuffer, &_data, 0);
}
  
void
RendererManager::VSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>> _cBuffers)
{
  // set the constant buffers
  for (uint32 i = 0; i < _cBuffers.size(); ++i) {
    g_GraphicAPI().VSSetConstantBuffer(_cBuffers[i], i, 1);
  }
}

void
RendererManager::PSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>> _cBuffers)
{
  // set the constant buffers
  for (uint32 i = 0; i < _cBuffers.size(); ++i) {
    g_GraphicAPI().PSSetConstantBuffer(_cBuffers[i], i, 1);
  }
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
      api.setVertexBuffer(actor->getComponent<Model>()->vertexB);
      api.setIndexBuffer(actor->getComponent<Model>()->indexB);
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
    if (!_gameActors[i]->m_active) {
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
  api.setVertexBuffer(_model.vertexB);
  api.setIndexBuffer(_model.indexB);
  // offsets
  uint32 currentVertexOrigin = 0;
  uint32 currentIndexOrigin = 0;
  // for each mesh in the model
  for (uint32 i = 0; i < _model.meshes.size(); ++i) {
    // get the material
    SPtr<Material> material = _model.meshes[i]->material;
    // set the material textures to the shader
    api.PSSetShaderResourceView(material->diffuse, 0);
    api.PSSetShaderResourceView(material->normal, 1);
    api.PSSetShaderResourceView(material->height, 2);
    api.PSSetShaderResourceView(material->metallic, 3);
    api.PSSetShaderResourceView(material->occlusion, 4);
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
