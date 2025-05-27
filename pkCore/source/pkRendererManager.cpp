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
void RendererManager::init(Window& _window)
{
  GraphicsAPI& api = g_GraphicAPI().instance();
  // create the render targets and depth stencil view
  m_pRTargetView = api.createRenderTargetView();

  uint32 winHeight = _window.getHeight();
  uint32 winWidth = _window.getWidth();

  m_pDepthRT = api.createTexture(nullptr,
                                 4,
                                 winWidth,
                                 winHeight,
                                 TEXTURE_FORMAT::kPK_FORMAT_R32_TYPELESS,
                                 kPK_USAGE_DEFAULT,
                                 kPK_BIND_SHADER_RESOURCE |
                                 kPK_BIND_DEPTH_STENCIL,
                                 false,
                                 TEXTURE_FORMAT::kPK_FORMAT_R32_FLOAT);

  m_pNormalRT = api.createTexture(nullptr,
                                  4,
                                  winWidth,
                                  winHeight,
                                  kPK_FORMAT_R32G32B32A32_FLOAT,
                                  kPK_USAGE_DEFAULT,
                                  kPK_BIND_SHADER_RESOURCE | kPK_BIND_RENDER_TARGET,
                                  false,
                                  kPK_FORMAT_R32G32B32A32_FLOAT);

  m_pDepthSView = api.createDepthStencilView(m_pDepthRT);

  // create shadow mapping
  m_pShadowDepth = api.createTexture(nullptr,
                                     4,
                                     winWidth,
                                     winHeight,
                                     TEXTURE_FORMAT::kPK_FORMAT_R32_TYPELESS,
                                     kPK_USAGE_DEFAULT,
                                     kPK_BIND_SHADER_RESOURCE |
                                     kPK_BIND_DEPTH_STENCIL,
                                     false,
                                     TEXTURE_FORMAT::kPK_FORMAT_R32_FLOAT);
  
  m_pShadowDepthSV = api.createDepthStencilView(m_pShadowDepth);

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
  basePass->setPSData(L"shaders/pkVShader.hlsl", "PS", "ps_5_0");
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
  m_passes.insert({ 0, basePass });

  /****************************************************************************
   * Shadow Pass
   ***************************************************************************/
  SPtr<Pass> shadowPass = make_shared<Pass>();
  // create all pointers
  shadowPass->create();
  shadowPass->setVSData(L"shaders/pkVShader.hlsl", "VS", "vs_5_0");
  shadowPass->setPSData(L"shaders/pkVShader.hlsl", "PS", "ps_5_0");
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
  m_passes.insert({ 1, shadowPass });

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
  m_passes.insert({ 2, AOPass });

  /****************************************************************************
   * Shadow Deferred pass
   ***************************************************************************/
  SPtr<Pass> ShadowDef = make_shared<Pass>();
  // create all pointers
  ShadowDef->create();
  // set and compile shaders
  ShadowDef->setVSData(L"shaders/pkDeferredShader.hlsl", "VS", "vs_5_0");
  ShadowDef->setPSData(L"shaders/pkShadowMapping.hlsl", "PS", "ps_5_0");
  ShadowDef->compileShaders();
  ShadowDef->createShaders();

  ShadowDef->createCBuffer(static_cast<uint32>(sizeof(CBCamera)), nullptr, 0); // light camera
  ShadowDef->createCBuffer(static_cast<uint32>(sizeof(CBCamera)), nullptr, 0); // main camera
  ShadowDef->createCBuffer(static_cast<uint32>(sizeof(Matrix4)), nullptr, 0); // light transform
  ShadowDef->createCBuffer(static_cast<uint32>(sizeof(Matrix4)), nullptr, 0); // camera transform

  ShadowDef->createInputLayout();
  ShadowDef->createSamplerState(SAM_STATE_ADRESS::kClamp,
                                SAM_STATE_FILTERS::kFilterMigMagMipLinear);
  m_passes.insert({ 3, ShadowDef });
}

void
RendererManager::compileShaders()
{
  Map<uint32, SPtr<Pass>>::iterator it;
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
  GraphicsAPI& api = g_GraphicAPI().instance();
  // set the constant buffers
  for (uint32 i = 0; i < _cBuffers.size(); ++i) {
    api.VSSetConstantBuffer(_cBuffers[i], i, 1);
  }
}

void
RendererManager::PSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>> _cBuffers)
{
  GraphicsAPI& api = g_GraphicAPI().instance();
  // set the constant buffers
  for (uint32 i = 0; i < _cBuffers.size(); ++i) {
    api.VSSetConstantBuffer(_cBuffers[i], i, 1);
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
    g_GraphicAPI().updateConstantBuffer(g_RenderManager().m_passes[0]->getCBuffer(2),
                                        &transform,
                                        static_cast<uint32>(sizeof(CBTransform)));
    g_GraphicAPI().updateConstantBuffer(g_RenderManager().m_passes[1]->getCBuffer(2),
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
  // get a reference from the api
  g_GraphicAPI().setVertexBuffer(_model.vertexB);
  g_GraphicAPI().setIndexBuffer(_model.indexB);
  // offsets
  uint32 currentVertexOrigin = 0;
  uint32 currentIndexOrigin = 0;
  // for each mesh in the model
  for (uint32 i = 0; i < _model.meshes.size(); ++i) {
    // get the material
    SPtr<Material> material = _model.meshes[i]->material;
    // set the material textures to the shader
    g_GraphicAPI().PSSetShaderResourceView(material->diffuse, 0);
    g_GraphicAPI().PSSetShaderResourceView(material->normal, 1);
    g_GraphicAPI().PSSetShaderResourceView(material->height, 2);
    g_GraphicAPI().PSSetShaderResourceView(material->metallic, 3);
    g_GraphicAPI().PSSetShaderResourceView(material->occlusion, 4);
    // draw the mesh
    g_GraphicAPI().drawIndexed(static_cast<uint32>(_model.meshes[i]->numIndex),
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